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
    : serverInfo(NetworkUtil::NetworkNodeInfo{std::move(serverIp), serverPort}) {}

QHostAddress BroadcastMessage::getServerIp() const {
  return serverInfo.ip;
}

quint16 BroadcastMessage::getServerPort() const {
  return serverInfo.port;
}

QByteArray BroadcastMessage::getData() const {
  QByteArray data;
  QDataStream stream{&data, QIODeviceBase::WriteOnly};

  stream << serverInfo.ip << serverInfo.port;

  return data;
}

QHostAddress& BroadcastMessage::getServerIp() {
  return serverInfo.ip;
}

quint16& BroadcastMessage::getServerPort() {
  return serverInfo.port;
}

bool operator==(const BroadcastMessage& lhs, const BroadcastMessage& rhs) {
  return lhs.serverInfo == rhs.serverInfo;
}


}  // namespace Private
}  // namespace BroadcastUtil

uint64_t std::hash<BroadcastUtil::Private::BroadcastMessage>::operator()(
    const BroadcastUtil::Private::BroadcastMessage& message) const {
  return std::hash<NetworkUtil::NetworkNodeInfo>{}(message.serverInfo);
}
