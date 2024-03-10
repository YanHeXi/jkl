#include "yhx/http/http.h"
#include "yhx/log.h"

void test_request()
{
    yhx::http::HttpRequest::ptr req(new yhx::http::HttpRequest);
    req->setHeader("host", "www.baidu.com");
    req->setBody("hello yhx");
    req->dump(std::cout) << std::endl;
}

void test_response()
{
    yhx::http::HttpResponse::ptr rsp(new yhx::http::HttpResponse);
    rsp->setHeader("X-X", "yhx");
    rsp->setBody("hello yhx");
    // rsp->setStatus((yhx::http::HttpStatus)200);
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char **argv)
{
    test_request();
    test_response();
    return 0;
}
