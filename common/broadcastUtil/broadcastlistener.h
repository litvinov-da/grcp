#pragma once

#include <QHostAddress>
#include <QObject>

#include "private/broadcastmessage.h"

class QUdpSocket;

namespace BroadcastUtil {

class BroadcastListener final : public QObject {
  Q_OBJECT

 public:
  explicit BroadcastListener(QObject* parent = nullptr);

 signals:
  void newBroadcastServerDetected(const QHostAddress& ip, quint16 port);

 private:
  void readDatagram();

 private:
  QUdpSocket* socket{nullptr};

  QSet<Private::BroadcastMessage> detected{};
};

}  // namespace BroadcastUtil
