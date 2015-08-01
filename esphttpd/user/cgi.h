#ifndef CGI_H
#define CGI_H

#include "httpd.h"

typedef struct Station Station;
typedef struct Lamp Lamp;
typedef struct ESPInput ESPInput;
typedef struct ESPOutput ESPOutput;



int cgiLed(HttpdConnData *connData);
//step2
//int setConfig(HttpdConnData *connData);
int postLed(HttpdConnData *connData);
//int getLed(HttpdConnData *connData);
int getStationInputs(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);
int cgiReadFlash(HttpdConnData *connData);
int tplCounter(HttpdConnData *connData, char *token, void **arg);
int cgiUploadEspfs(HttpdConnData *connData);

#endif
