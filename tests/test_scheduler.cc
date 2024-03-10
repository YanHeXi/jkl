#include "yhx/yhx.h"

static yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

void test_fiber()
{
    static int s_count = 5;
    YHX_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    sleep(1);
    if (--s_count >= 0)
    {
        yhx::Scheduler::GetThis()->schedule(&test_fiber, yhx::GetThreadId());
    }
}

int main(int argc, char **argv)
{
    YHX_LOG_INFO(g_logger) << "main";
    yhx::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    YHX_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    YHX_LOG_INFO(g_logger) << "over";
    return 0;
}
