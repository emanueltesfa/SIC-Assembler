#include "headers.h"


int IsADirective( char *Test ){

return		( 	! ( 
		 strcmp( Test, "START" )  &&
		 strcmp( Test, "END" )   &&
		 strcmp( Test, "RESB" )  &&
		 strcmp( Test, "RESW" )  &&
		 strcmp( Test, "RESR" )  &&
		 strcmp( Test, "BYTE" )  &&
		 strcmp( Test, "WORD" )  &&
		 strcmp( Test, "EXPORTS" )   

		 ) ) ;
}

int hanStart(char *start,char *startAddr, char lineCopy[1024], int lineNum){
        int startAddress = 0;
        char *ptr;

        if( strcmp( start, "START") == 0){

                if( isValidHex( startAddr ) == -1){
                        printf("\nASSEMBLY ERROR. Line number %d, Line is: %sInvalid hex in start Address \n", lineNum, lineCopy );
                        return -1;
                } else if(startAddr < 0){
                        printf("\nASSEMBLY ERROR. Line number %d, Line is: %sInvalid start address, is below range \n", lineNum, lineCopy );
                        return -1;
                } else if ( startAddress >= 32768 ){
                        printf("\nASSEMBLY ERROR. Line number %d, Line is: %sInvalid start address, is below range \n", lineNum, lineCopy );
                        return -1;
                } else{}


                  startAddress = strtol (startAddr, &ptr, 16);

                return startAddress;
        }
	//printf("%s", start);
        else{
             	printf("\nASSEMBLY ERROR. Line number %d, Line is: %sInvalid token for start directive \n", lineNum, lineCopy );
                return -1;
        }
}
