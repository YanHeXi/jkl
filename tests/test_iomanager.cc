#include "yhx/yhx.h"
#include "yhx/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

int sock = 0;

void test_fiber()
{
    YHX_LOG_INFO(g_logger) << "test_fiber sock=" << sock;

    sleep(3);

    close(sock);
    yhx::IOManager::GetThis()->cancelAll(sock);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    // inet_pton(AF_INET, "115.239.210.27", &addr.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (!connect(sock, (const sockaddr *)&addr, sizeof(addr)))
    {
    }
    else if (errno == EINPROGRESS)
    {
        YHX_LOG_INFO(g_logger) << "add event errno=" << errno << " " << strerror(errno);
        yhx::IOManager::GetThis()->addEvent(sock, yhx::IOManager::READ, []()
                                            { YHX_LOG_INFO(g_logger) << "read callback"; });
        yhx::IOManager::GetThis()->addEvent(sock, yhx::IOManager::WRITE, []()
                                            {
            YHX_LOG_INFO(g_logger) << "write callback";
            //close(sock);
            yhx::IOManager::GetThis()->cancelEvent(sock, yhx::IOManager::READ);
            close(sock); });
    }
    else
    {
        YHX_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
    }
}

void test1()
{
    std::cout << "EPOLLIN=" << EPOLLIN
              << " EPOLLOUT=" << EPOLLOUT << std::endl;
    yhx::IOManager iom(2, false);
    // yhx::IOManager iom;
    iom.schedule(&test_fiber);
}

yhx::Timer::ptr s_timer;
void test_timer()
{
    yhx::IOManager iom(2);
    s_timer = iom.addTimer(
        1000, []()
        {
            static int i = 0;
            YHX_LOG_INFO(g_logger) << "hello timer i=" << i;
            if(++i == 3) {
            s_timer->reset(2000, true);
            // s_timer->cancel();
            } },
        true);
}

int main(int argc, char **argv)
{
    // test1();
    test_timer();
    return 0;
}
