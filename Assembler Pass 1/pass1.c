#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct data
{
    char label[100];
    char opcode[100];
    char operand[100];
};

struct str 
{
    char *stt; 
};

struct opc
{
    char op[100];
    char hex[3];
};

int hextodec(char hex[100])
{
    long int decimal=0,i,base=1;
    int length=strlen(hex);
    for(i = length--; i >= 0; i--)
    {
        if(hex[i] >= '0' && hex[i] <= '9')
        {
            decimal += (hex[i] - 48) * base;
            base *= 16;
        }
        else if(hex[i] >= 'A' && hex[i] <= 'F')
        {
            decimal += (hex[i] - 55) * base;
            base *= 16;
        }
        else if(hex[i] >= 'a' && hex[i] <= 'f')
        {
            decimal += (hex[i] - 87) * base;
            base *= 16;
        }
    }
    return decimal;
}
int main()
{   printf("\n");
    printf("PASS 1 EVALUATION:\n");
    printf("\n\n");
    FILE *fp,*fp1,*fp2,*fp3,*fp4;
    int locctr=0,start,programlength,lc=0,oc=0,value,j;     
    char word[100],l[100];
    char delimeter[]=" \t\n";
    struct str sample[200];
    struct data list[200];
    struct opc opdata[200];

    fp=fopen("optab.txt","r");
    if(fp==NULL)
    {
        printf("FILE OPEN FAILED!\n");
        printf("------------------------\n");
    }
    else
    {
        printf("FILE SUCCESSFULLY OPENED!  (OPTAB) \n");
        printf("------------------------\n");
    }
// tokenize optab
    while(fgets(word,30,fp)!=NULL)
    {
        oc++;
        int count1=0;
        char *token=strtok(word,delimeter);
        while(token!= NULL)
        {
            count1++;
            sample[count1].stt=token;
            token = strtok(NULL,delimeter);
        }
        if(count1==2)
        {
            strcpy(opdata[oc].op,sample[1].stt);
            strcpy(opdata[oc].hex,sample[2].stt);
        }
        printf("%s\t%s\n",opdata[oc].op,opdata[oc].hex);
    }
    printf("------------------------\n");
    printf("\n");
    printf("\n------------------------\n");
    fp2=fopen("intermediate.txt","w");
    fp3=fopen("SYMTAB.txt","w");
    fp1=fopen("SIC_input.txt","r");
    if(fp1==NULL)
    {
        printf("FILE OPEN FAILED!\n");
    }
    else
    {
        printf("FILE SUCCESSFULLY OPENED! (SIC INPUT FILE)\n");
    }

    while(fgets(word,30,fp1)!=NULL)
    {
        lc++;
        int count2=0;
        char *token=strtok(word,delimeter);
        while(token!=NULL)
        {
            count2++;
            sample[count2].stt=token;
            token = strtok(NULL,delimeter);
        }
        if(count2==3)
        {
            strcpy(list[lc].label,sample[1].stt);
            strcpy(list[lc].opcode,sample[2].stt);
            strcpy(list[lc].operand,sample[3].stt);
        }
        if(count2==2)
        {
            strcpy(list[lc].opcode,sample[1].stt);
            strcpy(list[lc].operand,sample[2].stt);
        }
        if(count2==1)
        {
            strcpy(list[lc].opcode,sample[1].stt);
        }
    }

    int c=1;
    while(c<lc)
    {
        if(!strcmp(list[c].opcode,"START"))
        {
            locctr=hextodec(list[c].operand);
            start=hextodec(list[c].operand);
            printf("%x\t%s\t%s\t%s\n\n",locctr,list[c].label,list[c].opcode,list[c].operand);
            fprintf(fp2,"%x\t%s\t%s\t%s\n",locctr,list[c].label,list[c].opcode,list[c].operand);
            c++;
        }
        if(strcmp(list[c].opcode,"END"))
        {
            printf("%x\t%s\t%s\t%s\n\n",locctr,list[c].label,list[c].opcode,list[c].operand);
            fprintf(fp2,"%x\t%s\t%s\t%s\n",locctr,list[c].label,list[c].opcode,list[c].operand);
            if(strcmp(list[c].label,""))
            {
                //printf("\n%s\t%x\n",list[c].label,locctr);
                // if(list[c].opcode == "MULF")
                // break;
                fprintf(fp3,"%s\t%x\n",list[c].label,locctr);
            }
            for( j=0; j<=oc; j++)
            {
                if(!strcmp(list[c].opcode,opdata[j].op))
                {
                    locctr=locctr+3;
                }
            }
            if(!strcmp(list[c].opcode,"RESW"))
            {
                value=atoi(list[c].operand);
                locctr=locctr+(value*3);
            }
            else if(!strcmp(list[c].opcode,"RESB"))
            {
                value=atoi(list[c].operand);
                locctr=locctr+value;
            }
            else if(!strcmp(list[c].opcode,"BYTE"))
            {
                if(list[c].operand[0]=='C')
                {
                    value=strlen(list[c].operand);
                    locctr=locctr+(value-3);
                }
                if(list[c].operand[0]=='X')
                {
                    value=strlen(list[c].operand);
                    value=value-3;
                    locctr=locctr+(value/2);
                }
            }
            else if(!strcmp(list[c].opcode,"WORD"))
            {
                locctr=locctr+3;
            }
            else
            {
                locctr=locctr+0;
            }
            c++;
        }
    }

    printf("%x\t%s\t%s\t%s\n\n",locctr,list[lc].label,list[lc].opcode,list[lc].operand);
    fprintf(fp2,"%x\t%s\t%s\t%s\n",locctr,list[lc].label,list[lc].opcode,list[lc].operand);

    programlength=locctr-start;
    printf("\n------------------------\n");
    printf("\nProgram length = %x",programlength);
    

    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fp4=fopen("Length.txt","w");
    printf("\n------------->%x\n",programlength);
    fprintf(fp4,"%x",programlength);
    fclose(fp4);


    printf("\n------------------------\n");
    return 0;

}

