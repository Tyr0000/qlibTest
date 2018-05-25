#define IS_ALPHA(pStr) \
    (((*pStr >= '0') && (*pStr <= '9')) || ((*pStr >= 'A') && (*pStr <= 'Z')) || ((*pStr >= 'a') && (*pStr <= 'z')))

#define IS_DIGIT(pStr) \
    ((*pStr >= '0') && (*pStr <= '9'))

#define IS_COMMENT(pSym) \
	(*pSym == ';')
	
#define IS_WHITE_SPACE(pStr) \
	((*pStr != 0) && ((*pStr == ',') || (*pStr == ' ') || (*pStr == '\t')))
	
#define IS_EOL(pStr) \
	((*(pStr) == 0x0D) || (*(pStr) == 0x0A))
	
#define SKIP_EOL(pStr) \
	while IS_EOL(pStr) \
	{ \
			IsEOL= true; \
			pStr++; \
	}

#define SKIP_UNTIL_SYM(pStr, Sym) \
    IsEOL= false; \
    while ((*pStr != 0) && !IS_EOL(pStr))\
    { \
        if (*pStr == Sym) \
        { \
            break; \
        } \
        pStr++; \
    }

#define SKIP_WHITE_SPACE(pStr) \
	while IS_WHITE_SPACE(pStr) \
	{ pStr++; } 
    
#define GOTO_NEXT_LINE(pStr) \
	do \
  { \
    IsEOL= false; \
    while (*pStr != 0) \
    { \
        SKIP_EOL(pStr) \
        if (IsEOL) \
					break; \
        pStr++; \
    } \
    SKIP_WHITE_SPACE(pStr) \
	} \
	while ((*pStr != 0) && (IS_COMMENT(pStr) || IS_EOL(pStr)));

