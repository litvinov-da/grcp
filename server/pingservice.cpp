#include "pingservice.h"

#include <QTimer>

#include <chrono>

namespace {
constinit const auto timeout =
    std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::seconds(15));
}

PingService::PingService(const std::shared_ptr<spdlog::logger>& logger,
                         QObject* parent)
    : QObject(parent), timeoutTimer(new QTimer(this)), logger(logger) {
  timeoutTimer->setInterval(timeout);

  connect(timeoutTimer, &QTimer::timeout, this, &PingService::pingTimeout);
  connect(
      this, &PingService::startTimerPrivate, this, [this]() {
        assert(timeoutTimer);
        timeoutTimer->start();
      }
      );
  connect(this, &PingService::hasPingPrivate, this,
          &PingService::hasPing);
  logger->info("Ping service created");
}


::grpc::Status PingService::Ping(::grpc::ServerContext* context,
                                 const ::PingRequest* request,
                                 ::PingResponse* response) {
  emit hasPingPrivate();
  logger->info("Has ping request from {}", request->clientip());

  emit startTimerPrivate();
  return ::grpc::Status::OK;
}
