// mcpexergen.c Just do some random exersizes
// we ask for the chars, the group size, number of groups and output file name

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

int nGroupSize, nNoOfGroups;
char letters[200];
char nletters;
char outFileName[200];
FILE *ofile;

void Generate (void)
{//Generate
    int nGroupCount;
	int nInGroup;
    int GrpPerLine;
	int nRandLetterNo;
    GrpPerLine = (((nGroupSize + 1) * nNoOfGroups) + 79) / 80;
	GrpPerLine = nNoOfGroups / GrpPerLine;
    for(nGroupCount = 0; nGroupCount < nNoOfGroups; nGroupCount++)
    {//Many random numbers
        if((nGroupCount != 0) && ((nGroupCount % GrpPerLine) == 0))
            fprintf(ofile, "\n");
        for(nInGroup = 0; nInGroup < nGroupSize; nInGroup++)
		{// a group of letters
			nRandLetterNo = rand() % nletters;
			fprintf(ofile, "%c", letters[nRandLetterNo]);
        }// a group of letters
        if(((nGroupCount+1) % GrpPerLine) != 0)
            fprintf(ofile, " ");
    }//Many random numbers
    fprintf(ofile, "\n");
}//Generate

void Process (void)
{//Process
//
// Seed the random number generator with time. Comment out for debug.
//
    srand(time(NULL));
    ofile = fopen(outFileName, "w");
    if(NULL == ofile)
    {//can't open
        printf("\nCan't open %s!\n", outFileName);
        exit(999);
    }//can't open
    Generate();
    fclose(ofile);
}//Process

unsigned char Yorn (char *msg)  // Asks a question & only take yes/no replys
{//Yorn
	char what;
	while(1)
	{//keep at it till we see [YyNn]
		printf("\n%s", msg);
		what = toupper(getch());        // y or n in any case
		if('Y' == what)
			break;
		if('N' == what)
			break;
		printf("\tResponse must be 'Y' or 'N'"); // bad reply
	}//keep at it till we see [YyNn]
	printf("%c\n", what);             // show what the reply was
	return ('Y' == what);   // return boolean true if yes, false if no.
}//Yorn

void main (void)
{//main
    int i;
    char ch;
    printf("\ncharacters for exercize:");
    gets(letters);
    nletters = strlen(letters);
    for(i = 0; i < nletters; i++)
        letters[i] = toupper(letters[i]);
    printf("\nGroupSize:");
    scanf("%d", &nGroupSize);
    printf("\nNo of groups to generate:");
    scanf("%d", &nNoOfGroups);
    printf("\nOutput File Name:");
    scanf("%s", outFileName);
// show what
    printf("\ncharacters   = %s", letters);
    printf("\nNo of chars  = %d", nletters);
    printf("\nGroup size   = %d", nGroupSize);
    printf("\nNo of Groups = %d", nNoOfGroups);
    printf("\nout file     = %s", outFileName);
// Make sure we want to launch
	if(Yorn("Start generation?"))
		Process();
    printf("\nComplete!!!\n");
}//main
