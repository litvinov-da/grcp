#pragma once

#include <spdlog/spdlog.h>

#include <QObject>

class QUdpSocket;

namespace Server {
class Server : public QObject {
  Q_OBJECT

 public:
  Server(const std::shared_ptr<spdlog::logger>& logger,
         QObject* parent = nullptr);

  void setPort(quint16 port);

  bool start();

 private:
  QUdpSocket* broadcastSocket{nullptr};

  std::shared_ptr<spdlog::logger> logger{nullptr};
  std::optional<quint16> port {std::nullopt};
};
}  // namespace Server