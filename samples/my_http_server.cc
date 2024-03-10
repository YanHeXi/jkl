#include "yhx/http/http_server.h"
#include "yhx/log.h"

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();
yhx::IOManager::ptr worker;
void run() {
  g_logger->setLevel(yhx::LogLevel::Level::INFO);
  yhx::Address::ptr addr = yhx::Address::LookupAnyIPAddress("0.0.0.0:8020");
  if (!addr) {
    YHX_LOG_ERROR(g_logger) << "get address error";
    return;
  }

  //   yhx::http::HttpServer::ptr http_server(
  //   new yhx::http::HttpServer(true, worker.get()));
  yhx::http::HttpServer::ptr http_server(new yhx::http::HttpServer(true));
  bool ssl = false;
  while (!http_server->bind(addr, ssl)) {
    YHX_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
    sleep(1);
  }

  if (ssl) {
    // http_server->loadCertificates("/home/apps/soft/yhx/keys/server.crt",
    // "/home/apps/soft/yhx/keys/server.key");
  }

  http_server->start();
}

int main(int argc, char** argv) {
  yhx::IOManager iom(1);
  //   worker.reset(new yhx::IOManager(4, false));
  iom.schedule(run);
  return 0;
}
