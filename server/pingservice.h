#pragma once

#include "api.grpc.pb.h"
#include "api.pb.h"

#include <QObject>
#include <spdlog/spdlog.h>

class PingService final : public QObject, public MaintainingApi::Service {
  Q_OBJECT
 public:
  explicit PingService(
      const std::shared_ptr<spdlog::logger>& logger, QObject* parent = nullptr);

  ::grpc::Status Ping(::grpc::ServerContext* context,
                      const ::PingRequest* request,
                      ::PingResponse* response) override;

 signals:
  void hasPing();
  void pingTimeout();

  void startTimerPrivate();
  void hasPingPrivate();

 private:
  QTimer* timeoutTimer{nullptr};
  std::shared_ptr<spdlog::logger> logger{nullptr};
};
