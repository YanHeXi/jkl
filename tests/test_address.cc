#include "yhx/address.h"
#include "yhx/log.h"

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

void test()
{
    std::vector<yhx::Address::ptr> addrs;

    YHX_LOG_INFO(g_logger) << "begin";
    // bool v = yhx::Address::Lookup(addrs, "localhost:3080");
    bool v = yhx::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    // bool v = yhx::Address::Lookup(addrs, "www.yhx.top", AF_INET);
    YHX_LOG_INFO(g_logger) << "end";
    if (!v)
    {
        YHX_LOG_ERROR(g_logger) << "lookup fail";
        return;
    }

    for (size_t i = 0; i < addrs.size(); ++i)
    {
        YHX_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }

    auto addr = yhx::Address::LookupAny("localhost:4080");
    if (addr)
    {
        YHX_LOG_INFO(g_logger) << *addr;
    }
    else
    {
        YHX_LOG_ERROR(g_logger) << "error";
    }
}

void test_iface()
{
    std::multimap<std::string, std::pair<yhx::Address::ptr, uint32_t>> results;

    bool v = yhx::Address::GetInterfaceAddresses(results);
    if (!v)
    {
        YHX_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
        return;
    }

    for (auto &i : results)
    {
        YHX_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
                               << i.second.second;
    }
}

void test_ipv4()
{
    // auto addr = yhx::IPAddress::Create("www.yhx.top");
    auto addr = yhx::IPAddress::Create("127.0.0.8");
    if (addr)
    {
        YHX_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char **argv)
{
    test_ipv4();
    // test_iface();
    // test();
    return 0;
}
