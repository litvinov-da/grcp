#include "broadcasttableitem.h"

namespace Client {

QString broadcastStatusToString(BroadcastTableItem::Status status) {
  switch (status) {

    case BroadcastTableItem::Status::Offline:
      return "Offline";
    case BroadcastTableItem::Status::Online:
      return "Online";
  }

  assert(false && "Not all types handled");
  return {};
}

BroadcastTableItem::BroadcastTableItem(const QHostAddress& ip, quint16 port)
    : ipInfo(IpInfo{ip, port}), lastPingTime{}, status(Status::Offline) {}

}  // namespace Client
