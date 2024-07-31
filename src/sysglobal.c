#include "sysglobal.h"
#include "syslog.h"
#include "fileLib.h"
#include "log.h"


void setup_sysglobals(){
    if (!fileExists(SERVER_CONF)) {
        write_to_log("No Server Configuration found continuing with default", SRV_LOG_LVL);
    }else {
        read_server_conf();
    }
}

void read_server_conf(){
    write_to_log("SERVER CONF NOT IMPLEMENTED", SRV_LOG_LVL);
}
