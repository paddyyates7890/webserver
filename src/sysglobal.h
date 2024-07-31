#ifndef _SYSGLOBAL_
#define _SYSGLOBAL_

#define SERVER_CONF "webserver.conf"

#define ACCESS_LOG "access.log"
#define SRV_LOG "webserver.log"

#define BUFFERSIZE 104857600

void setup_sysglobals();
void read_server_conf();

#endif // !DEBUG
