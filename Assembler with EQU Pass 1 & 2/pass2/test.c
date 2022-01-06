#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int stringthex(char z[4])
{
    z=strrev(z);
    int k=0,hex=0;
    while(z[k]!='\0')
    {
        if(z[k]=='A'||z[k]=='B'||z[k]=='C'||z[k]=='D'||z[k]=='E'||z[k]=='F')
            hex+=(int)(z[k]-55)*(pow(16,k));
        else if(z[k]=='a'||z[k]=='b'||z[k]=='c'||z[k]=='d'|z[k]=='e'||z[k]=='f') 
              hex+=(int)(z[k]-87)*(pow(16,k));
        else
            hex+=(int)(z[k]-48)*(pow(16,k));
        k++;
    }
    z=strrev(z);
    return hex;
}

int main()
{
    FILE *fp1,*fp2;
    char buffer[100];
    char array[100][100];
    char col[100][3][100];
    char* tok;
    int i=0;
    int j=0;
    int y;
    int lines[100]; 
    int ct = 0;     //col count
    char op[100][2][100];
    int ct2=0; //optab count
    int flag=0;
    int startaddress=0;
    int locctr = 0;

    //Reading from file to buffer and get counts & 3darray
    fp1=fopen("test.txt","r");   

    while((fscanf(fp1,"%[^\n]%*c",buffer))!=EOF)
    {
        strcpy(array[ct],buffer);
        i=0;
        tok = strtok(buffer,"\t");
        while(tok!=NULL){
            i++;
            tok = strtok(NULL,"\t");
        }
        lines[ct]=i;
        if(lines[ct]==3)
            y=0;
        else
            y=1;
        tok =strtok(array[ct],"\t");
        while(tok!=NULL)
        {
            strcpy(col[ct][y],tok);
            // printf("%s %d %d\n",col[ct][y],ct,y);
            y++;
            tok = strtok(NULL,"\t");
        }
        // printf("%d\n",lines[ct]);
        ct++;
    }
    fclose(fp1);
 
    fp2=fopen("optab.txt","r");
    // optab 3d array
    while ((fscanf(fp2, "%[^\n]%*c", buffer)) != EOF )
	{
		// printf("%s\n",inbuf);
        tok = strtok(buffer,"\t");
        i=0;
        while (tok!= NULL)
        {
            // printf("words: %s\n",tok);
            strcpy(op[ct2][i],tok);
            i++;
            tok = strtok(NULL,"\t");
        }
        ct2++;
    }
    fclose(fp2);

    //Generating intermediate file
    fp2 = fopen("intermtest.txt","w");

    if(strcmp(col[0][1],"START")==0)
    {
        startaddress=stringthex(col[0][2]);
        locctr = startaddress;
        printf("%s\t%s\t%s\n",col[0][0],col[0][1],col[0][2]);
        fprintf(fp2,"%s\t%s\t%s\n",col[0][0],col[0][1],col[0][2]);
    }

    fp1 = fopen("symtest.txt","w");
  
    for (i=1;i<ct-1;i++){
        flag = 0;
        //symtab entry
        if(lines[i] == 3){
            fprintf(fp1,"%04x %s\n",locctr,col[i][0]);
            // printf("sym %04x %s\n",locctr,col[i][0]);
        }
        //optab search
        for(j=0;j<ct2;j++){
            if(strcmp(col[i][1],op[j][0])==0){
               printf("%04x",locctr);
               fprintf(fp2,"%04x",locctr);
                locctr += 3;
                flag = 1;
                break;
            }
        }
        
        if(flag==0){

            if(strcmp(col[i][1],"EQU")!=0){
                printf("%04x",locctr);
                fprintf(fp2,"%04x",locctr);
            }
            if(strcmp(col[i][1],"WORD")==0)
                locctr+=3;
            else if(strcmp(col[i][1],"RESW")==0){
                locctr+=3*atoi(col[i][2]);
            }
            else if(strcmp(col[i][1],"RESB")==0){
                locctr+=atoi(col[i][2]);
            }
            else if(strcmp(col[i][1],"BYTE")==0){
                locctr+=strlen(col[i][2])-3;
            }
        }
        printf("\t%s\t%s\t%s\n",col[i][0],col[i][1],col[i][2]);
        fprintf(fp2,"\t%s\t%s\t%s\n",col[i][0],col[i][1],col[i][2]);
    }

    printf("\t\t%s\n",col[ct-1][1]);
    fprintf(fp2,"\t\t%s\n",col[ct-1][1]);
    printf("Length : %04x\n",locctr-startaddress);
    fclose(fp1);
    fclose(fp2);
}