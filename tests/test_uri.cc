#include <iostream>

#include "yhx/uri.h"

int main(int argc, char** argv) {
  // yhx::Uri::ptr uri =
  //     yhx::Uri::Create("http://www.sylar.top/test/uri?id=100&name=yhx#frg");
  yhx::Uri::ptr uri = yhx::Uri::Create(
      "http://admin@www.sylar.top/test/中文/"
      "uri?id=100&name=yhx&vv=中文#frg中文");
  // yhx::Uri::ptr uri = yhx::Uri::Create("http://admin@www.sylar.top");
  // yhx::Uri::ptr uri = yhx::Uri::Create("http://www.sylar.top/test/uri");
  std::cout << uri->toString() << std::endl;
  auto addr = uri->createAddress();
  std::cout << *addr << std::endl;
  return 0;
}
