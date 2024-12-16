#include "server.h"
#include <QUdpSocket>

#include <QHostInfo>

#include "broadcastsender.h"
#include "pingserver.h"

#include <QNetworkInterface>
#include <QThreadPool>

namespace Server {

namespace {

QHostAddress getHostIp() {
  const auto localhost = QHostAddress(QHostAddress::LocalHost);
  for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != localhost) {
      return address;
    }
  }

  return {};
}
}  // namespace

Server::Server(const std::shared_ptr<spdlog::logger>& logger, QObject* parent)
    : QObject{parent},
      logger(logger),
      broadcastSender(new BroadcastUtil::BroadcastSender(this)),
      ip(getHostIp()), serverThread(new QThread(nullptr)) {
  connect(broadcastSender, &BroadcastUtil::BroadcastSender::hasError, this,
          &Server::logBroadcastSenderError);
  connect(broadcastSender, &BroadcastUtil::BroadcastSender::sent, this,
          &Server::logBroadcastSent);
}

Server::~Server() {
  serverThread->quit();
  serverThread->wait();
}

void Server::setPort(quint16 port) {
  this->port = port;
  broadcastSender->setPort(port);
}

bool Server::start() {
  assert(logger);

  logger->info("Starting server...");

  if (!port) {
    logger->error("No port set up. Impossible to start server");
    return false;
  }

  if (ip.isNull()) {
    logger->error("Error while initializing ip");
    return false;
  }

  auto* server = new PingServer(logger, ip, port.value());

  server->moveToThread(serverThread);

  connect(server, &PingServer::hasPing, broadcastSender,
          &BroadcastUtil::BroadcastSender::stop, Qt::QueuedConnection);
  connect(server, &PingServer::pingTimeout, broadcastSender,
          &BroadcastUtil::BroadcastSender::start, Qt::QueuedConnection);

  connect(serverThread, &QThread::started, server, &PingServer::start);

  connect(serverThread, &QThread::finished, serverThread, &QThread::deleteLater);
  connect(serverThread, &QThread::finished, server, &QObject::deleteLater);

  broadcastSender->setPort(port.value());
  broadcastSender->setIp(ip);

  broadcastSender->start();
  serverThread->start();

  return true;
}

void Server::logBroadcastSenderError(BroadcastUtil::Error error) {
  switch (error.type) {
    case BroadcastUtil::Error::Type::NoPort: {
      logger->error("No port error occured. Description: {}",
                    error.description);
    } break;
    case BroadcastUtil::Error::Type::NoIpAddress: {
      logger->error("No ip address error occured. Description: {}",
                    error.description);
    } break;
    case BroadcastUtil::Error::Type::Unknown: {
      logger->error("Unknown error occured; Description: {}",
                    error.description);
    } break;
    default: {
      assert(false && "Not all error type handled");
    }
  }
}

void Server::logBroadcastSent() {
  logger->trace("Broadcast message sent");
}

}  // namespace Server
