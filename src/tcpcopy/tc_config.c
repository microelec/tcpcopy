

#include <tc_config.h>
#include <stdio.h>

uint32_t tcpCopyConf[MAX_TC_CONF_INDEX];

#define SKIP_SPACES(ptr) while(*ptr == ' ' || *ptr == '\t') ptr++;

void initializeProductConfiguration()
{
  int i=0;
  for(;i<MAX_TC_CONF_INDEX;i++)
    tcpCopyConf[i] = 0;

  tcpCopyConf[RunModeIndex] = Online;

}

bool isOfflineMode() 
{
  return tcpCopyConf[RunModeIndex]==PcapOffline;
}


void doConfiguration(uint32_t index, uint32_t value) 
{
  tcpCopyConf[index]=value;
}

// parseLine
// retrun true for "index=value" pattern
static bool parseLine(char *string_ptr, uint32_t *index_ptr, uint32_t *value_ptr)
{
  char *end_ptr = NULL;
  uint32_t param;
  char *origLine = string_ptr;

  SKIP_SPACES(string_ptr)

  //Check if pure comment line 
  if(*string_ptr == '#')
  {
    *index_ptr = 0;
    *value_ptr = 0;
    return false;
  }
  // Check if empty line 
  if(*string_ptr == '\0' || *string_ptr == '\r' || *string_ptr == '\n')
  {
    *index_ptr = 0;
    *value_ptr = 0;
    return false;
  }

  // Convert string presentation of domain and index to long value 
  param = (uint32_t) strtoul(string_ptr, &end_ptr, 0);
  *index_ptr = param;
  
  
  // Check if string ended too early 
  if(end_ptr == NULL)
  {
    *index_ptr = 0;
    *value_ptr = 0;
    
    tc_log_info(LOG_ERR, 0, "parseLine() Error when parsing line: %s",origLine);
    return false;
  }

  SKIP_SPACES(end_ptr)

  if(*end_ptr != '=')
  {
    *index_ptr = 0;
    *value_ptr = 0;
    
    tc_log_info(LOG_ERR, 0, "parseLine() Error when parsing line: %s",origLine);
                   
    return false;
  }

  string_ptr = end_ptr;
  string_ptr++;
  end_ptr = NULL; 

  SKIP_SPACES(string_ptr)

  // Convert string presentation of value to long value 
  *value_ptr = (uint32_t) strtoul(string_ptr, &end_ptr, 0);

  // Check if string ended OK 
  if(end_ptr != NULL)
  {
    SKIP_SPACES(end_ptr)
    
    if(*end_ptr == '\0')
    {
      //This line is the last in file 
      return false;
    }

    if(*end_ptr != '#' && *end_ptr != '\n' && *end_ptr != '\r')
    {
      // Parse error 
      *index_ptr = 0;
      *value_ptr = 0;
      
      tc_log_info(LOG_ERR, 0, "parseLine() Error when parsing line: %s",origLine);
      return false;
    }
  }
  
  return true;
}

void readConfigurationFile(char *fileName)
{
  //fopen
  FILE* fp = NULL;
  fp = fopen(fileName,"r");
  if(fp==NULL)
  {
    tc_log_info(LOG_ERR, 0, "file open fail: %s",fileName);
    return;
  }

  //readline and parse line
  char lineBuf[200];
  uint32_t confIndex = 0;
  uint32_t confValue = 0;
   
  while(NULL !=fgets(lineBuf, sizeof(lineBuf)-1, fp))
  {
    bool parseOk = parseLine(&lineBuf[0],&confIndex,&confValue);

    if(parseOk && confIndex<MAX_TC_CONF_INDEX)
      doConfiguration(confIndex,confValue);
  }
  //fclose
  fclose(fp);
}
  


