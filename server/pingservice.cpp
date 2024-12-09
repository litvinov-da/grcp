#include "pingservice.h"

#include <QTimer>

#include <chrono>

namespace {
constexpr const auto timeout =
    std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::seconds(15));
}

PingService::PingService(const std::shared_ptr<spdlog::logger>& logger,
                         QObject* parent)
    : QObject(parent), timeoutTimer(new QTimer(this)), logger(logger) {
  connect(timeoutTimer, &QTimer::timeout, this, &PingService::pingTimeout);
}

::grpc::Status PingService::Ping(::grpc::ServerContext* context,
                                 const ::PingRequest* request,
                                 ::PingResponse* response) {
  emit hasPing();
  logger->info("Has ping request from {}", request->clientip());
  timeoutTimer->start(timeout);
  return ::grpc::Status::OK;
}
