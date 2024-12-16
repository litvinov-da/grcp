#pragma once

#include <QHostAddress>

namespace NetworkUtil {

struct NetworkNodeInfo {
  QHostAddress ip;
  quint16 port;

  bool operator==(const NetworkNodeInfo& other) const;
};

}  // namespace NetworkUtil

namespace std {
template <>
struct hash<NetworkUtil::NetworkNodeInfo> {
  std::uint64_t operator()(const NetworkUtil::NetworkNodeInfo& serverInfo) const;
};
}
