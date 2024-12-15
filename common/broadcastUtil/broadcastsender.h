#pragma once

#include "error.h"

#include <QHostAddress>
#include <QObject>

class QUdpSocket;
namespace BroadcastUtil {

class BroadcastSender final : public QObject {
  Q_OBJECT

 public:
  explicit BroadcastSender(QObject* parent = nullptr);

  void setPort(quint16 newPort);
  void setIp(const QHostAddress& newIp);

  void stop();
  void start();

 signals:
  void sent(QPrivateSignal);
  void hasError(Error error, QPrivateSignal);

 private:
  void sendDatagram();

 private:
  QTimer* timer{nullptr};
  QUdpSocket* socket{nullptr};

  std::optional<quint16> port;
  QHostAddress ip;
};

}  // namespace BroadcastUtil
