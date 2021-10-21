#include "headers.h"

int main(int argc, char *argv[])
{

        FILE *fp;
        char line[1024];
        char *newsym;
        char *nextoken;
        char *tokens;
        int endCounter = 0;
        int lineNumber = 1;
        int locCtr = 0;
        int locCtrArr[50];
        int initAdd = 0;
        SYMBOL *tab[1024];

        for (int x = 0; x < 1025; x++)
        {
                tab[x] = NULL;
        }

        if (argc != 2)
        {
                printf("\nERROR: Usage: %s filename\n", argv[0]);
                return 0;
        }

        fp = fopen(argv[1], "r");

        if (fp == NULL)
        {
                printf("\nERROR: %s could not be opened for reading,\n", argv[1]);
                return 0;
        }
        newsym = malloc(1024 * sizeof(char));
        memset(newsym, '\0', 1024 * sizeof(char));
        nextoken = malloc(1024 * sizeof(char));
        memset(nextoken, '\0', 1024 * sizeof(char));
	memset(tab, '\0', 1024 * sizeof(SYMBOL*));
        //Lines Exist in FP
        while (fgets(line, 1024, fp) != NULL)
        {

                if (locCtr == 32768)
                {
                        printf("\nASSEMBLY ERROR. line number %d, Line is: %sLocation counter exceeds max amount.\n", lineNumber, line);
                        return -1;
                }
                char lineCopy[1024];
                strcpy(lineCopy, line);

                //Check for Comment ASCII
                if (line[0] == 35)
                {
                        //printf("COMMENT:%s", line );
                        continue;
                }

                //If line contains upperCase Char
                if ((line[0] >= 65) && (line[0] <= 90))
                {

                        strcpy(lineCopy, line);
                        newsym = strtok(line, " \t\n");

                        //CHECK FOR START
                        if (lineNumber == 1)
                        {
                                char *tempSymbol = strtok(NULL, " \t\r\n");
                                char *tempAddr = strtok(NULL, " \t\r\n");
                                locCtr = hanStart(tempSymbol, tempAddr, lineCopy, lineNumber);
                                initAdd = locCtr;

                                if (locCtr == -1)
                                {
                                        return -1;
                                }
                        }

                        nextoken = strtok(NULL, " \t\n\r");

                        //CHECK FOR VALID SYMBOL
                        if (IsAValidSymbol(newsym) == 0)
                        {
                                printf("\nASSEMBLY ERROR. Line number %d, Line is: %s . INVALID SYMBOL\n", lineNumber, line);
                                fclose(fp);
                                return -1;
                        }
                        //CHECK IF SYMBOL IS NAMED AFER DIRECTIVE
                        if (IsADirective(newsym) == 1)
                        {
                                printf("\nASSEMBLY ERROR. Line number %d, Line is: %s \n SYMBOL IS NAMED AFTER A DIRECTIVE\n", lineNumber, line);
                                fclose(fp);
                                return -1;
                        }

                        //CHECK IF TOKEN EXISTS AFTER SYMBOL
                        /* if (nextoken == "\t"){
                            printf("\nERROR. NO OPCODE/DIRECTIVE AFTER SYMBOL");
                            return 0;
                          } */

                        if (checkIfSymbolExits(tab, newsym) == 0)
                        {
                                addSymToTab(locCtr, newsym, tab);
                                locCtrArr[lineNumber] = locCtr;
                        }
                        else
                        {
                                printf("\n ASSEMBLY ERROR. Line number %d, Line is: %s \n SYMBOL ALREADY EXISISTS IN TABLE. SYMBOL IS : %s \n", lineNumber, line, newsym);
                                //fclose(fp);
                                return -1;
                        }

                        lineNumber++;
                }

                //IF OPDCODE WITH NO SYMBOL
                else
                {

                        //Test LATER
                        //			printf("This is opcode\n");
                        if (line[0] != 9)
                        {
                                if (IsAValidSymbol(newsym) == 0)
                                {
                                        return -1;
                                }
                        }
                        if (strcmp(nextoken, "END") == 0)
                        {
                                endCounter++;
                                if (endCounter == 2)
                                {
                                        printf("\nASSEMBLY ERROR. Line number %d, Line is: %sCANNOT HAVE 2 END DIRECTIVES\n", lineNumber, line);
                                        return -1;
                                }
                        }
                        locCtrArr[lineNumber] = locCtr;
                        lineNumber++;
                }

                if (line[0] == 13)
                {
                        if (strcmp(nextoken, "END") != 0)
                        {
                                printf("\nASSEMBLY ERROR. Line number %d, Line is: %s \n Cannot have empty lines\n", lineNumber, line);
                                endCounter++;
                                return -1;
                        }
                        else
                        {
                                endCounter++;
                                //printf("/n END COUNTER: %d", endCounter);
                        }
                }

                tokens = strtok(lineCopy, " \t\n\r");

                while (tokens)
                {

                        //printf("\nTOKENS in Loop: %s, NEWSYM: %s\n", tokens,newsym);
                        //CHECK IF SYMBOL AND TOKEN EQUAL
                        if (strcmp(newsym, tokens) == 0)
                        {
                                tokens = strtok(NULL, " \t\n\r");

                                continue;
                        }

                        if (strcmp(tokens, "END") == 0)
                        {
                                endCounter++;
                                if (endCounter > 1)
                                {
                                        printf("\nASSEMBLY ERROR. Line number %d, Line is: %sMultiple end directives not allowed\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "WORD") == 0)
                        {
                                tokens = strtok(NULL, " \t\n\r");
                                if (tokens == NULL)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMissing operand after End\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                int word = atoi(tokens);
                                if (word > 8388608)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sConstant word after directive exceed max word size of 2^23\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                locCtr += 3;
                                if (locCtr > 1048576)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMemory has been exceed\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "RESW") == 0)
                        {
                                tokens = strtok(NULL, " \t\n\r");
                                if (tokens == NULL)
                                {
                                        printf("ASSEMBLY ERROR.Line number %d, Line is: %sMissing operand after REWS\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                locCtr += 3 * atoi(tokens);
                                if (locCtr > 1048576)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMemory has been exceed\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "RESB") == 0)
                        {
                                tokens = strtok(NULL, " \t\n\r");
                                if (tokens == NULL)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMissing operand after Resb\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                locCtr += atoi(tokens);
                                if (locCtr > 1048576)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMemory has been exceed\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "RESR") == 0)
                        {
                                locCtr += 3;
                                if (locCtr > 1048576)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMemory has been exceed\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                tokens = strtok(NULL, " \t\n\r");
                                if (tokens == NULL)
                                {
                                        printf("ASSEMBLY ERROR. Line number: %d, Line is: %sMISSING OPERAND AFTER RESR\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "EXPORTS") == 0)
                        {
                                locCtr += 3;
                                if (locCtr > 1048576)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMemory needed, exceeded memory\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "BYTE") == 0)
                        {
                                tokens = strtok(NULL, " \t\n\r");
                                if (tokens == NULL)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMissing operand after BYTE\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                if (tokens[0] == 'X' && tokens[1] == '\'')
                                {
                                        char *hex = strtok(tokens, "X'");
                                        if (isValidHex(hex) == -1)
                                        {
                                                printf("ASSEMBLY ERROR. Line number %d, Line is: %sInvalid Hex in operand BYTE\n", lineNumber, lineCopy);
                                                return -1;
                                        }
                                        locCtr += (strlen(hex)) / 2;
                                }
                                else if (tokens[0] == 'C' && tokens[1] == '\'')
                                {
                                        char *constant = strtok(tokens, "C'");

                                        locCtr += strlen(constant);
                                }
                                else
                                {
                                        printf("\tASSEMBLY ERROR. Line number %d, Line is:%s Invalid operand after BYTE\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                if (locCtr > 1048576)
                                {
                                        printf("ASSEMBLY ERROR. Line number %d, Line is: %sMemory needed, exceed memory\n", lineNumber, lineCopy);
                                        return -1;
                                }
                                break;
                        }
                        else if (strcmp(tokens, "START") == 0)
                        {
                                break;
                        }
                        //IF NO DIRECTIVES FOUND
                        else
                        {
                                locCtr += 3;
                                break;
                        }
                }
        }
        for (int i = 0; tab[i] != NULL; i++)
        {

                printf("\n%s,\t %x\n", tab[i]->Name, tab[i]->Address);
        }
        rewind(fp);

        int tempLocCtr = 0;
        int totalLines = lineNumber;
        lineNumber = 1;
	printf("\n TOTAL LINES: %d\n", totalLines);

        while (fgets(line, 1024, fp) != NULL)
        {
                char lineCopy2[1024];
                char hex[7];
                strcpy(lineCopy2, line);
                tempLocCtr = 0;
                char *tokens = strtok(line, " \t\r\n");
                printf("\n\nLine Num: %d\n", lineNumber);
		printf("\n Line is: %s", lineCopy2); 
                if (lineCopy2[0] == 35)
                {
                        continue;
                }

                //Retrieve LocCtr
                for (int i = 0; i < totalLines; i++)
                {
                        if (i == lineNumber)
                        {
                                tempLocCtr = locCtrArr[i];
                        }
                }

                //Calcualte H Record
                if (lineNumber == 1)
                {
                        int lenProg = locCtrArr[totalLines - 1] - initAdd;
                        char headerR[12];

                        strcat(headerR, "H");
                        strcat(headerR, tokens); // SHOULD BE 6 SPACES OF NAME OF PROG (Trailing Spaces)
                        sprintf(hex, "%X", initAdd);
                        strcat(headerR, hex); //SHOULD BE 6 SPACE OF START ADDR (Trailing 0)
                        sprintf(hex, "%8.8X", lenProg);
                        strcat(headerR, hex); //SHOULD BE 6 SAPCES (Prior 00) LENGTH OF PROG
                        printf("\nH record: %s\n", headerR);
                        //FIX LENGTH OF EACH RECORD
                }

                //T record
                else if ( (lineNumber > 1) && (lineNumber < (totalLines - 1)) )
                {
                        char tRecord[60];
                        int opRetr = 0;
                        int addRetr = 0;
                        memset(tRecord, 0, sizeof(tRecord)); // Reset array
                        strcat(tRecord, "T");
                        sprintf(hex, "%X", tempLocCtr);
                        strcat(tRecord, "00");
                        strcat(tRecord, hex);
                        strcat(tRecord, "03");

                        //IF SYMBOL IS DECLARED
                        if ((lineCopy2[0] >= 65) && (lineCopy2[0] <= 90))
                        {
                                tokens = strtok(NULL, " \t\r\n");

                                if (IsADirective(tokens) == 0)
                                {
                                       		opRetr = searchOpcode(tokens);
                                        	if (opRetr == -1)
                                        	{
                                                	lineNumber++;
                                               		printf("\n PASS 2 ERROR NOT A VALID OPCODE\n");
                                                	continue;
                                        	}
                                
                                	sprintf(hex, "%X", opRetr);
                                	strcat(tRecord, hex);
                                	tokens = strtok(NULL, " \t\r\n");
                                	addRetr = searchSymTab(tokens, tab);
                                	if (addRetr == 0)
                                	{
                                        	printf("SYMBOL DOES NOT EXIT IN TABLE");
                                        	break;
                                	}
						
                                	sprintf(hex, "%X", addRetr);				
                                	strcat(tRecord, hex);
				
				} //UPDATED BRACKET TO ACCOUNT FOR DIRECTIVES
				else
				{

					if( strcmp(tokens, "BYTE") == 0 ) {
						tokens = strtok(NULL, " \t\r\n");
						
						if(tokens[0] == 'C'){	
							char *temp = strtok( tokens, "C'" );
							int intConv[60]; 
							
							for(int i = 0; i<strlen(temp); i++){
								intConv[i] = (int) temp[i];
								sprintf(hex,"%x",intConv[i]);
								strcat(tRecord, hex);	
							}

						} else if (tokens[0] == 'X'  ){
							char *temp = strtok ( tokens, "X'");
							strcat(tRecord,temp );
						}else{
							printf("\nASSEMBLY ERROR. NOT VALID OPERAND IN BYTE.\n");
						}
					
					} else if(strcmp(tokens, "WORD") == 0 ){
						int convInt; 
						tokens = strtok(NULL, " \t\r\n");
						convInt = atoi ( tokens );
						sprintf(hex , "%6.6X" ,convInt );
						strcat(tRecord, hex); 
					} else if(strcmp(tokens, "RESB") == 0 ){
						lineNumber++;
						continue;					
					} else if(strcmp(tokens, "RESW") == 0 ){
						lineNumber++;
						continue;
					}else{}
				}

                                printf("\nT record: %s\n", tRecord);
                        }
                        //if line has no SYMBOL DECLARED
                        else
                        {

                                if (IsADirective(tokens) == 0)
                                {
                                        opRetr = searchOpcode(tokens);
                                        if (opRetr == -1)
                                        {
                                                lineNumber++;
                                                printf("\n PASS 2 ERROR NOT A VALID OPCODE\n");
                                                continue;
                                        }
                                }
                                //if a directive
                                sprintf(hex, "%X", opRetr);
                                strcat(tRecord, hex);
				if (opRetr == 76 || opRetr ==72){
					printf("\nThis is jSub or R sub\n");
					strcat(tRecord, "RSUB"); // doesnt matter
					printf("\nT record: %s\n", tRecord);
					lineNumber++;
					continue;	
				}else if( opRetr == 80 || opRetr == 84){
					tokens = strtok(NULL, ",");
					addRetr = searchSymTab(tokens, tab);
                                	if (addRetr == 0)
                                	{
                                        	printf("SYMBOL DOES NOT EXIT IN TABLE");
                                        	break;
                                	}

        	                        addRetr += 32768;
                                	sprintf(hex, "%X", addRetr);
                                	strcat(tRecord, hex);
                                	printf("\nT record: %s\n", tRecord);
					lineNumber++;
					continue;
				}
                                //printf("\nTokens2 in else: %s \n", tokens);
				tokens = strtok(NULL, " \t\r\n");
				
				
				
                                addRetr = searchSymTab(tokens, tab);
                                if (addRetr == 0)
                                {
                                        printf("SYMBOL DOES NOT EXIT IN TABLE");
                                        break;
                                }
                                sprintf(hex, "%X", addRetr);
                                strcat(tRecord, hex);
                                printf("\nT record: %s\n", tRecord);
                        }
                        //break;
                }
		
		else {
			int addRetr = 0;
			char eRecord[7];
			strcat(eRecord, "E");
			tokens = strtok(NULL, " \t\r\n");
			tokens = strtok(NULL, " \t\r\n");
			addRetr = searchSymTab(tokens, tab);
			if (addRetr == 0)
			{
				printf("SYMBOL DOES NOT EXIT IN TABLE");
                                break;
			}
			sprintf(hex, "%6.6X", addRetr);
                        strcat(eRecord, hex);
			printf("\nE record: %s\n", eRecord);
		}

                lineNumber++;
        }


        fclose(fp);

	for (int i = 1; tab[i+1] != NULL; i++)
	{
		//free(tab[i]->Name);
                free(tab[i]);
        }
}

int isValidHex(char *hexString)
{
        for (int i = 0; i < strlen(hexString); i++)
        {
                if (isxdigit(hexString[i]) == 0)
                {
                        return -1;
                }
        }
        return 0;
}

int searchSymTab(char *tokens, SYMBOL *table[])
{
        int result = 0;
        printf("\nTokens: %sEND \n", tokens);

        for (int i = 1; table[i+1] != NULL; i++){

                if (strcmp(table[i]->Name, tokens) == 0)
                {
                        result = table[i]->Address;
                }
        }

        return result;
}
