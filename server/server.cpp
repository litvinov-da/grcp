#include "server.h"
#include <QUdpSocket>

#include <QHostInfo>
#include <QNetworkInterface>
#include "broadcastsender.h"
#include "pingservice.h"

#include <grpcpp/grpcpp.h>

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
      ip(getHostIp()),
      broadcastSender(new BroadcastUtil::BroadcastSender(this)) {
  broadcastSender->setIp(ip);

  connect(broadcastSender, &BroadcastUtil::BroadcastSender::hasError, this,
          &Server::logBroadcastSenderError);
  connect(broadcastSender, &BroadcastUtil::BroadcastSender::sent, this,
          &Server::logBroadcastSent);
}

void Server::setPort(quint16 port) {
  this->port = port;
  broadcastSender->setPort(port);
}

bool Server::start() {
  assert(logger);

  logger->info("Starting server...");

  auto* pingService = new PingService(logger, this);
  connect(pingService, &PingService::hasPing, broadcastSender,
          &BroadcastUtil::BroadcastSender::stop, Qt::QueuedConnection);
  connect(pingService, &PingService::pingTimeout, broadcastSender,
          &BroadcastUtil::BroadcastSender::start, Qt::QueuedConnection);

  std::thread grpcServerThread{[pingService, this]() {
    grpc::ServerBuilder builder;

    assert(!ip.isNull());
    assert(port);

    builder.AddListeningPort(
        ip.toString().toStdString() + ":" + std::to_string(port.value()),
        grpc::InsecureServerCredentials());
    builder.RegisterService(pingService);

    auto server{builder.BuildAndStart()};
    server->Wait();
  }};
  grpcServerThread.detach();

  broadcastSender->start();

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
