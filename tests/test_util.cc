#include "yhx/yhx.h"
#include <assert.h>

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

void test_assert()
{
    YHX_LOG_INFO(g_logger) << yhx::BacktraceToString(10);
}

int main()
{
    test_assert();
    return 0;
}