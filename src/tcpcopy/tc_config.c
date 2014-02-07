

#include <tc_config.h>
#include <stdio.h>
#include <string.h>

#define SKIP_SPACES(ptr) while(*ptr == ' ' || *ptr == '\t') ptr++;
static void set_num(void *conf, uint32_t confIndex, char *value);
static void readConfigurationFile(char *fileName);


uint32_t tcpCopyConf[MAX_TC_CONF_INDEX];
#define ERROR -1

SConfMapping confMappingTable[]=
{
{"runMode",RunModeIndex,set_num}


};

void initializeProductConfiguration()
{
  int i=0;
  for(;i<MAX_TC_CONF_INDEX;i++)
    tcpCopyConf[i] = 0;

  tcpCopyConf[RunModeIndex] = Online;

  /* Set config file path, only support default path currently*/
  clt_settings.config_path = "/usr/local/etc/tcpcopy.conf";

  
  readConfigurationFile(clt_settings.config_path); 

}

bool isOfflineMode() 
{
  return tcpCopyConf[RunModeIndex]==PcapOffline;
}


static uint32_t str_to_uint(char *line)
{
    int  value;
    for(value = 0; *line != '\0'; line++) {
        if (*line < '0' || *line > '9') {
            return ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    if (value < 0) {
        return ERROR;

    } else {
        return value;
    }
}

void set_str(void *conf, uint32_t confIndex, char *value)
{
  
}
static void set_num(void *conf, uint32_t confIndex, char *value)
{
  uint32_t *p = conf;
  uint32_t val = str_to_uint(value);
  if(ERROR != val)
    *(p + confIndex) = val;
}

static uint32_t findConfIndex(char* key)
{
  int cfgNum = sizeof(confMappingTable)/sizeof(confMappingTable[0]);
  SConfMapping *item = &confMappingTable[0];
  int i;
  for(i = 0; i<cfgNum; i++)
    if(0 == strcmp(item[i].key,key))
      return i;

 return ERROR;
}

static void doConfiguration(char* key, char* value) 
{
  //tcpCopyConf[index]=value;
  uint32_t mappingTableIndex = findConfIndex(key);
  if(ERROR == mappingTableIndex)
  {
    tc_log_info(LOG_ERR, 0, "doConfiguration() Error: not supported configuration %s",key);
    return;
  }
  //Only support "str=int" format first
  confMappingTable[mappingTableIndex].set(tcpCopyConf,confMappingTable[mappingTableIndex].index,value);
  
}

/* parseLine
 * retrun true for "index=value" pattern
 */
static bool parseLine(char *string_ptr, char *index_ptr, char *value_ptr)
{
  char *origLine = string_ptr;

  SKIP_SPACES(string_ptr)

  /* Check if pure comment line */
  if(*string_ptr == '#')
  {
    *index_ptr = 0;
    *value_ptr = 0;
    return false;
  }
  /* Check if empty line */
  if(*string_ptr == '\0' || *string_ptr == '\r' || *string_ptr == '\n')
  {
    *index_ptr = 0;
    *value_ptr = 0;
    return false;
  }

  /* Get the key*/
  int i = 0;
  while('=' !=*string_ptr &&' ' !=*string_ptr&&'\t' !=*string_ptr&& i<20)
  {
    *index_ptr++ = *string_ptr++;
    i++;
  }
  *index_ptr = '\0';
  SKIP_SPACES(string_ptr)
  
  if('=' !=*string_ptr)
  {
    tc_log_info(LOG_ERR, 0, "parseLine() Error when parsing line: %s",origLine);
    return false;
  }
  /* skip '='*/
  string_ptr++;

  SKIP_SPACES(string_ptr)

  /* Get the value*/
  i = 0;
  while('#' !=*string_ptr &&'\n' !=*string_ptr&&'\r' !=*string_ptr&&' ' !=*string_ptr&&'\t' !=*string_ptr&& i<20)
  {
    *value_ptr++ = *string_ptr++;
    i++;
  }
  *value_ptr = '\0';
  return true;
}

static void readConfigurationFile(char *fileName)
{
  /*readline and parse line*/
  char lineBuf[200];
  char confIndex[21];
  char confValue[21];
 
  FILE* fp = NULL;
  fp = fopen(fileName,"r");
  if(fp==NULL)
  {
    tc_log_info(LOG_ERR, 0, "file open fail: %s",fileName);
    return;
  }

  
  while(NULL !=fgets(lineBuf, sizeof(lineBuf)-1, fp))
  {
    bool parseOk = parseLine(&lineBuf[0],&confIndex,&confValue);

    if(parseOk)
      doConfiguration(confIndex,confValue);
  }
  fclose(fp);
}
  


