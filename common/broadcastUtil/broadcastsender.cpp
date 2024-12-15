#include "broadcastsender.h"

#include <QTimer>
#include <QUdpSocket>

#include <QNetworkDatagram>
#include "private/broadcastcommon.h"
#include "private/broadcastmessage.h"
namespace BroadcastUtil {

BroadcastSender::BroadcastSender(QObject* parent)
    : QObject(parent), timer(new QTimer(this)), socket(new QUdpSocket(this)) {
  timer->setInterval(Private::broadcastTimeout);
  connect(timer, &QTimer::timeout, this, &BroadcastSender::sendDatagram);
}

void BroadcastSender::sendDatagram() {
  if (ip.isNull()) {
    emit hasError(Error::createNoIpAddressError(), QPrivateSignal{});
    return;
  }

  if (!port) {
    emit hasError(Error::createNoPortError(), QPrivateSignal{});
    return;
  }

  const auto messageData =
      Private::BroadcastMessage{port.value(), ip}.getData();
  if (socket->writeDatagram(messageData, ip, Private::port) == -1) {
    emit hasError(Error::createUnknownError(), QPrivateSignal{});
    return;
  }

  emit sent(QPrivateSignal{});
}

void BroadcastSender::setIp(const QHostAddress& newIp) {
  ip = newIp;
}

void BroadcastSender::stop() {
  timer->stop();
}

void BroadcastSender::start() {
  timer->start();
}

void BroadcastSender::setPort(quint16 newPort) {
  port = newPort;
}

}  // namespace BroadcastUtil
