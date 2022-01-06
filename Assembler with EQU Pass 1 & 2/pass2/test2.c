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
    FILE *fp1,*fp2,*fp3;
    char array[100][100];
    char buffer[100];
    char col[100][4][100];
    int i=0;
    int q=0;

    fp1 = fopen("intermtest.txt","r");
    while((fscanf(fp1,"%[^\n]%*c",buffer))!=EOF)
    {
        strcpy(array[q],buffer);
       // printf("%s\n",buffer);
        q++;
    }
    fclose(fp1);
   
    int lines=q,j=0;

    //Getting how many words in each line
    int linecount[q],count=0;
    for(i=0;i<lines;i++)
    {
        count=1;
        for(j=0;array[i][j]!='\0';j++)
        {
            if(array[i][j]=='\t' && array[i][j+1]=='\t')
                count++;
        }
        linecount[i]=count;
        // printf("%d\n",count);
    }
    
    //3darray
    int y;
    char *tok;
    int ct=0;  //optab
    int flag = 0;
    int flag2 = 0;
    int ct2=0; //symtab
    int k=0;
    char sym[100][2][100];
    char op[100][2][100];

    for(i=0;i<lines;i++)
    {
        if(linecount[i]==1)
            y=0;
        else
            y=1;

        char *token=strtok(array[i],"\t");
        while(token!=NULL)
        {
            strcpy(col[i][y],token);
            // printf("%s %d %d\n",col[i][y],i,y);
            y++;
            token=strtok(NULL,"\t");
        }
    }
   
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
            strcpy(op[ct][i],tok);
            i++;
            tok = strtok(NULL,"\t");
        }
        ct++;
    }
    fclose(fp2);

    fp2=fopen("symtest.txt","r");
    // symtab 3d array
    while ((fscanf(fp2, "%[^\n]%*c", buffer)) != EOF )
	{
		// printf("%s\n",inbuf);
        tok = strtok(buffer," ");
        i=0;
        while (tok!= NULL)
        {
            // printf("words: %s\n",tok);
            strcpy(sym[ct2][i],tok);
            i++;
            tok = strtok(NULL," ");
        }
        ct2++;
    }
    fclose(fp2);


    if(strcmp(col[0][2],"START")==0){
        printf(" %s %s %s\n",col[0][1],col[0][2],col[0][3]);
    }
    //head here
    for(i=1;i<lines-1;i++){
        flag = 0;
        flag2 = 0;
        printf("%s %s %s %s",col[i][0],col[i][1],col[i][2],col[i][3]);
        // if(linecount[i]==1){
            for(j=0;j<ct;j++){
                if(strcmp(col[i][2],op[j][0])==0){
                    printf(" %s",op[j][1]);
                    for(k=0;k<ct2;k++){
                        // printf("\n%s %s\n",col[i][3],sym[k][1]);
                        if(strcmp(col[i][3],sym[k][1])==0){
                            printf("%s\n",sym[k][0]);
                            flag = 1;
                            flag2 = 1;
                            break;
                        }
                    }
                    if(!flag){
                        if(col[i][3][strlen(col[i][3])-1]=='X' && (col[i][3][strlen(col[i][3])-2]==',')){
                            strcpy(buffer,col[i][3]);
                            buffer[strlen(buffer)-1]=0;
                            buffer[strlen(buffer)-1]=0;
                            for(k=0;k<ct2;k++){
                                if(strcmp(buffer,sym[k][1])==0){
                                    printf("%04x\n",stringthex(sym[k][0])+0x8000);
                                    flag = 1;
                                    flag2 = 1;
                                    break;
                                }
                            }
                        }
                        else{
                            printf("0000\n");
                            flag2 = 1;
                        }
                    }
                }
               
            }
            if(strcmp(col[i][2],"BYTE")==0 || strcmp(col[i][2],"WORD")==0){
                char temp[10];
                int p;
                for(p=2;p<(strlen(col[i][3])-1);p++)
                {
                    char tp[10];
                    itoa(col[i][3][p],tp,16);
                    strcat(temp,tp);
                }
                printf("%s\n",temp);
                flag2 = 1;
            }
            if(!flag2)
            {
                printf("\n");
            }
    }
}