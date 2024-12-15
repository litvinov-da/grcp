#pragma once

#include <spdlog/spdlog.h>

#include <QHostInfo>
#include <QObject>

#include <error.h>

class QUdpSocket;

namespace BroadcastUtil {
class BroadcastSender;
}
namespace Server {
class Server final : public QObject {
  Q_OBJECT

 public:
  Server(const std::shared_ptr<spdlog::logger>& logger,
         QObject* parent = nullptr);

  void setPort(quint16 port);

  bool start();

 private:
  void logBroadcastSenderError(BroadcastUtil::Error error);
  void logBroadcastSent();

 private:
  std::shared_ptr<spdlog::logger> logger{nullptr};

  std::optional<quint16> port {std::nullopt};
  QHostAddress ip;

  BroadcastUtil::BroadcastSender* broadcastSender {nullptr};
};
}  // namespace Server
