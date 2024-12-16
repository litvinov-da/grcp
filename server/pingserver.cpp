#include "pingserver.h"

#include "pingservice.h"

#include <grpcpp/grpcpp.h>
#include <QNetworkInterface>
namespace Server {

PingServer::PingServer(const std::shared_ptr<spdlog::logger>& logger, const QHostAddress& ip, quint16 port)
    : logger(logger), ip(ip), port(port) {}

void PingServer::start() {
  auto* pingService = new PingService(logger, this);
  connect(pingService, &PingService::hasPing, this,
          &PingServer::hasPing);
  connect(pingService, &PingService::pingTimeout, this,
          &PingServer::pingTimeout);

  grpc::ServerBuilder builder;

  assert(!ip.isNull());

  builder.AddListeningPort(
      ip.toString().toStdString() + ":" + std::to_string(port),
      grpc::InsecureServerCredentials());
  builder.RegisterService(pingService);

  auto server{builder.BuildAndStart()};
  server->Wait();
}


}  // namespace Server
