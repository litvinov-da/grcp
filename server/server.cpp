#include "server.h"
#include <QUdpSocket>

const QByteArray broadcastDatagram{};

namespace Server {
Server::Server(const std::shared_ptr<spdlog::logger>& logger, QObject* parent)
    : QObject{parent}, logger(logger), broadcastSocket(new QUdpSocket(this)) {}

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

  broadcastSocket->writeDatagram(broadcastDatagram, QHostAddress::Broadcast,
                                 10001);

  return true;
}
}  // namespace Server