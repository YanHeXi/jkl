#include "yhx/http/http_server.h"
#include "yhx/log.h"

static yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

#define XX(...) #__VA_ARGS__

yhx::IOManager::ptr worker;
void run() {
  g_logger->setLevel(yhx::LogLevel::Level::INFO);
  // yhx::http::HttpServer::ptr server(
  // new yhx::http::HttpServer(true, worker.get(), yhx::IOManager::GetThis()));
  yhx::http::HttpServer::ptr server(new yhx::http::HttpServer);
  yhx::Address::ptr addr = yhx::Address::LookupAnyIPAddress("0.0.0.0:8020");
  while (!server->bind(addr)) {
    sleep(2);
  }
  auto sd = server->getServletDispatch();
  sd->addServlet("/yhx/xx", [](yhx::http::HttpRequest::ptr req,
                               yhx::http::HttpResponse::ptr rsp,
                               yhx::http::HttpSession::ptr session) {
    rsp->setBody(req->toString());
    return 0;
  });

  sd->addGlobServlet("/yhx/*", [](yhx::http::HttpRequest::ptr req,
                                  yhx::http::HttpResponse::ptr rsp,
                                  yhx::http::HttpSession::ptr session) {
    rsp->setBody("Glob:\r\n" + req->toString());
    return 0;
  });

  // sd->addGlobServlet("/yhxx/*", [](yhx::http::HttpRequest::ptr req,
  //                                  yhx::http::HttpResponse::ptr rsp,
  //                                  yhx::http::HttpSession::ptr session) {
  //   rsp->setBody(
  //       XX(<html><head> < title > 404 Not Found</ title></
  //       head><body><center>
  //       <
  //              h1 > 404 Not Found</ h1></ center><hr><center> nginx / 1.16.0
  //              < / center > </ body></ html> < !--a padding to disable MSIE
  //              and
  //          Chrome friendly error page-- > < !--a padding to disable MSIE and
  //          Chrome friendly error page-- > < !--a padding to disable MSIE and
  //          Chrome friendly error page-- > < !--a padding to disable MSIE and
  //          Chrome friendly error page-- > < !--a padding to disable MSIE and
  //          Chrome friendly error page-- > < !--a padding to disable MSIE and
  //          Chrome friendly error page-- >));
  //   return 0;
  // });

  server->start();
}

int main(int argc, char** argv) {
  yhx::IOManager iom(2);
  // worker.reset(new yhx::IOManager(3, false, "worker"));
  iom.schedule(run);
  return 0;
}
