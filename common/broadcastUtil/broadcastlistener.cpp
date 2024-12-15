#include "broadcastlistener.h"

#include <QNetworkDatagram>
#include <QUdpSocket>

#include "private/broadcastcommon.h"
#include "private/broadcastmessage.h"

namespace BroadcastUtil {

BroadcastListener::BroadcastListener(QObject* parent)
    : socket(new QUdpSocket(this)) {
  socket->bind(Private::port, QAbstractSocket::DontShareAddress);

  connect(socket, &QUdpSocket::readyRead, this,
          &BroadcastListener::readDatagram);
}

void BroadcastListener::readDatagram() {
  while (socket->hasPendingDatagrams()) {
    const auto datagram = socket->receiveDatagram();
    const auto message = Private::BroadcastMessage::create(datagram.data());
    if (!detected.contains(message)) {
      detected.insert(message);
      emit newBroadcastServerDetected(message.getServerIp(),
                                      message.getServerPort());
    }
  }
}

}  // namespace BroadcastUtil
