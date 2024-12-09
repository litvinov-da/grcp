#include "broadcastmessage.h"

namespace {

constexpr std::string_view uniqueBroadcastId{"uniqueBroadcastId"};

const QString messageTemplate{"%1\t%2\t%3"};

}  // namespace

std::optional<BroadcastMessage> BroadcastMessage::create(const char* data,
                                                         int size) {
  const auto binaryMessage = QString{QByteArray::fromRawData(data, size)};
  const auto splitedMessage = binaryMessage.split("\t");
  if (splitedMessage.isEmpty()) {
    return std::nullopt;
  }

  if (splitedMessage.size() != 3) {
    return std::nullopt;
  }

  if (splitedMessage.front() != uniqueBroadcastId) {
    return std::nullopt;
  }

  // todo: add another validation

  return BroadcastMessage(QHostAddress{splitedMessage[1]},
                          splitedMessage[2].toUInt());
}

BroadcastMessage::BroadcastMessage(const QHostAddress& ip, quint16 port)
    : ip(ip), port(port) {}

QByteArray BroadcastMessage::getBinaryMessage() const {
  return messageTemplate
      .arg(uniqueBroadcastId.data(), ip.toString(), QString::number(port))
      .toUtf8();
}

QHostAddress BroadcastMessage::getIpAdress() const {
  return ip;
}

quint16 BroadcastMessage::getPort() const {
  return port;
}
