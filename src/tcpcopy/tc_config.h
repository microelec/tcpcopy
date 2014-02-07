
#ifndef _TC_CONFIG_
#define _TC_CONFIG_

#include <stdio.h>
#include <xcopy.h>
#include <tcpcopy.h>
typedef unsigned int uint32_t;
#define MAX_TC_CONF_INDEX 64

typedef struct SConfMapping
{
  char*    key;
  uint32_t index;
  char*    (*set)(void *conf, uint32_t confIndex, char *value);
}SConfMapping;

typedef enum EConfIndex
{
  RunModeIndex = 1,
  MaxTcConfIndex =MAX_TC_CONF_INDEX
}EConfIndex;

typedef enum ERunMode
{
  Online = 0,
  PcapOffline =1
}ERunMode;

//global function
void initializeProductConfiguration();
bool isOfflineMode();

#endif
