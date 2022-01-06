#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>

int stringthex(char z[4])
{
    z=strrev(z);
    int k=0,hex=0;
    while(z[k]!='\0')
    {
        hex+=(int)(z[k]-48)*(pow(16,k));
        k++;
    }
    z=strrev(z);
    return hex;
}

int main()
{
    FILE *fp1;
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
    }

    //Putting them into 3dimensional array of characters
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
            y++;
            token=strtok(NULL,"\t");
        }
    }

/////////////////////////////////////////////////////////////////////
    FILE *fp2;
    char buffer2[1000];
    char array2[100][100];
    char op[2][100][100];
    int i2=0;
    int q2=0;
    
    //Reading from file to buffer
    fp2=fopen("optab.txt","r");
    while((fscanf(fp2,"%[^\n]%*c",buffer2))!=EOF)
    {
        strcpy(array2[q2],buffer2);
        q2++;
    }
    fclose(fp2);

    int lines2=q2;
    
    //Putting them into 3dimensional array of characters
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

    //Generating intermediate file intermediate file
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


        // Check for assembler directive also
        //      if((flag2==0))
        //        printf("INVALID OPCODE PRESENT\n");
    }

      //  printf("Press Enter to generate outputfile\n");
    fp1=fopen("output.txt","w");

    int z;
    for(z=0;z<lines;z++)
        fprintf(fp1,"%s\t%s\t%s\t%s\n",col[0][z],col[1][z],col[2][z],col[3][z]);
    fclose(fp1);

    fp2=fopen("symtab.txt","w");
    for(z=0;z<symlines;z++)
        fprintf(fp1,"%s\t%s\n",sym[0][z],sym[1][z]);

    int length=x-startaddress;
    fclose(fp2);
}
        //  fp1=fopen("Header.txt","w");
        //  fprintf(fp1,"H^%6s^%06x^%06x",col[1][0],startaddress,length);
        //  fclose(fp1);

// int xregister=0;
// char search[10];
// int searchflag=0;
// int last=0;
// int flag3=0;
// int symx;
// int indexmode=0;
// FILE *fp3;

//          for(j=0;j<lines;j++)
//         {
//             flag3=0;
//             for(i=0;i<lines2;i++)
//             {
//                 if(strcmp(col[2][j],op[0][i]) ==0 )
//                 {
//                     printf("%s\t%s\t%s\t%9s\t",col[0][j],col[1][j],col[2][j],col[3][j]);
//                     printf("%s",op[1][i]);

//                    /// Generate opcode
//                     last=strlen(col[3][j]);
//                     indexmode=0;

//                     if(  (col[3][j][last-1]=='X') && (col[3][j][last-2]==',') )
//                     {
//                         strcpy(search,col[3][j]);
//                         search[last-2]='\0';
//                         indexmode=1;
//                     }
//                     else
//                         strcpy(search,col[3][j]);

//                     /////////////////////////////////////////

//              searchflag=0;
//             //    for(symx=0;symx<symlines;symx++)
//             //     {
//             //     if(  strcmp(search,sym[0][symx] )==0)
//             //      {
//             //            if(indexmode==0)
//             //            {

//             //            printf("%s",sym[1][symx]);
//             //            }
//             //            else
//             //            printf("%x",(stringthex(sym[1][symx])+xregister));

//             //            searchflag=1;
//             //      }
//             // }

//             // if(searchflag==0)
//             // printf("     ERROR:LABEL NOT FOUND");


//             //    printf("\n");
//                ///
//             //    flag3=1;
//             }

//             }

//              if(flag3==0)
//              printf("%s\t%s\t%s\t%9s\t\n",col[0][j],col[1][j],col[2][j],col[3][j]);
//             }
//     return(0);
// }


