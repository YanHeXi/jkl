#include "yhx/iomanager.h"
#include "yhx/log.h"
#include "yhx/tcp_server.h"

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

void run() {
  auto addr = yhx::Address::LookupAny("0.0.0.0:8033");
  // auto addr2 = yhx::UnixAddress::ptr(new yhx::UnixAddress("/tmp/unix_addr"));
  std::vector<yhx::Address::ptr> addrs;
  addrs.push_back(addr);
  // addrs.push_back(addr2);

  yhx::TcpServer::ptr tcp_server(new yhx::TcpServer);
  std::vector<yhx::Address::ptr> fails;
  while (!tcp_server->bind(addrs, fails)) {
    sleep(2);
  }
  tcp_server->start();
}
int main(int argc, char** argv) {
  yhx::IOManager iom(2);
  iom.schedule(run);
  return 0;
}
