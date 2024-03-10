#include <fstream>
#include <iostream>

#include "yhx/http/http_connection.h"
#include "yhx/http/http_parser.h"
#include "yhx/iomanager.h"
#include "yhx/log.h"
#include "yhx/streams/zlib_stream.h"

static yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

// void test_pool() {
//   yhx::http::HttpConnectionPool::ptr pool(new yhx::http::HttpConnectionPool(
//       "www.yhx.top", "", 80, false, 10, 1000 * 30, 5));

//   yhx::IOManager::GetThis()->addTimer(
//       1000,
//       [pool]() {
//         auto r = pool->doGet("/", 300);
//         YHX_LOG_INFO(g_logger) << r->toString();
//       },
//       true);
// }

void run() {
  yhx::Address::ptr addr = yhx::Address::LookupAnyIPAddress("www.baidu.com:80");
  if (!addr) {
    YHX_LOG_INFO(g_logger) << "get addr error";
    return;
  }

  yhx::Socket::ptr sock = yhx::Socket::CreateTCP(addr);
  bool rt = sock->connect(addr);
  if (!rt) {
    YHX_LOG_INFO(g_logger) << "connect " << *addr << " failed";
    return;
  }

  yhx::http::HttpConnection::ptr conn(new yhx::http::HttpConnection(sock));
  yhx::http::HttpRequest::ptr req(new yhx::http::HttpRequest);
  // req->setPath("/blog/");
  req->setHeader("host", "www.baidu.com");
  YHX_LOG_INFO(g_logger) << "req:" << std::endl << *req;

  conn->sendRequest(req);
  auto rsp = conn->recvResponse();

  if (!rsp) {
    YHX_LOG_INFO(g_logger) << "recv response error";
    return;
  }
  YHX_LOG_INFO(g_logger) << "rsp:" << std::endl << *rsp;

  std::ofstream ofs("rsp.dat");
  ofs << *rsp;

  YHX_LOG_INFO(g_logger) << "=========================";

  auto r = yhx::http::HttpConnection::DoGet("http://www.baidu.com/", 300);
  YHX_LOG_INFO(g_logger) << "result=" << r->result << " error=" << r->error
                         << " rsp="
                         << (r->response ? r->response->toString() : "");

  YHX_LOG_INFO(g_logger) << "=========================";
  //   test_pool();
}

// void test_https() {
//   auto r = yhx::http::HttpConnection::DoGet(
//       "http://www.baidu.com/", 300,
//       {{"Accept-Encoding", "gzip, deflate, br"},
//        {"Connection", "keep-alive"},
//        {"User-Agent", "curl/7.29.0"}});
//   YHX_LOG_INFO(g_logger) << "result=" << r->result << " error=" << r->error
//                          << " rsp="
//                          << (r->response ? r->response->toString() : "");

//   yhx::http::HttpConnectionPool::ptr pool(new yhx::http::HttpConnectionPool(
//               "www.baidu.com", "", 80, false, 10, 1000 * 30, 5));
//   auto pool = yhx::http::HttpConnectionPool::Create("https://www.baidu.com",
//   "",
//                                                     10, 1000 * 30, 5);
//   yhx::IOManager::GetThis()->addTimer(
//       1000,
//       [pool]() {
//         auto r = pool->doGet("/", 3000,
//                              {{"Accept-Encoding", "gzip, deflate, br"},
//                               {"User-Agent", "curl/7.29.0"}});
//         YHX_LOG_INFO(g_logger) << r->toString();
//       },
//       true);
// }

void test_data() {
  yhx::Address::ptr addr = yhx::Address::LookupAny("www.baidu.com:80");
  auto sock = yhx::Socket::CreateTCP(addr);

  sock->connect(addr);
  const char buff[] =
      "GET / HTTP/1.1\r\n"
      "connection: close\r\n"
      "Accept-Encoding: gzip, deflate, br\r\n"
      "Host: www.baidu.com\r\n\r\n";
  sock->send(buff, sizeof(buff));

  std::string line;
  line.resize(1024);

  std::ofstream ofs("http.dat", std::ios::binary);
  int total = 0;
  int len = 0;
  while ((len = sock->recv(&line[0], line.size())) > 0) {
    total += len;
    ofs.write(line.c_str(), len);
  }
  std::cout << "total: " << total << " tellp=" << ofs.tellp() << std::endl;
  ofs.flush();
}

void test_parser() {
  std::ifstream ifs("http.dat", std::ios::binary);
  std::string content;
  std::string line;
  line.resize(1024);

  int total = 0;
  while (!ifs.eof()) {
    ifs.read(&line[0], line.size());
    content.append(&line[0], ifs.gcount());
    total += ifs.gcount();
  }

  std::cout << "length: " << content.size() << " total: " << total << std::endl;
  yhx::http::HttpResponseParser parser;
  size_t nparse = parser.execute(&content[0], content.size(), false);
  std::cout << "finish: " << parser.isFinished() << std::endl;
  content.resize(content.size() - nparse);
  std::cout << "rsp: " << *parser.getData() << std::endl;

  auto& client_parser = parser.getParser();
  std::string body;
  int cl = 0;
  do {
    size_t nparse = parser.execute(&content[0], content.size(), true);
    std::cout << "content_len: " << client_parser.content_len
              << " left: " << content.size() << std::endl;
    cl += client_parser.content_len;
    content.resize(content.size() - nparse);
    body.append(content.c_str(), client_parser.content_len);
    content = content.substr(client_parser.content_len + 2);
  } while (!client_parser.chunks_done);

  std::cout << "total: " << body.size() << " content:" << cl << std::endl;

  // yhx::ZlibStream::ptr stream = yhx::ZlibStream::CreateGzip(false);
  // stream->write(body.c_str(), body.size());
  // stream->flush();

  // body = stream->getResult();

  std::ofstream ofs("http.txt");
  ofs << body;
}

int main(int argc, char** argv) {
  yhx::IOManager iom(2);
  iom.schedule(run);
  // iom.schedule(test_parser);
  return 0;
}
