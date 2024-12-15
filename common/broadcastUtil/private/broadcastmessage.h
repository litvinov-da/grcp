#pragma once

#include <qtypes.h>
#include <QHostAddress>

class QNetworkDatagram;

namespace BroadcastUtil {
namespace Private {

class BroadcastMessage final {
 public:
  static BroadcastMessage create(const QByteArray& data);

  BroadcastMessage(quint16 serverPort, QHostAddress serverIp);

  QHostAddress getServerIp() const;
  quint16 getServerPort() const;

  QByteArray getData() const;

  friend bool operator==(const BroadcastMessage& lhs, const BroadcastMessage& rhs);

 private:
  BroadcastMessage() = default;

  QHostAddress& getServerIp();
  quint16& getServerPort();

 private:
  quint16 serverPort;
  QHostAddress serverIp;
};

size_t qHash(const BroadcastMessage& key, size_t seed = 0) noexcept;

}  // namespace Private
}  // namespace BroadcastUtil
