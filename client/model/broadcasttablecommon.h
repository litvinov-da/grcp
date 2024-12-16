#pragma once

#include <Qt>
#include <cstdint>

namespace Client {

enum Columns {
  Ip_Column = 0,
  LastPingTime_Column,
  Status_Column,
  AvailableAction_Column,
  SizeInternal_Column
};

enum class AvailableAction : std::uint8_t { Connect = 0, Disconnect };

enum ExtendedRoles { CurrentStatus_Role = Qt::UserRole + 1, AvailableAction_Role };
}  // namespace Client
