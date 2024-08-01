#include "sysglobal.h"
#include "../utils/linkedList.h"
#include "syslog.h"
#include "../utils/fileLib.h"
#include "log.h"


void setup_sysglobals(){
    if (!fileExists(SERVER_CONF)) {
        write_to_log("No Server Configuration found continuing with default", SRV_LOG_LVL);
    }else {
        read_server_conf();
    }
}

void read_server_conf(){
    write_to_log("Reading configuration file and configureing server", SRV_LOG_LVL);
    linkedList *conf;

    conf = rFileList(SERVER_CONF);
    LLFOREACH(conf, first, next, cur){
        char* line = cur->value;
        // String functions.
    } 

}
