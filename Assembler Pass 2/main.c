#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

char label[50], opcode[50], operand[50];
char symbol[50];
char value[10];
char mnemonic[50];
char operand_address[5];
char opcode_address[10];
char locctr[10];
int length = 0;
char text_record[100];
char object_code[20];
char integer[20];
int cur_length = 0;
int is_last = 1;
int starting_address;
int STARTING_ADDR = 0;
int i = 0;


void get_length(){
    FILE *fp4 = fopen("length.txt", "r");
    if(fp4== NULL)
        printf("Error Opening length.txt\n");

    fscanf(fp4, "%d", &length);
}

int check_indexed(){
    int is_indexed = 0;
    char *p = strtok (operand, " ,");
    char *array[3];
    int len = 0;

    while (p != NULL)
    {
        array[len++] = p;
        p = strtok (NULL, " ,");
    }

    if((len == 2) && (strcmp(array[1], "X") == 0)){
        strcpy(operand, array[0]);
        is_indexed = 1;
    }

    return is_indexed;
}

int search_symtab()
{
	FILE *fp5=fopen("symtab.txt","r");
    if(fp5== NULL)
        printf("Error Opening symtab.txt\n");

    int found = 0;
    strcpy(operand_address, "0000");
    while(!feof(fp5))
    {
    	fscanf(fp5,"%s\t%s",symbol,value);
        if(strcmp(symbol,operand)==0)
        {
        	strcpy(operand_address, value);
            found = 1;
        	break;
        }
    }

    fclose(fp5);
    if(!found)
        printf("%s --- Error! - undefined symbol\n", operand);
    return found;
}

int search_optab()
{
	FILE *fp6=fopen("Optab.txt","r");
    if(fp6== NULL)
        printf("Error Opening Optab.txt\n");

 	int found = 0;
    strcpy(opcode_address, "0");
    while(!feof(fp6))
    {
    	fscanf(fp6,"%s\t%s",mnemonic,value);
        if(strcmp(mnemonic, opcode)==0)
        {
        	strcpy(opcode_address, value);
        	found = 1;
        	break;
        }
    }

    fclose(fp6);
    return found;
}

