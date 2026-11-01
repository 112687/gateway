// #include "../thirdparty/log/log.h"
#include "log/log.h"

int main(int argc, char const *argv[])


{

    log_set_level(LOG_DEBUG);

    log_trace("trace...");
    log_debug("debug...");
    log_info("info...");
    log_warn("warn...");
    log_error("error...");
    log_fatal("fatal..., msg=%s", "fatal msg");

 return 0;
}
