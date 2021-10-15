#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct symbols {
	int	DefinedOnSourceLine;
	int	Address; 
	char	Name[7]; } Symbol;

typedef struct symbols	SYMBOL;

struct opcode {
	int	OpCode;	
	char	Name[8];

};

typedef struct opcode OPCODE;


OPCODE OpcodeTable[ 59 ];

int IsAnInstruction( char *Test );
int IsAValidSymbol( char *TestSymbol );
int IsADirective( char *Test );
int isValidHex( char *hexString );
int hanStart( char *start, char *startAddr, char lineCopy[1024], int numLine );
int checkIfSymbolExits(SYMBOL *tab[], char *sname);
void addSymToTab(int addr, char *sname, SYMBOL *table[]);
void fixHexLength();
void fixCharLength();
void addTrecord();
int searchOpcode(char *opCode);
