#ifndef _SYSGLOBAL_
#define _SYSGLOBAL_

#define SERVER_CONF "webserver.conf"

#define ACCESS_LOG "access.log"
#define SRV_LOG "webserver.log"

#define BUFFERSIZE 104857600

struct sysglobal;
typedef struct sysglobal{
    int port;
    char* default_location;
    char* source_dir;

} sysglobal;

// global init
void setup_sysglobals();
void read_server_conf();
void setValue(char* name, char* value);

// global retrieval
int getport();
char* getdefaultlocation();
char* getsourcedirectory();
// free
void freeconfig();
#endif // !DEBUG
