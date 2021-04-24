#include <stdio.h>
#include "parse.h"

#ifndef UCCI_H
#define UCCI_H

const int UCCI_MAX_DEPTH = 32;


enum UcciCommEnum {UCCI_COMM_UNKNOWN, UCCI_COMM_UCCI, UCCI_COMM_ISREADY, UCCI_COMM_POSITION,  UCCI_COMM_GO,  UCCI_COMM_QUIT}; 


union UcciCommStruct {
  struct {
    const char *szFenStr;     // 
    int nMoveNum;             
    char (*lpdwMovesCoord)[5]; //记录moves后的走法
  };

};


#endif


const int MAX_MOVE_NUM = 1024;

const int LINE_INPUT_MAX_CHAR = 8192;
static char szFen[LINE_INPUT_MAX_CHAR];
static char dwCoordList[MAX_MOVE_NUM][5];

static bool ParsePos(UcciCommStruct &UcciComm, char *lp) {
  int i;
  if (StrEqvSkip(lp, "fen ")) 
  {
    strcpy(szFen, lp);
    UcciComm.szFenStr = szFen;
  } 
  else if (StrEqv(lp, "startpos")) 
  {
    UcciComm.szFenStr = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";
  } 
  else 
  {
    return false;
  }
  
  UcciComm.nMoveNum = 0;
  if (StrScanSkip(lp, " moves ")) 
  {
    *(lp - strlen(" moves ")) = '\0';
    UcciComm.nMoveNum = MIN((int) (strlen(lp) + 1) / 5, MAX_MOVE_NUM); 
    for (i = 0; i < UcciComm.nMoveNum; i ++) 
    {
      dwCoordList[i][0] = *lp; 
      dwCoordList[i][1] = *(lp+1); 
      dwCoordList[i][2] = *(lp+2); 
      dwCoordList[i][3] = *(lp+3); 
      dwCoordList[i][4] = '\0'; 
      lp += 5;
    }
    UcciComm.lpdwMovesCoord = dwCoordList;
  }
  return true;
}

UcciCommEnum BootLine(void) 
{
  char szLineStr[LINE_INPUT_MAX_CHAR];
  
  while (!std::cin.getline(szLineStr, LINE_INPUT_MAX_CHAR)) 
  {
    Sleep(1);
  }
  if (StrEqv(szLineStr, "ucci")) 
  {
    return UCCI_COMM_UCCI;
  } 
  else 
  {
    return UCCI_COMM_UNKNOWN;
  }
}

UcciCommEnum IdleLine(UcciCommStruct &UcciComm, bool bDebug) 
{
  char szLineStr[LINE_INPUT_MAX_CHAR];
  char *lp;
  int i;
  bool bGoTime;

  while (!std::cin.getline(szLineStr, LINE_INPUT_MAX_CHAR)) 
  {
      Sleep(1);
  }
  lp = szLineStr;
  if (bDebug) 
  {
    printf("info idleline [%s]\n", lp);
    fflush(stdout);
  }
  if (false) 
  {
  } 
  else if (StrEqv(lp, "isready")) 
  {
    return UCCI_COMM_ISREADY;
  } 

  else if (StrEqvSkip(lp, "position ")) 
  {
		return ParsePos(UcciComm, lp) ? UCCI_COMM_POSITION : UCCI_COMM_UNKNOWN;
	}
 
  else if (StrEqvSkip(lp, "go time ")) {
		//bGoTime = true;
		//UcciComm.nTime = Str2Digit(lp, 0, 2000000000);
		return UCCI_COMM_GO;
	}

  else if (StrEqv(lp, "quit")) 
  {
    return UCCI_COMM_QUIT;
  } 
  else 
  {
    return UCCI_COMM_UNKNOWN;
  }
}

UcciCommEnum BusyLine(UcciCommStruct &UcciComm, bool bDebug) 
{
  char szLineStr[LINE_INPUT_MAX_CHAR];
  char *lp;
  if (std::cin.getline(szLineStr, LINE_INPUT_MAX_CHAR)) 
  {
    if (bDebug) 
    {
      printf("info busyline [%s]\n", szLineStr);
      fflush(stdout);
    }
    if (false) 
    {
    } 
    else if (StrEqv(szLineStr, "isready")) 
    {
      return UCCI_COMM_ISREADY;
    } 
    else if (StrEqv(szLineStr, "quit")) 
    {
      return UCCI_COMM_QUIT;
    } 
    else 
    {
      lp = szLineStr;
      return UCCI_COMM_UNKNOWN;
      
    }
  } 
  else 
  {
    return UCCI_COMM_UNKNOWN;
  }
}
