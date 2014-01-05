
#ifndef _TC_CONFIG_
#define _TC_CONFIG_

#include <stdio.h>
#include <xcopy.h>
#include <tcpcopy.h>
typedef unsigned int uint32_t;
#define MAX_TC_CONF_INDEX 64

typedef enum EConfIndex
{
  RunModeIndex = 1,
  MaxTcConfIndex =MAX_TC_CONF_INDEX
}EConfIndex;

typedef enum ERunMode
{
  Online = 1,
  PcapOffline =2
}ERunMode;

//global function
void initializeConfiguration();
bool isOfflineMode();
void readConfigurationFile(char *fileName);

#endif
