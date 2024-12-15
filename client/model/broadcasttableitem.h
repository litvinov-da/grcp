#pragma once

#include <QDateTime>
#include <QHostAddress>
#include <QMetaType>

namespace Client {

struct BroadcastTableItem {
  struct IpInfo {
    QHostAddress ipAdress;
    quint16 port;
  };

  enum class Status : std::uint8_t { Offline = 0, Online };

  IpInfo ipInfo;
  QDateTime lastPingTime;
  Status status;

  BroadcastTableItem() = default;
  BroadcastTableItem(const QHostAddress& ip, quint16 port);
};

QString broadcastStatusToString(BroadcastTableItem::Status status);

}  // namespace Client

Q_DECLARE_METATYPE(Client::BroadcastTableItem)
Q_DECLARE_METATYPE(Client::BroadcastTableItem::IpInfo)
