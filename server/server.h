#pragma once

#include <spdlog/spdlog.h>

#include <QHostInfo>
#include <QObject>

#include <error.h>

namespace BroadcastUtil {
class BroadcastSender;
}
namespace Server {
class PingServer;

class Server final : public QObject {
  Q_OBJECT

 public:
  Server(const std::shared_ptr<spdlog::logger>& logger,
         QObject* parent = nullptr);
  ~Server() override;

  void setPort(quint16 port);

  bool start();

 private:
  void logBroadcastSenderError(BroadcastUtil::Error error);
  void logBroadcastSent();

 private:
  std::shared_ptr<spdlog::logger> logger{nullptr};
  BroadcastUtil::BroadcastSender* broadcastSender{nullptr};
  QHostAddress ip;
  std::optional<quint16> port {std::nullopt};

  QThread* serverThread{nullptr};
};
}  // namespace Server
