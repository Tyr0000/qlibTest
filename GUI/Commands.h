#pragma once

typedef struct _CMMND_TAG
{
	BYTE 
		CmmndCode,
		CmmndCodeAdd;			// 0xFF- ReadOnly
	char Title[128];
	char **Values;
/*
	_CMMND_TAG(BYTE Cmmnd, BYTE CmndAdd= 0, char *pTitle= NULL)
		{
		  CmmndCode= Cmmnd;
			CmmndCodeAdd= CmndAdd;
			strcpy(Title, pTitle);
		}
*/
}BOARD_CMMND;

#define GROUPS_CNT			5

extern
char *CommandGroups[];

extern 
BOARD_CMMND 
  Cmmnd_1_1[],
  Cmmnd_1_2[],
  Cmmnd_2_1[];
