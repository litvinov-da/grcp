#pragma once

#include <networknodeinfo.h>
#include <QDateTime>
#include <QHostAddress>
#include <QMetaType>

namespace Client {

struct BroadcastTableItem {
  enum class Status : std::uint8_t { Offline = 0, Online };

  NetworkUtil::NetworkNodeInfo ipInfo;
  QDateTime lastPingTime;
  Status status;

  BroadcastTableItem() = default;
  BroadcastTableItem(const QHostAddress& ip, quint16 port);
};

QString broadcastStatusToString(BroadcastTableItem::Status status);

}  // namespace Client

Q_DECLARE_METATYPE(Client::BroadcastTableItem)
