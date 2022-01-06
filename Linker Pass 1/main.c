#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char symbol[50], control_section[50], symbol_name[50];
char record_type[50], control_section_name[50];
int starting_address, length_of_cs, symbol_address, CSADR, CSLTH;

int searchESTAB(char toSearch[50], int type)
{
    FILE *fp1=fopen("ESTAB.txt","r");
    if(fp1 == NULL)
    	printf("ERROR - Cant open file ESTAB.txt");

    int found = 0;
    char delimit[]=" \t\r\n\v\f";

    char line[100];
    size_t len = 100 * sizeof(char);

    while ((fgets(&line, &len, fp1)) != NULL)
    {
        int len = 0;
        strcpy(control_section, " ");
        strcpy(symbol, " ");

        char *p = strtok (line, delimit);
        char *array[5];

        while (p != NULL)
        {
            array[len++] = p;
            p = strtok (NULL, delimit);
        }

        if(len == 2){
            strcpy(symbol, array[0]);

        }
        else if(len == 3){
            strcpy(control_section, array[0]);
        }

        // Search
        if(type == 0){
        	if(strcmp(control_section, toSearch) == 0){
        		found = 1;
        	}
        }
        else if(type == 1){
        	if(strcmp(symbol, toSearch) == 0){
        		found = 1;
        	}
        }


    }

    fclose(fp1);

    return found;
}

void pass1(char PROGADDR_HEX[10]){
	int PROGADDR = (int)strtol(PROGADDR_HEX, NULL, 16);
	CSADR = PROGADDR;

	FILE *fp1=fopen("object_program.txt","r");
	if(fp1 == NULL)
		printf("ERROR - Cant open file object_program.txt\n");

	FILE *fp2=fopen("ESTAB.txt","w");
	if(fp2 == NULL)
		printf("ERROR - Cant open file ESTAB.txt\n");
	fclose(fp2);


    char delimit[]=" \t\r\n\v\f";

    char line[100];
    size_t len = 100 * sizeof(char);

    while ((fgets(&line, &len, fp1)) != NULL)
    {
        int len = 0;
        char *p = strtok (line, delimit);
        char *array[100];

        while (p != NULL)
        {
            array[len++] = p;
            p = strtok (NULL, delimit);
        }
        strcpy(record_type, array[0]);


        if(strcmp(record_type, "H") == 0){
        	strcpy(control_section_name, array[1]);
        	starting_address = (int)strtol(array[2], NULL, 16);
           	CSLTH = length_of_cs = (int)strtol(array[3], NULL, 16);

           	if(searchESTAB(control_section_name, 0)){
           		printf("ERROR - Duplicate external symbol found - %s\n", control_section_name);

           	}else{
           		fp2=fopen("ESTAB.txt","a");
				if(fp2 == NULL)
					printf("ERROR - Cant open file ESTAB.txt\n");
        		fprintf(fp2, "%s\t%s\t%x\t%x\n", control_section_name, " ", CSADR + starting_address, length_of_cs);
				fclose(fp2);
        	}
        }
        else if(strcmp(record_type, "D") == 0){
        	int i;
        	for(i = 1; i < len; i=i+2){
        		strcpy(symbol_name, array[i]);
        		symbol_address = (int)strtol(array[i+1], NULL, 16);

        		if(searchESTAB(symbol_name, 1)){
        			printf("ERROR - Duplicate external symbol found - %s\n", symbol_name);

        		}else{
        			fp2=fopen("ESTAB.txt","a");
					if(fp2 == NULL)
						printf("ERROR - Cant open file ESTAB.txt\n");
        			fprintf(fp2, "%s\t%s\t%x\t%s\n", " ", symbol_name, CSADR + symbol_address, " ");
					fclose(fp2);
        		}

        	}

        }
        else if(strcmp(record_type, "E") == 0){
        	CSADR += CSLTH;
        }

    }
    fclose(fp1);

    printf("Completed Pass 1 of Linking Loader\n\n");
}

void showESTAB(){
	FILE *fp1=fopen("ESTAB.txt","r");
	if(fp1 == NULL)
		printf("ERROR - Cant open file ESTAB.txt\n");

    int found = 0;
    char delimit[]=" \t\n";
    char line[100];
    size_t len = 100 * sizeof(char);

    while ((fgets(&line, &len, fp1)) != NULL)
    	printf("%s", line);

    fclose(fp1);
}

int main(){
	char PROGADDR_HEX[20];
	printf("Enter the program address (Starting address) in Hex : ");
	scanf("%s", PROGADDR_HEX);
	pass1(PROGADDR_HEX);
	showESTAB();

	return 0;

}
