#include <cstdio>
#include <DesktopTown/Context.hpp>

static void on_start(DesktopTown::Context& ctx)
{
    fprintf(stderr, "start\r\n");
}

static void on_update(DesktopTown::Context& ctx)
{
    fprintf(stderr, "update\r\n");
}

static void on_stop(DesktopTown::Context& ctx)
{
    fprintf(stderr, "stop\r\n");
}

int main()
{
    DesktopTown::Context({on_start, on_update, on_stop});
}
