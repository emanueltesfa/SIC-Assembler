#include "headers.h"



int IsAValidSymbol( char *TestSymbol ){

	int Result = 1;
	int index = 0;
	int maxlength = 6;
	int done;
	done = 0 ;

	while( !done ) {
//		printf("\nIs digit is : %c\n", TestSymbol[index]);
		if ( TestSymbol[index] == '\0' ) {
			break;
		}
		if( isalnum(TestSymbol[index]) == 0 ){

	          	printf("ASSEMBLY ERROR. Charecter in Symbol %s is invalid " ,TestSymbol);

			Result = 0;
			break;
		}
		if(IsADirective(TestSymbol) == 1){
			done = 1;
			Result = 0;
		}
		if ( index == maxlength ) {
			done = 1;
			Result = 0;
		}
		index++;

	}

/*	if(!(isalpha(TestSymbol)) || !(isdigit(TestSymbol)) ){
   		 Result = 0;
  	}
*/
	return Result;
}

void addSymToTab(int addr, char *sname, SYMBOL *table[]) {
  	int index = 0;
  	SYMBOL *newsym;

 	newsym = malloc(sizeof(SYMBOL));
  	newsym -> Address = addr; //inside newsym set Address to Addr Obj.property = 100; 
 	strcpy(newsym -> Name, sname);

  	while(table[index] != NULL){
		index++;
	}

	table[index] = newsym;

}// int return of index 

int checkIfSymbolExits(SYMBOL *tab[], char *sname){
	int resuct = 0;
	int index = 0;

	while(tab[index]!= NULL){
		if(strcmp(sname,tab[index]->Name) == 0){
			resuct = -1;
			break;
		}
		index++;
	}

	return resuct;
}
