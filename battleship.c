#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = false; //causes dump of computer board before game if true (for debugging)

// boards. 0 means empty, non-zero means ship is there, number is size, -tive means hit on a ship
int playerBoard[BOARDROWS+1][BOARDCOLS+1];
int computerBoard[BOARDROWS+1][BOARDCOLS+1];
int compShotBoard[BOARDROWS+1][BOARDCOLS+1];

int getRand(int lowval,int highval){
    return (rand()%(highval+1-lowval) + lowval);
}


int getShot(bool fromUser, int board[BOARDROWS+1][BOARDCOLS+1]){
    int row,col;

    while(1){
        if(fromUser){
            printf("Give a shot (row, col):");
            scanf("%d%d",&row,&col);
            if(col<1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
                printf("Invalid input\n");
            else  //valid input
                break;
        }
        else { //computer generated
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) { //make sure haven't shot here before
                compShotBoard[row][col]=1; //valid shot
                break;
            }
        }
    } //will leave this loop with valid input
    if(board[row][col] != 0){
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; //make sure is -tive
        return -1*board[row][col]; //a hit!
    }
    return 0; //miss
}


void populateBoard(bool getUserInput, int board[BOARDROWS+1][BOARDCOLS+1])
{
    int shipSize = 5,row,col,orientation;
    if(getUserInput==true)
    {
      printf("Rows are 1 - 9, Columns are 1 - 11\nOrientation is 0 for across, 1 for down\n");
      while(shipSize>0)
      {
        bool shipPlaced=true;
        printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", shipSize);
        scanf("%d %d %d", &row, &col, &orientation);
        while (row<1 || row>9 || col<1 || col>11 || (orientation!=0 && orientation!=1))
        {
          printf("Invalid Input\nGive starting row, starting column and orientation (3 inputs) for ship of size=%d:", shipSize);
          scanf("%d %d %d", &row, &col, &orientation);
        }
        if(orientation==1)
        {
          if(row+shipSize-1>9)
          {
            printf("Invalid Input\n");
            shipPlaced=false;
          }
          for(int i=row; i<row+shipSize && shipPlaced==true; i++)
          {
            if(board[i][col]!=0)
            {
              shipPlaced=false;
              printf("Conflicts with ship already placed\n");
            }
          }
          if(shipPlaced==true)
          {
            for(int i=row; i<row+shipSize;i++)
              board[i][col]=shipSize;
            shipSize--;
          }
        }
        else
        {
          if(col+shipSize-1>11)
          {
            printf("Invalid Input\n");
            shipPlaced=false;
          }
          for(int j=col;j<col+shipSize &&shipPlaced==true; j++)
          {
            if(board[row][j]!=0)
            {
              printf("Conflicts with ship already placed\n");
              shipPlaced=false;
            }
          }
          if(shipPlaced==true)
          {
            for(int j=col;j<col+shipSize; j++)
              board[row][j]=shipSize;
            shipSize--;
          }
        }
      }
    }
    else
    {
      while(shipSize>0)
      {
        bool shipPlaced=true;
        row = getRand(1,9);
        col = getRand(1,11);
        orientation = getRand(0,1);
        if(orientation==1)
        {
          if(row+shipSize-1>9)
            shipPlaced=false;
          for(int i=row;i<row+shipSize &&shipPlaced==true; i++)
          {
            if(board[i][col]!=0)
              shipPlaced=false;
          }
          if(shipPlaced==true)
          {
            for(int i=row;i<row+shipSize;i++)
              board[i][col]=shipSize;
            shipSize--;
          }
        }
        else
        {
          if(col+shipSize-1>11)
            shipPlaced=false;
          for(int j=col; j<col+shipSize &&shipPlaced==true; j++)
          {
            if(board[row][j]!=0)
              shipPlaced=false;
          }
          if(shipPlaced==true)
          {
            for(int j=col; j<col+shipSize;j++)
              board[row][j]=shipSize;
            shipSize--;
          }
        }
      }
    }
}

bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) //ship and not hit
                return(true);
        }
    }
    return (false); //no ships found not all hit
}

bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) //ship and not hit
                return(false);
        }
    }
    return (true); //no ships found, all hit
}


void showBoard(int board[BOARDROWS+1][BOARDCOLS+1]){
    printf("   Cols\n");
    printf("    1  2  3  4  5  6  7  8  9 10 11\n");
    printf("     ________________________________\n");
    for(int i=1;i<=BOARDROWS;i++){
        printf("R%d|",i);
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>=0)
                printf(" ");
            printf("%d ",board[i][j]);
        }
        printf("|\n");
    }
    printf("  __________________________________\n");
}

int main(int argc, char **argv)
{
    bool playerBoardOK,computerBoardOK;

    // initialize the seed randomly.

     	srand(time(NULL));

    //init the boards
    for(int i=0;i<BOARDROWS;i++){
        for(int j=0;j<BOARDCOLS;j++){
            playerBoard[i][j]=0;
            computerBoard[i][j]=0;
            compShotBoard[i][j]=0;
        }
    }

    populateBoard(true,playerBoard);
    populateBoard(false,computerBoard);

    printf("Your board is\n");
    showBoard(playerBoard);
    if(dumpComputer){
        printf("\n\nComputer board is\n");
        showBoard(computerBoard);
    }

    // play starts here
    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); //shot from user
        if(hit){
            printf("HIT on size %d\n",hit);
            if(noneLeft(hit,computerBoard))
                printf("Sunk computer's %d\n",hit);
        }
        else
            printf("MISS!!\n");
        hit=getShot(false,playerBoard);
        if(hit){
            printf("Computer hits! Size=%d\n", hit);
            if(noneLeft(hit,playerBoard))
                printf("Sunk your %d\n",hit);
        }
        else
            printf("Computer miss!\n");
        playerBoardOK = allShipsNotHit(playerBoard);
        computerBoardOK = allShipsNotHit(computerBoard);
      //showBoard(playerBoard); //debug
      //showBoard(computerBoard); //debug
   }
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

	return 0;
}
