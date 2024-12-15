#include "broadcastmessage.h"

#include <QNetworkDatagram>

namespace BroadcastUtil {
namespace Private {

namespace {}
BroadcastMessage BroadcastMessage::create(const QByteArray& data) {
  BroadcastMessage message{};
  QDataStream stream{data};

  stream >> message.getServerIp() >> message.getServerPort();

  return message;
}

BroadcastMessage::BroadcastMessage(quint16 serverPort, QHostAddress serverIp)
    : serverPort(serverPort), serverIp(std::move(serverIp)) {}

QHostAddress BroadcastMessage::getServerIp() const {
  return serverIp;
}

quint16 BroadcastMessage::getServerPort() const {
  return serverPort;
}

QByteArray BroadcastMessage::getData() const {
  QByteArray data;
  QDataStream stream{&data, QIODeviceBase::WriteOnly};

  stream << serverIp << serverPort;

  return data;
}

QHostAddress& BroadcastMessage::getServerIp() {
  return serverIp;
}

quint16& BroadcastMessage::getServerPort() {
  return serverPort;
}

bool operator==(const BroadcastMessage& lhs, const BroadcastMessage& rhs) {
  return lhs.serverIp == rhs.serverIp && lhs.serverPort == rhs.serverPort;
}


size_t qHash(const BroadcastMessage& key, size_t seed) noexcept {
  // see https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
  seed ^= ::qHash(key.getServerIp(), seed) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  seed ^= ::qHash(key.getServerPort(), seed) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}

}  // namespace Private
}  // namespace BroadcastUtil
