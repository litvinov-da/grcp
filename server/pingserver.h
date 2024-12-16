#pragma once

#include <error.h>
#include <QObject>

#include <spdlog/spdlog.h>
#include <QHostAddress>

namespace Server {

class PingServer : public QObject {
  Q_OBJECT
 public:
  PingServer(const std::shared_ptr<spdlog::logger>& logger, const QHostAddress& ip, quint16 port);
  PingServer(QObject* parent) = delete;

  void start();

 signals:
  void hasPing();
  void pingTimeout();

 private:
  std::shared_ptr<spdlog::logger> logger{nullptr};
  QHostAddress ip;
  quint16 port;
};

}  // namespace Server
