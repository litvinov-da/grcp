#pragma once
#include <string_view>

#include <QHostAddress>
#include <QtCore>

class QByteArray;

class BroadcastMessage {
 public:
  static std::optional<BroadcastMessage> create(const char* data, int size);

  BroadcastMessage(const QHostAddress& ip, quint16 port);

  QByteArray getBinaryMessage() const;

  QHostAddress getIpAdress() const;
  quint16 getPort() const;

 private:
  QHostAddress ip;
  quint16 port;
};