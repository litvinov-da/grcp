#pragma once

#include <qtypes.h>

#include <QHostAddress>
#include "api.grpc.pb.h"

#include <networknodeinfo.h>

namespace Client {

class PingSender final : public QObject{
  Q_OBJECT

 public:
  PingSender(const NetworkUtil::NetworkNodeInfo& serverInfo, QObject* parent = nullptr);

  void start();
  void stop();

 signals:
  void disconnect(const NetworkUtil::NetworkNodeInfo& serverInfo);
  void hasPing(const NetworkUtil::NetworkNodeInfo& serverInfo);

 private:
  void ping();

 private:
  std::unique_ptr<MaintainingApi::Stub> stub;
  QTimer* pingTimer{nullptr};

  uint noPingCount{0};

  const QHostAddress clientIp;
  const NetworkUtil::NetworkNodeInfo serverInfo;
};

}  // namespace Client
