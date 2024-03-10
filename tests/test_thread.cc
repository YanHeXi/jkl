#include "yhx/yhx.h"
#include <unistd.h>

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

int count = 0;
yhx::RWMutex s_mutex;
// yhx::Mutex s_mutex;

void fun1()
{
    YHX_LOG_INFO(g_logger) << "name: " << yhx::Thread::GetName()
                           << " this.name: " << yhx::Thread::GetThis()->getName()
                           << " id: " << yhx::GetThreadId()
                           << " this.id: " << yhx::Thread::GetThis()->getId();

    for (int i = 0; i < 100000; ++i)
    {
        yhx::RWMutex::WriteLock lock(s_mutex);
        // yhx::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2()
{
    int i = 1000;
    while (i--)
    {
        YHX_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3()
{
    int i = 1000;
    while (i--)
    {
        YHX_LOG_INFO(g_logger) << "========================================";
    }
}

int main(int argc, char **argv)
{
    YHX_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/yanhexi/vscode_project/YHX_Server/bin/conf/log2.yml");
    yhx::Config::LoadFromYaml(root);

    std::vector<yhx::Thread::ptr> thrs;
    for (int i = 0; i < 5; ++i)
    {
        yhx::Thread::ptr thr(new yhx::Thread(&fun2, "name_" + std::to_string(i * 2)));
        yhx::Thread::ptr thr2(new yhx::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        thrs.push_back(thr2);
    }
    for (size_t i = 0; i < thrs.size(); ++i)
    {
        thrs[i]->join();
    }
    YHX_LOG_INFO(g_logger) << "thread test end";
    YHX_LOG_INFO(g_logger) << "count=" << count;

    return 0;
}
