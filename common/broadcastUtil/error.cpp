#include "error.h"

namespace BroadcastUtil {

namespace {
namespace ErrorDescription {
constinit const std::string_view noPort{"No port set up"};
constinit const std::string_view noIpAddress{"No address set up"};
constinit const std::string_view unknown{"Unknown error happened"};
}  // namespace ErrorDescription
}  // namespace

Error Error::createNoPortError() {
  return Error{ErrorDescription::noPort, Type::NoPort};
}

Error Error::createNoIpAddressError() {
  return Error{ErrorDescription::noIpAddress, Type::NoIpAddress};
}

Error Error::createUnknownError() {
  return Error{ErrorDescription::unknown, Type::Unknown};
}

}  // namespace BroadcastUtil
