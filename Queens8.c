// RecursiveEightQueens

/*
   places 8 queens on chessboard so that they are mutually unthreatening.
   allows user to place the 1st queen; then the program places the rest of
   the queens by a recursive backtracking algorithm. prints out all the
   moves, including the moves that may be "taken back" as well as the
   moves that "take back". algorithm, due to Dijkstra and Wirth, is justly
   famous for its elegant method of keeping track of threatened squares.

   This elegance has been missing in the C implementation of Queens8
   until now. By doing some pointer Math with Macros to hide the ugly-
   ness, we have arrays with indexes from [1..8] [2..16] and [-7..7]
*/

#include <stdio.h>
#include <stdlib.h>

#define ARRAYSZ(ll,ul) ((ul)-(ll)+1)
#define ARRAYFIX(b, ll, ul) (b - ll)

/*
TYPE
   INDEX = 1..8;       {FOR INDEXING ROWS AND COLUMNS OF CHESSBOARD}
   VALS = 0..8;
*/

enum {FALSE, TRUE};

short j;    //      for indexing updiagonalsafe and downdiagonalsafe
short ctr;  //      for counting placements and removals
int i;      //      for reading row for 1st queen placements


unsigned char *rows;            //Array[1..8] of 1..8
unsigned char *rowsafe;         //Array[1..8] of boolean values
unsigned char *updiagonalsafe;  //Array[2..16] of boolean values index=r+c
unsigned char *downdiagonalsafe;//Array[-7..7] of boolean values index=r-c
unsigned char rawrows[ARRAYSZ(1,8)];
unsigned char rawrowsafe[ARRAYSZ(1,8)];
unsigned char rawupdiagonalsafe[ARRAYSZ(2,16)];
unsigned char rawdowndiagonalsafe[ARRAYSZ(-7,7)];

short   queensplaced; // 0..8

void PlaceQueenOn (short r, short c)
{//PlaceQueenOn
    ctr++;
    printf("\n%3d: Place Queen %1d on Row %1d", ctr, c, r);
    queensplaced++;
    rowsafe[r]            = FALSE;
    updiagonalsafe[r+c]   = FALSE;
    downdiagonalsafe[r-c] = FALSE;
    rows[c] = r;
}//PlaceQueenOn

void TakeQueen (short r, short c) //from [r,c]
{//TakeQueen
    ctr++;
    printf("\n%3d: Take Queen %1d from Row %1d",ctr, c, r);
    queensplaced--;
    rowsafe[r]            = TRUE;
    updiagonalsafe[r+c]   = TRUE;
    downdiagonalsafe[r-c] = TRUE;
    rows[c]               = 0;
}//TakeQueen

void PlaceRestOfQueens (void)
{//PlaceRestOfQueens
    short r;
    short c;
    c = queensplaced + 1;
    for(r = 1; r <= 8; r++)
    {//for r
       if(rowsafe[r] && updiagonalsafe[r+c] && downdiagonalsafe[r-c])
        {
            PlaceQueenOn(r,c);
            if(queensplaced < 8)
                PlaceRestOfQueens();
            if(8 == queensplaced)
                break;
            TakeQueen(r, c);//back track remove queen from [r,c]
        }
    }//for r
}//PlaceRestOfQueens

void Initialize (void)
{//Initialize
    short r, c;
    queensplaced = 0;
    ctr = 0;
    for(r = 1; r <= 8; r++)
    {// for r
        rowsafe[r] = TRUE;
        rows[r] = 0;
        for(c = 1; c <= 8; c++)
        {//for c
            updiagonalsafe[r+c]   = TRUE;
            downdiagonalsafe[r-c] = TRUE;
        }//for c
    }// for r
}//Initialize

int main (void)
{//main
    rows             = ARRAYFIX(rawrows, 1, 8);
    rowsafe          = ARRAYFIX(rawrowsafe,1,8);
    updiagonalsafe   = ARRAYFIX(rawupdiagonalsafe,2,16);
    downdiagonalsafe = ARRAYFIX(rawdowndiagonalsafe,-7,7);
    while(TRUE)
    {//We look till you give up
        Initialize();
        printf("\nOn which row do you want Queen 1?");
        printf("\nEnter Integer in the range 1..8 to continue.");
        printf("\nEnter any other Integer to halt.:");
        scanf("%d", &i);
        if((1 <= i) && (i <= 8))
        {//i is in range
            PlaceQueenOn(i, 1);
            PlaceRestOfQueens();
            if(8 == queensplaced)
            {//All Queens placed
                printf("\n");
                for(i = 1; i <= 8; i++)
                    printf("%3d", rows[i]);
                printf("\n");
            }//All Queens placed
        }//i is in range
    else
    {// let's free space to show how to. Not really needed
        free(rows+1);
        free(rowsafe+1);
        free(updiagonalsafe+2);
        free(downdiagonalsafe-7);
        exit(0);
    }
    }//We look till you give up
}//main
