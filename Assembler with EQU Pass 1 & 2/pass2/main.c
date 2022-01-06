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
    char buffer[1000];
    char array[100][100];
    char col[4][100][100];
    int i=0;
    int q=0;

    //Reading from file to buffer
    fp1=fopen("input.txt","r");
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
        count=0;
        for(j=0;array[i][j]!='\0';j++)
        {
            if(array[i][j]=='\t')
                count++;
        }
        linecount[i]=count;
        // printf("%d\n",count);
    }

    //3darray
    int y;
    for(i=0;i<lines;i++)
    {
        if(linecount[i]==2)
            y=1;
        else
            y=2;

        char *token=strtok(array[i],"\t");
        while(token!=NULL)
        {
            strcpy(col[y][i],token);
            // printf("%s %d %d\n",col[y][i],y,i);
            y++;
            token=strtok(NULL,"\t");
        }
    }

    
    char buffer2[1000];
    char array2[100][100];
    char op[2][100][100];
    int i2=0;
    int q2=0;
    
    fp2=fopen("optab.txt","r");
    while((fscanf(fp2,"%[^\n]%*c",buffer2))!=EOF)
    {
        strcpy(array2[q2],buffer2);
        q2++;
    }
    fclose(fp2);

    int lines2=q2;
    
    //optab 3darray
    int y2;
    for(i2=0;i2<lines2;i2++)
    { 
        y2=0;
        char *token2=strtok(array2[i2],"\t");
        while(token2!=NULL)
        {
            strcpy(op[y2][i2],token2);
                y2++;
            token2=strtok(NULL,"\t");
        }
    }

    //Generating intermediate file
    int flag2=0;
    char sym[2][100][100];
    int flag=0;
    int symlines=0;
    int x=0;
    int startaddress=0;
    if(strcmp(col[2][0],"START")==0)
    {
        x=stringthex(col[3][0]);
        startaddress=stringthex(col[3][0]);
        sprintf(col[0][0], "%04x", x);
        sprintf(col[0][1], "%04x", x);
    }

    for (j=1;j<lines-1;j++)
    {
        if(flag2==1)
            sprintf(col[0][j], "%04x", x);

        flag=0;
        
        for(i=0;i<symlines;i++)
        {   
            if(strcmp(col[1][j],sym[0][i]) ==0 )
            {
                flag=1; 
                break;
            }
        }

        if(flag==0)
        {
            sprintf(sym[1][symlines], "%04x", x);
            sprintf(sym[0][symlines], "%s",col[1][j]);
            symlines++;
        }

        flag2=0;
        for(i=0;i<lines2;i++)
        {
            if(strcmp(col[2][j],op[0][i]) ==0 )
            {
                x=x+3;
                flag2=1;
            }
        }

        if(strcmp(col[2][j],"WORD")==0 )
        {
            x=x+3;
            flag2=1;
        }
        else if((strcmp(col[2][j],"BYTE")==0) )
        {
            x=x+strlen(col[3][j])-3;
            flag2=1;

        }
        else if(strcmp(col[2][j],"RESB")==0 )
        {
            x=x+(atoi(col[3][j]));
            flag2=1;

        }
        else if(strcmp(col[2][j],"RESW")==0 )
        {
            x=x+3*((atoi(col[3][j])));
            flag2=1;
        }
        //      if((flag2==0))
        //        printf("INVALID OPCODE PRESENT\n");
    }

    fp1=fopen("interm.txt","w");

    int z;
    for(z=0;z<lines;z++)
        fprintf(fp1,"%s\t%s\t%s\t%s\n",col[0][z],col[1][z],col[2][z],col[3][z]);
    fclose(fp1);

    fp2=fopen("symtab.txt","w");
    for(z=0;z<symlines;z++)
        fprintf(fp1,"%s\t%s\n",sym[0][z],sym[1][z]);
    int length=x-startaddress;
    printf("Length is %x\n",length);
    fclose(fp2);

    //PASS2 begin
    fp3=fopen("object.txt","w");
    fprintf(fp3,"H^%6s^%06x^%06x\n",col[1][0],startaddress,length);

    int xregister=0x8000;
    char search[10];
    int searchflag=0;
    int last=0;
    int flag3=0;
    int symx;
    int indexmode=0;
    int byteflag=0;
   
    char textr[100][30];
    fp2=fopen("interm2.txt","w");
    for(j=0;j<lines;j++)
    {
        flag3=0;
        for(i=0;i<lines2;i++)
        {
            //start address generation
            if(strcmp(col[2][j],op[0][i]) ==0 )
            {
                printf("%s\t%s\t%s\t%s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
                fprintf(fp2,"%s\t%s\t%s\t%s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
                printf("%s",op[1][i]);
                fprintf(fp2,"%s",op[1][i]);
                strcpy(textr[j],op[1][i]);

                //For BUFFER,X ==> indexed
                last=strlen(col[3][j]);
                indexmode=0;

               
                if((col[3][j][last-1]=='X') && (col[3][j][last-2]==',') )
                {
                    strcpy(search,col[3][j]);
                    search[last-2]='\0';
                    indexmode=1;
                }
                //Search SYMTAB
                else
                    strcpy(search,col[3][j]);
                searchflag=0;

                //start search
                for(symx=0;symx<symlines;symx++)
                {
                    if(strcmp(search,sym[0][symx])==0)
                    {
                        if(indexmode==0)
                        {
                            printf("%s",sym[1][symx]);
                            fprintf(fp2,"%s",sym[1][symx]);                            
                            strcat(textr[j],sym[1][symx]);
                        }
                        
                        else
                        {
                            printf("%x",(stringthex(sym[1][symx])+xregister));
                            fprintf(fp2,"%x",(stringthex(sym[1][symx])+xregister));
                            char tp[10];
                            itoa((stringthex(sym[1][symx])+xregister),tp,16);
                            strcat(textr[j],tp);
                        }
                        searchflag=1;
                    }
                }
                //Label error
                //RDREC 2039 WRREC 2061
                if(searchflag==0)
                {
                    if(strcmp(search,"RDREC")==0)
                    {
                        strcat(textr[j],"2039");
                        printf("2039");
                        fprintf(fp2,"2039");
                    }
                    else if(strcmp(search,"WRREC")==0)
                    {
                        strcat(textr[j],"2061");
                        printf("2061");    
                        fprintf(fp2,"2061");    
                    }
                    else
                    {
                        printf("\tLABEL NOT FOUND");
                        strcat(textr[j],"ERR");
                    }
                }
                printf("\n");
                fprintf(fp2,"\n");
               
                //Not Memory Allocation
                flag3=1;
            }  

        }
        
        //If memory allocation
         if(flag3==0)
            //If encountered 'BYTE'
            if(strcmp(col[2][j],"BYTE")==0)
            {
                char temp[10];
                int p;
                printf("%s\t%s\t%s\t%s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
                fprintf(fp2,"%s\t%s\t%s\t%s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
                
                //extract EOF from C'EOF' 
                for(p=2;p<(strlen(col[3][j])-1);p++)
                {
                    char tp[10];
                    itoa(col[3][j][p],tp,16);
                    strcat(temp,tp);
                }
                printf("%s\n",temp);
                fprintf(fp2,"%s\n",temp);
                strcpy(textr[j],temp);
            }
            //If Encountered "WORD"
            else if(strcmp(col[2][j],"WORD")==0)
            {
                int tp;
                char g[10];
                printf("%s\t%s\t%s\t%s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
                fprintf(fp2,"%s\t%s\t%s\t%s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
                itoa((int)col[3][j],g,16);
                printf("%s\n",g);
                fprintf(fp2,"%s\n",g);
                strcpy(textr[j],g);
            }
            else
            {
                printf("%s\t\t%s\t%s\t%s\t\n",col[0][j],col[1][j],col[2][j],col[3][j]);
                fprintf(fp2,"%s\t\t%s\t%s\t%s\t\n",col[0][j],col[1][j],col[2][j],col[3][j]);
            }
    }
    fclose(fp2);

    //Text Record
    int split=0;
    int lenflag=0;
    int reclength=4; //batch size
    int ct=0; 
    int tc=1; //text record count
    
    //Get Count of TA's 
    for(i=0;i<lines2;i++)
    {
        if(strcmp(textr[i],"")!=0)    
            ct++;
    }

    //Initialize Text record
    printf("T^%06s^%02X",col[0][0],reclength*3);
    fprintf(fp3,"T^%06s^%02X",col[0][0],reclength*3);  
    
    //Complete Text Record
    for(i=0;i<lines2;i++)
    {
        if(strcmp(textr[i],"")!=0)
        {
            printf("^%s",textr[i]);
            fprintf(fp3,"^%s",textr[i]);
            split+=1;

            if(split%reclength==0){
                printf("\nT^");
                fprintf(fp3,"\nT^");
                lenflag=1;
                tc++;
            }
            
        }
        if(lenflag)
        {
            if(tc>(ct/reclength))
            {
                printf("%06s^%02X",col[0][i+1],(ct%reclength)*3);
                fprintf(fp3,"%06s^%02X",col[0][i+1],(ct%reclength)*3);
            }
            else
            {
                printf("%06s^%02X",col[0][i+1],reclength*3);
                fprintf(fp3,"%06s^%02X",col[0][i+1],reclength*3);
            }
            lenflag=0;
        }
        
    }

    //End Record
    printf("\nE^%06s",col[0][0]);
    fprintf(fp3,"\nE^%06s",col[0][0]);
    fclose(fp3);
    return(0);
}


            