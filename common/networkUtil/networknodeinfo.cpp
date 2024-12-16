#include "networknodeinfo.h"

uint64_t std::hash<NetworkUtil::NetworkNodeInfo>::operator()(
    const NetworkUtil::NetworkNodeInfo& serverInfo) const {
  uint64_t seed = 0;
  // see https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
  seed ^= ::qHash(serverInfo.ip, seed) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  seed ^= ::qHash(serverInfo.port, seed) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}

bool NetworkUtil::NetworkNodeInfo::operator==(const NetworkNodeInfo& other) const {
  return ip == other.ip && port == other.port;
}
