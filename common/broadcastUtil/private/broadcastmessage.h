#pragma once

#include <qtypes.h>
#include <QHostAddress>

#include <networknodeinfo.h>

class QNetworkDatagram;

namespace BroadcastUtil {
namespace Private {

class BroadcastMessage final {
  friend class std::hash<BroadcastMessage>;

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
  NetworkUtil::NetworkNodeInfo serverInfo;
};

}  // namespace Private
}  // namespace BroadcastUtil

namespace std {
template <>
struct hash<BroadcastUtil::Private::BroadcastMessage> {
  std::uint64_t operator()(
      const BroadcastUtil::Private::BroadcastMessage& message) const;
};
}
