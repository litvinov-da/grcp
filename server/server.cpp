#include "server.h"
#include <QUdpSocket>

#include <QHostInfo>
#include <QNetworkInterface>
#include "broadcastmessage.h"
#include "pingservice.h"

#include <grpcpp/grpcpp.h>

namespace Server {

namespace {
constexpr const int broadcastPort = 10001;
constexpr const int broadcastTimeout = 1000;

QHostAddress getHostIp() {
  const auto localhost = QHostAddress(QHostAddress::LocalHost);
  for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != localhost) {
      return address;
    }
  }
}
}  // namespace

Server::Server(const std::shared_ptr<spdlog::logger>& logger, QObject* parent)
    : QObject{parent},
      logger(logger),
      broadcastSocket(new QUdpSocket(this)),
      ip(getHostIp()),
      broadcastTimer(new QTimer(this)) {
  broadcastTimer->setInterval(broadcastTimeout);
  connect(broadcastTimer, &QTimer::timeout, this,
          &Server::sendBroadcastDatagram);
}

void Server::setPort(quint16 port) {
  this->port = port;
}

bool Server::start() {
  assert(logger);

  logger->info("Starting server...");

  if (!port) {
    logger->error("No port initialized. No broadcasting started");
    logger->info("Init port before starting server");

    return false;
  }

  if (ip.isNull()) {
    logger->error("Something went wrong when initializing ip address");
    return false;
  }

  auto* pingService = new PingService(logger, this);
  connect(pingService, &PingService::hasPing, broadcastTimer, &QTimer::stop,
          Qt::QueuedConnection);
  connect(
      pingService, &PingService::pingTimeout, this,
      [this]() { broadcastTimer->start(); }, Qt::QueuedConnection);

  std::thread grpcServerThread{[pingService, this]() {
    grpc::ServerBuilder builder;

    assert(!ip.isNull());
    assert(port);

    builder.AddListeningPort(
        ip.toString().toStdString() + std::to_string(port.value()),
        grpc::InsecureServerCredentials());
    builder.RegisterService(pingService);

    auto server{builder.BuildAndStart()};
    server->Wait();
  }};

  broadcastTimer->start();

  return true;
}
void Server::sendBroadcastDatagram() {
  assert(!ip.isNull());
  assert(port);

  logger->trace("Send broadcast");
  broadcastSocket->writeDatagram(
      BroadcastMessage{ip, port.value()}.getBinaryMessage(),
      QHostAddress::Broadcast, broadcastPort);
}
}  // namespace Server