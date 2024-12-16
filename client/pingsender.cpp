#include "pingsender.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <QNetworkInterface>
#include <QTimer>

#include "api.pb.h"

namespace Client {

namespace {
// TODO(d.litvinov) remove duplicates with server.cpp
QHostAddress getHostIp() {
  const auto localhost = QHostAddress(QHostAddress::LocalHost);
  for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != localhost) {
      return address;
    }
  }

  return {};
}

std::shared_ptr<grpc::ChannelInterface> createChannel(const NetworkUtil::NetworkNodeInfo& serverInfo) {
  return grpc::CreateChannel(
      serverInfo.ip.toString().toStdString() + ":" + std::to_string(serverInfo.port),
      grpc::InsecureChannelCredentials());
}

constinit const auto pingTime =
    std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::seconds(5));

constinit const uint disconnectCount = 3;

}  // namespace

PingSender::PingSender(const NetworkUtil::NetworkNodeInfo& serverInfo,
                       QObject* parent)
    : QObject(parent),
      stub(MaintainingApi::NewStub(createChannel(serverInfo))),
      pingTimer(new QTimer(this)),
      clientIp(getHostIp()),
      serverInfo(serverInfo) {
  pingTimer->setInterval(pingTime);
  connect(pingTimer, &QTimer::timeout, this, &PingSender::ping);
}

void PingSender::start() {
  ping();
  pingTimer->start();
}

void PingSender::stop() {
  pingTimer->stop();
}

void PingSender::ping() {
  grpc::ClientContext context;
  PingRequest request;
  request.mutable_clientip()->assign(clientIp.toString().toStdString());
  PingResponse response;
  const auto status = stub->Ping(&context, request, &response);

  if (status.ok()) {
    emit hasPing(serverInfo);
    return;
  }

  ++noPingCount;

  if (noPingCount == disconnectCount) {
    emit disconnect(serverInfo);
  }
}

}  // namespace Client
