#include "sysglobal.h"
#include "../utils/linkedList.h"
#include "syslog.h"
#include "../utils/fileLib.h"
#include "log.h"
#include "../utils/pstring.h"
#include <string.h>
#include <stdlib.h>

static sysglobal* configuration;

void setup_sysglobals(){
    configuration = calloc(1, sizeof(sysglobal));
    configuration->default_location = NULL;
    configuration->port = NULL;

    if (!fileExists(SERVER_CONF)) {
        write_to_log("No Server Configuration found continuing with default", SRV_LOG_LVL);
    }else {
        read_server_conf();
    }
}

void read_server_conf(){
    write_to_log("Reading configuration file and configureing server", SRV_LOG_LVL);
    
    linkedList *conf;
    linkedList *splitLine;
    char* name;
    char* value;

    conf = rFileList(SERVER_CONF);
    LLFOREACH(conf, first, next, cur){
        char* line = cur->value;
        splitLine = splitstr(line, ' ');
        name = LLindex(splitLine, 0);
        value = LLindex(splitLine, 1);
        
        setValue(name, value);

    } 

    LLdestroy(conf);
    LLdestroy(splitLine);
}

void setValue(char* name, char* value){
    if(strcmp(name, "DEFAULT_LOCATION")){
        configuration->default_location = value;
    }else if (strcmp(name, "PORT")){
        char* output;
        configuration->port = strtol(value, &output, 10)+1;
    }else if (strcmp(name, "SOURCE")){
        configuration->source_dir = value;
    }else {
        write_to_log("config name %s is not valid please check webserver.config", SRV_LOG_LVL, name);
    }


}

int getport(){
    if (configuration->port != NULL) {
        return configuration->port;
    }else {
        return 9999;
    }
}

char* getdefaultlocation(){
    if (configuration->default_location != NULL) {
        return configuration->default_location;
    }else {
        return "index.html";
    }
} 

char* getsourcedirectory(){
    if(configuration->source_dir != NULL){
        return configuration->source_dir;
    }else {
        return "/var/webserver/";
    }
}

void freeconfig(){
    free(configuration);
}
