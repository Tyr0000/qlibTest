#include "stdafx.h"
#include "DataDescriptorImport.h"
#include "DlgParamsBuilder.h"

typedef struct _DLG_DATA
{
    char DlgName[128];
}
DLG_DATA;

std::vector<PARAM_DATA> ListParams;

char ScalarTst[]= 
"dialog = speed_settings,   \"Speed Sensors\"\r\n\
   field = \"#Crankshaft Speed Settings\"\r\n\
       field = \"Tooth Count\", trigg1_cnt\r\n\
       field = \"Pulse divider\", trigg1_div\r\n\
       field = \"Missing Tooth Wheel\", trigger_conf_missing\r\n\
   field = \"#Driven Wheel Speed Sensor Settings\"";

void ImportParamsDialog(char *pData)
{
    char *pSym= ScalarTst; // pData;
    char *pValStr;
    bool IsEOL= false;
    
    pSym= strstr(pSym, "dialog"); 
    if (pSym == NULL) return;
    SKIP_UNTIL_SYM(pSym, '"')
    pSym++;    
    pValStr= pSym;
    SKIP_UNTIL_SYM(pSym, '"')
    *pSym= 0;
    pSym++;
    
    while (*pSym != 0) 
    {
        pSym= strstr(pSym, "field"); 
        if (*pSym == 0) break;
        SKIP_UNTIL_SYM(pSym, '"')
        if (*pSym == NULL)
            break;
        pSym++;    
        pValStr= pSym;
        SKIP_UNTIL_SYM(pSym, '"')
        *pSym= 0;
        pSym++;    
        SKIP_WHITE_SPACE(pSym)
        if (IsEOL)
        {
            if (!IS_ALPHA(pValStr)) pValStr++;
        }
//        GOTO_NEXT_LINE(pSym) // skip type
    }
}

void ImportMenuStructure(char *pData)
{
    char *pSym= ScalarTst; // pData;
    char *pValStr;
    bool IsEOL= false;
    
    pSym= strstr(pSym, "menu"); 
    if (pSym == NULL) return;
    SKIP_UNTIL_SYM(pSym, '"')
    pSym++;    
    pValStr= pSym;
    SKIP_UNTIL_SYM(pSym, '"')
    *pSym= 0;
    if (!IS_ALPHA(pValStr)) pValStr++;
    pSym++;
    
    while (*pSym != 0) 
    {
        pSym= strstr(pSym, "subMenu"); 
        if (*pSym == 0) break;
        SKIP_UNTIL_SYM(pSym, '"')
        if (*pSym == NULL)
            break;
        pSym++;    
        pValStr= pSym;
        SKIP_UNTIL_SYM(pSym, '"')
        *pSym= 0;
        pSym++;    
        SKIP_WHITE_SPACE(pSym)
        if (IsEOL)
        {
            if (!IS_ALPHA(pValStr)) pValStr++;
        }
//        GOTO_NEXT_LINE(pSym) // skip type
    }
}