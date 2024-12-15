#pragma once

#include <string_view>
#include <QMetaType>

namespace BroadcastUtil {

struct Error final {
  static Error createNoPortError();
  static Error createNoIpAddressError();
  static Error createUnknownError();

  enum class Type {
    NoPort = 0,
    NoIpAddress,
    Unknown
  };

  std::string_view description;
  Type type;
};

}  // namespace BroadcastUtil

Q_DECLARE_METATYPE(BroadcastUtil::Error)