void pass2(){
    FILE *fp1 = fopen("intermediate.txt", "r");
    FILE *fp2 = fopen("output.txt", "w");
    FILE *fp3 = fopen("object program.txt", "w");

    if(fp1== NULL)
        printf("Error Opening intermediate.txt\n");
    if(fp2== NULL)
        printf("Error Opening output.txt\n");
    if(fp3== NULL)
        printf("Error Opening object program.txt\n");

    char delimit[]=" \t\r\n";
    int start;
    char line[100];
    size_t len = 100 * sizeof(char);

    while ((fgets(&line, &len, fp1)) != NULL)
    {
        int len = 0;
        strcpy(label, " ");
        strcpy(opcode, " ");
        strcpy(operand, " ");
        char *p = strtok (line, delimit);
        char *array[5];
        strcpy(object_code, "");

        while (p != NULL)
        {
            array[len++] = p;
            p = strtok (NULL, delimit);
        }

        if(len == 1){
            strcpy(opcode, array[0]);
        }
        else if(len == 2){
            strcpy(locctr, array[0]);
            strcpy(opcode, array[1]);
        }
        else if(len == 3){
            strcpy(locctr, array[0]);
            strcpy(opcode, array[1]);
            strcpy(operand, array[2]);
        }
        else if(len == 4){
            strcpy(locctr, array[0]);
            strcpy(label, array[1]);
            strcpy(opcode, array[2]);
            strcpy(operand, array[3]);
        }

        if(strcmp(opcode, "END")==0)
            break;

        if(strcmp(opcode, "START")==0){
            fprintf(fp2, "%s\t%s\t%s\t%s\n", locctr, label, opcode, operand);
            STARTING_ADDR = starting_address = (int)strtol(operand, NULL, 16);
            get_length();
            for(i = 0; i <= 6 - strlen(label); i++)
                strcat(label, " ");
            fprintf(fp3,"H^%s^%06x^%06x\n", label, starting_address, length);
            fprintf(fp3, "T^");
            fprintf(fp3, "%06x^", starting_address);

            continue;
        }

        if((!strcmp(label, " ")==0) || (!strcmp(opcode, " ")==0) || (!strcmp(operand, " ")==0)){
            if(search_optab()){
                if(!(strcmp(operand, " ") == 0)){
                    int is_indexed = check_indexed();

                    search_symtab();

                    if(is_indexed){
                        strcat(operand, ", X");
                        int num = (int)strtol(operand_address, NULL, 16);
                        num = num | (1 << 15);
                        sprintf(operand_address, "%04x", num);
                    }
                }
                else
                    strcpy(operand_address, "0000");

                strcpy(object_code, strcat(opcode_address, operand_address));
                fprintf(fp2, "%s\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, object_code);

                cur_length = (int)strtol(locctr, NULL, 16) - starting_address;
            }
            else if((strcmp(opcode, "BYTE") == 0) || (strcmp(opcode, "WORD") == 0)){
                if(strcmp(opcode, "WORD") == 0){
                    strcpy(object_code, "");
                    sprintf(integer, "%06x", atoi(operand));
                    strcpy(object_code, integer);
                    fprintf(fp2, "%s\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, object_code);
                    // printf("%s\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, object_code);

                }

                else{
                    fprintf(fp2, "%s\t%s\t%s\t%s\t", locctr, label, opcode, operand);
                    // printf("%s\t%s\t%s\t%s\t", locctr, label, opcode, operand);

                    strcpy(object_code, "");
                    if(operand[0] == 'C' || operand[0] == 'c'){
                        for(i = 2; i < strlen(operand) - 1; i++){
                            sprintf(integer, "%x", operand[i]);
                            strcat(object_code, integer);
                        }
                        fprintf(fp2, "%s\n", object_code);
                        // printf("%s\n", object_code);

                    }

                    else{
                        for(i = 2; i < strlen(operand) - 1; i++){
                            sprintf(integer, "%c", operand[i]);
                            strcat(object_code, integer);
                        }
                        fprintf(fp2, "%s\n", object_code);
                    }

                }
            }
           else{
                fprintf(fp2, "%s\t%s\t%s\t%s\n", locctr, label, opcode, operand);
                // printf("%s\t%s\t%s\t%s\n", locctr, label, opcode, operand);
            }

            if(((int)strtol(locctr, NULL, 16) - starting_address) < 30){
                if(!(strcmp(object_code, "")) == 0){
                    strcat(text_record, "^");
                    strcat(text_record, object_code);
                }else if(is_last){
                    cur_length = (int)strtol(locctr, NULL, 16) - starting_address;
                    is_last = 0;
                }
            }

            else{
                cur_length = (int)strtol(locctr, NULL, 16) - starting_address;
                fprintf(fp3, "%02x%s\n", cur_length, text_record);
                strcpy(text_record, "^");
                strcat(text_record, object_code);
                starting_address = (int)strtol(locctr, NULL, 16);
                fprintf(fp3, "T^");
                fprintf(fp3, "%06x^", starting_address);
                is_last = 1;
            }


        }

    }
    // Remaining records
    fprintf(fp3, "%02x%s\n", cur_length, text_record);
    starting_address = (int)strtol(locctr, NULL, 16);

    // End record
    fprintf(fp3, "E^%06x\n", STARTING_ADDR);


    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    printf("Completed Pass 2\n");
}

void show_object_program(){
    FILE *fp7 = fopen("object program.txt", "r");
    char locctr[50];

    if(fp7== NULL)
        printf("Error Opening output.txt\n");

    printf("\n---------Object Program---------\n");

    char line[100];
    size_t len = 100 * sizeof(char);

    while ((fgets(&line, &len, fp7)) != NULL)
    {
        printf("%s", line);

    }

    fclose(fp7);
}

void show_output(){
    FILE *fp8 = fopen("output.txt", "r");
    char locctr[50];

    if(fp8== NULL)
        printf("Error Opening output.txt\n");

    printf("\n---------Output File---------\n");

    char line[100];
    size_t len = 100 * sizeof(char);

    while ((fgets(&line, &len, fp8)) != NULL)
        printf("%s", line);

    fclose(fp8);

}

int main(){

	pass2();
    show_output();
    show_object_program();

	return 0;
}
