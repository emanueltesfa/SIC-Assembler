#include "headers.h"
const OPCODE opcode[59] = {
        { 24, "ADD" }, { 88, "ADDF" }, { 144, "ADDR" }, { 64, "AND" }, { 180, "CLEAR" }, { 40, "COMP" }, { 136, "COMPF" },
        { 160, "COMPR" }, { 36, "DIV" }, { 100, "DIVF" }, { 156, "DIVR" }, { 196, "FIX" }, { 192, "FLOAT" },
        { 244, "HIO" }, { 60, "J" },{ 48, "JEQ" }, { 52, "JGT" }, { 56, "JLT" }, { 72, "JSUB" }, { 0, "LDA" },
        { 104, "LDB" }, { 80, "LDCH" }, { 112, "LDF" }, { 8, "LDL" }, { 108, "LDS" }, { 116, "LDT" }, { 4, "LDX" },
        { 208, "LPS" }, { 32, "MUL" }, { 96, "MULF" }, { 152, "MULR" }, { 200, "NORM" }, { 68, "OR" }, { 216, "RD" },
        { 172, "RMO" }, { 76, "RSUB" }, { 164, "SHIFTL" }, { 168, "SHIFTR" }, { 240, "SIO" }, { 236, "SSK" },
        { 12, "STA" },{ 120, "STB" }, { 84, "STCH" }, { 128, "STF" }, { 212, "STI" }, { 20, "STL" }, { 124, "STS" },
        { 232, "STSW" }, {132, "STT" }, { 16, "STX" }, { 28, "SUB" }, { 92, "SUBF" }, { 148, "SUBR" }, { 176, "SVC" },
        { 224, "TD" }, { 248, "TIO" }, { 44, "TIX" }, { 184, "TIXR" }, { 220, "WD" }
};


int searchOpcode(char *inputOp){
	
	int opNum = -1;
        //printf("Opcode passed in is: %s\n", inputOp);

        for (int i = 0; i <59; i++){
        	
		//printf("Opcode table name: %s\n", opcode[i].Name);
		if(strcmp (opcode[i].Name, inputOp) == 0  ){
			//printf("\n Opcode number is : %d\n", opcode[i].OpCode);
			opNum = opcode[i].OpCode; 
			//return opcode[i].OpCode;				
		}	
        	 
	}
        return opNum;
}