#define FIND_TAG(pStr, StrTag, StrLen) \
    IsFound= false; \
    do \
    { \
        GOTO_NEXT_LINE(pStr) \
        if (*pStr == 0) break; \
        if (strncmp(pStr, #StrTag, StrLen) == 0) \
        { \
			pStr+= StrLen; \
            SKIP_WHITE_SPACE(pStr) \
            if (*pStr == '=') \
            { \
			    pStr++; \
                SKIP_WHITE_SPACE(pStr) \
                IsFound= true; \
                break; \
            } \
        } \
    } \
    while (*pStr != 0);
    
#define SKIP_TAG(pStr, StrLen) \
  IsFound= false; \
	pStr+= StrLen; \
  SKIP_WHITE_SPACE(pStr) \
  if (*pStr == '=') \
  { \
	  pStr++; \
/*      SKIP_WHITE_SPACE(pStr) */\
      IsFound= true; \
  } 

#define SKIP_TAG_TILL_STR_VAL(pStr) \
    IsFound= false; \
    IsEOL= false; \
    while (*pStr != 0) \
    { \
        if (*pStr == '"') \
        { \
            break; \
        } \
        SKIP_EOL(pStr) \
        if (IsEOL) \
		    break; \
        pStr++; \
    }
    
#define GET_STR_VAL(pStr) \
 	IsFound= false; \
	pValStr= NULL; \
	SKIP_WHITE_SPACE(pStr) \
	if (*pStr == '"') \
	{ \
		pStr++; \
		if (*pStr == '"') pStr++; \
		else \
		{ \
		    if (!IS_ALPHA(pStr)) pStr++; \
		    pValStr= pStr; \
		    SKIP_UNTIL_SYM(pStr, '"') \
		    if (*pStr == '"') \
		    { \
			    *pStr= 0; \
			    pStr++; \
			    IsFound= true; \
		    } \
		} \
	}  
   
#define GET_VAL(pStr) \
	IsFound= false; \
	pVal= NULL; \
	SKIP_WHITE_SPACE(pStr) \
	if (IS_ALPHA(pStr)) \
	{ \
    	pVal= pStr; \
    	do \
    	{ \
    		pStr++; \
    	} \
    	while (IS_ALPHA(pStr) || (*pStr == '_') || (*pStr == '-') || (*pStr == '[') || (*pStr == ']' || (*pStr == ':'))); \
    	if (*pStr != 0) \
    	{ \
		    *pStr= 0; \
		    pStr++; \
		    IsFound= true; \
		} \
	} 
	
#define GET_NUM_VAL(pStr) \
	IsFound= false; \
	pVal= NULL; \
	SKIP_WHITE_SPACE(pStr) \
	if (IS_DIGIT(pStr) || (*pStr == '-') || (*pStr == '.')) \
	{ \
    	pVal= pStr; \
    	do \
    	{ \
    		pStr++; \
    	} \
    	while (IS_DIGIT(pStr) || (*pStr == '-') || (*pStr == '.')); \
    	if (*pStr != 0) \
    	{ \
		    *pStr= 0; \
		    pStr++; \
		    IsFound= true; \
		} \
	} 
	
#define FIND_SUB_STR(pStrStart, pStrEnd, pMask, MaskLen, pTargetStr) \
    pTargetStr= pStrStart; \
    do \
    { \
        if (memcmp(pTargetStr, pMask, MaskLen) == 0)\
            break; \
            pTargetStr++; \
        if (pTargetStr >= pStrEnd) \
        { \
            pTargetStr= NULL; \
            break; \
        } \
    } \
    while (true); 
	
/*
#define IS_ALPHA         IS_ALPHA(Parser.pStr) 
#define IS_COMMENT                  IS_COMMENT(Parser.pStr) 
#define SKIP_EOL                   SKIP_EOL(Parser.pStr) 
#define GET_VAL                     GET_VAL(Parser.pStr) 
#define GET_STR_VAL                 GET_STR_VAL(Parser.pStr) 
#define SKIP_TAG_TILL_STR_VAL       SKIP_TAG_TILL_STR_VAL(Parser.pStr) 
#define SKIP_TAG                    SKIP_TAG(Parser.pStr, StrLen) 
#define FIND_TAG                    FIND_TAG(Parser.pStr, StrTag, StrLen) 
#define GOTO_NEXT_LINE              GOTO_NEXT_LINE(Parser.pStr) 
#define SKIP_WHITE_SPACE            SKIP_WHITE_SPACE(Parser.pStr) 
#define SKIP_UNTIL_SYM              SKIP_UNTIL_SYM(Parser.pStr, Sym) 
*/
typedef struct _PAARSER
{
    long DataLen;
    char *pStr, *pData, *pValStr, *pVal;
    FILE *F; 
    bool IsEOL;
    bool IsComment;
    bool IsFound;
    _PAARSER() 
    { 
        F= NULL;
        pData= NULL;
        IsEOL= false;
        IsComment= false;
        IsFound= false;
    }
    ~_PAARSER() 
    { 
        Clear();
    }
    void Clear()
    {
	    if (pData != NULL)
		    free(pData);
	    if (F != NULL)
		    fclose(F);
    }
    void Init(TCHAR *pFN)
    {
        Clear();
#ifdef _UNICODE	
    	F= _wfopen(pFN, L"rb");
#else
	    F= fopen(pFN, "rb");
#endif	
        if (F == NULL)
	        return;

        if (fseek(F, 0, SEEK_END) != 0)
	        goto L_EXIT;
        DataLen= ftell(F);
        pData= (char *)malloc(DataLen+ 1);

        if (fseek(F, 0, SEEK_SET) != 0)
	        goto L_EXIT;
        if (fread(pData, 1, DataLen, F) != DataLen)
	        goto L_EXIT;
        pData[DataLen]= 0;	        	
        pStr= pData;
    L_EXIT:    ;
    }
}
PARSER; 

void ImportParamsDialog(char *pData= NULL);
