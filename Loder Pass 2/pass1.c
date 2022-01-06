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

    char arr[100][50][10];
    int lines[100];


int get_oc(int abc,int i_val)
{
    int i,j,diff,oc,count;
    for(i=i_val-1;i>0;i--)
    {
        if(arr[i][0][0] == 'T')
        {
            diff = abs(abc-stringthex(arr[i][1]));
            if(diff > 30)
                continue;
            else
            {
                count=(diff/3)+2;
                return(stringthex(arr[i][count]));
            }
        }
    }
}



int main()
{
    FILE *fin,*fout;
    char inbuf[200];    
    int progaddr, csaddr, cslth, i, j;
    int ct = 0;
    char *tok;
    
    char estab[100][2][10];
    //To get no. of tokens in each line

    fin = fopen("input.txt","r");

    //Populate Array
    while ((fscanf(fin, "%[^\n]%*c", inbuf)) != EOF )
	{
		// printf("%s\n",inbuf);
        tok = strtok(inbuf," ");
        i=0;
        while (tok!= NULL)
        {
            // printf("words: %s\n",tok);
            strcpy(arr[ct][i],tok);
            i++;
            tok = strtok(NULL," ");
        }
        lines[ct] = i;
        ct++;
    }
    fclose(fin);
    
    printf("Enter Starting Address: \n");
    scanf("%x", &progaddr);

    //set CSADDR to PROGADDR {for first control section}
    csaddr = progaddr;
    
    printf("\n==============\nPASS 1 Begins\n==============\n");
    
    fout = fopen("estab.txt","w");
    printf("\nAdding to ESTAB:\n\n");

    for(i=0;i<ct;i++)
    { 
        //if head record 
        if(arr[i][0][0] == 'H')
        {
            //set CSLTH to control section length 
            cslth = stringthex(arr[i][lines[i]-1]);

            //Display result
            printf("%s\t%04X\t%x\n",arr[i][1],csaddr,cslth);
            fprintf(fout,"%s %04X\n",arr[i][1],csaddr);

        }

        //if Define record
        else if(arr[i][0][0] == 'D')
        {
            //For every symbol and address in the rec
            for(j=1;j<lines[i];j++)   
                //Even is addr, Odd is symbol
                if(j % 2 == 1)
                {
                    printf("%s\t",arr[i][j]);
                    fprintf(fout,"%s ",arr[i][j]);
                    // strcpy(estab[i][0],arr[i][j]);
                }
                else
                {
                    //enter symbol with value(CSADDR + indicated address) 
                    printf("%04X\n",csaddr+stringthex(arr[i][j]));
                    fprintf(fout,"%04X\n",csaddr+stringthex(arr[i][j]));
                    // strcpy(estab[i][1],csaddr+stringthex(arr[i][j]));
                }
        }

        //If end of CS is reached (Note: not end of file. E & END are different )
        else if(strcmp(arr[i][0],"E") == 0)
        {
            //add CSLTH to CSADDR {starting address for next control section} 
            csaddr += cslth;
        }
    }
    fclose(fout);

    //Pass 2
    printf("\n==============\nPASS 2 Begins\n==============\n");
    int exeaddr;
    csaddr = progaddr;
    exeaddr = progaddr;
    
    int ct2 = 0;
    fin = fopen("estab.txt","r");
    
    //estab populate
    while ((fscanf(fin, "%[^\n]%*c", inbuf)) != EOF )
	{
		// printf("%s\n",inbuf);
        tok = strtok(inbuf," ");
        i=0;
        while (tok!= NULL)
        {
            // printf("words: %s\n",tok);
            strcpy(estab[ct2][i],tok);
            i++;
            tok = strtok(NULL," ");
        }
        ct2++;
    }
    for(i=0;i<ct;i++)
    { 
        //if head record
        if(arr[i][0][0] == 'H')
        {
            //set CSLTH to control section length 
            cslth = stringthex(arr[i][lines[i]-1]);            \
        }

        //if Text record
        else if(arr[i][0][0] == 'T')
        {
            int addr =0;
            for(j=2;j<lines[i];j++)
            {
                printf("(%06s) moved to (%06X)\n",arr[i][j],csaddr+stringthex(arr[i][1])+addr);
                addr+=3;
            }
        }

        else if(arr[i][0][0] == 'M')
        {
            int k=0;
            memmove(inbuf, arr[i][lines[i]-1]+1, strlen(arr[i][lines[i]-1]));
            for(k=0;k<ct2;k++)
            {
                if(strcmp(estab[k][0],inbuf)==0)
                {
                    if(arr[i][lines[i]-1][0] == '+'){
                        int oc_val=get_oc(stringthex(arr[i][1]),i);
                        int obc =oc_val+stringthex(estab[k][1]);
                        printf("Object code %06X is been changed to %06X and moved to location %06X\n",oc_val,(obc-oc_val)+oc_val,obc-oc_val);
                    }
                    else{
                        int oc_val=get_oc(stringthex(arr[i][1]),i);
                        int obc =oc_val+stringthex(estab[k][1]);
                        printf("Object code %06X is been changed to %06X and moved to location %06X\n",oc_val,oc_val-(obc-oc_val),obc-oc_val);

                    }
                }
            }
        }
        else if(strcmp(arr[i][0],"E") == 0)
        {
            if(strcmp(arr[i][1],"") != 0)
                exeaddr = csaddr + stringthex(arr[i][1]);
            //add CSLTH to CSADDR {starting address for next control section} 
            csaddr += cslth;
        }
    }
    printf("Jumping to %06X for exec",exeaddr);
    return 0;
}


