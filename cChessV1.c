/*
 * Tyler Ginn
 * Text based Chess V2
 * Start Date: 8/21/23	End Date: __/__/__
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BOARDSIZE 8

#define WHITE 'w'
#define BLACK 'b'
#define STALE 's'

#define EMPTY 'E'
#define PAWN 'P'
#define ROOK 'R'
#define HORSE 'H'
#define BISHOP 'B'
#define QUEEN 'Q'
#define KING 'K'

struct Piece
{
   char color;
   char type;
   char moved;
};

struct Game
{
   struct Piece** board;
   char checked;//not used
   char win;//havent implemented
};

struct Game* createGame()
{
   struct Game* newGame=(struct Game*)malloc(sizeof(struct Game));
   newGame->board=malloc(BOARDSIZE*sizeof(struct Piece*));
   for(int i=0;i<BOARDSIZE;i++)
      newGame->board[i]=malloc(BOARDSIZE*sizeof(struct Piece));
   newGame->win=EMPTY;
   newGame->checked=EMPTY;
   return newGame;
}

void destroyGame(struct Game* oldGame)
{
   for(int i=0;i<BOARDSIZE;i++)
      free(oldGame->board[i]);
   free(oldGame->board);
   free(oldGame);
}

void setPiece(struct Game* gameBoard, int row, int col, char newColor, char newType, char newMove)
{
   gameBoard->board[row][col].color=newColor;
   gameBoard->board[row][col].type=newType;
   gameBoard->board[row][col].moved=newMove;
}

void movePiece(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{
   gameBoard->board[newRow][newCol].type=gameBoard->board[ogRow][ogCol].type;
   gameBoard->board[newRow][newCol].color=gameBoard->board[ogRow][ogCol].color;
   gameBoard->board[newRow][newCol].moved=TRUE;

   gameBoard->board[ogRow][ogCol].type=EMPTY;
   gameBoard->board[ogRow][ogCol].color=EMPTY;
   gameBoard->board[ogRow][ogCol].moved=TRUE;

}

void setupBoard(struct Game* newGame)
{
   int row, col;
   char color;
   for(row=0;row<BOARDSIZE;row++)
   {
      color=WHITE;
      switch(row){//magic number land
      case 0: //black
         color=BLACK;
      case 7: //white
         setPiece(newGame,row,0,color,ROOK,FALSE);
         setPiece(newGame,row,1,color,HORSE,FALSE);
         setPiece(newGame,row,2,color,BISHOP,FALSE);
         setPiece(newGame,row,3,color,QUEEN,FALSE);
         setPiece(newGame,row,4,color,KING,FALSE);
         setPiece(newGame,row,5,color,BISHOP,FALSE);
         setPiece(newGame,row,6,color,HORSE,FALSE);
         setPiece(newGame,row,7,color,ROOK,FALSE);
         break;
      case 1: //black pawn
         color=BLACK;
      case 6: //white pawn
         for(col=0;col<BOARDSIZE;col++)
            setPiece(newGame,row,col,color,PAWN,FALSE);
         break;
      default: //empty
         for(col=0;col<BOARDSIZE;col++)
            setPiece(newGame,row,col,EMPTY,EMPTY,TRUE);
         break;
      }
   }
}

void printBoard(struct Game *gameBoard)
{
   int row,col=0;
   printf("\n |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |");
   for(row=0;row<BOARDSIZE;row++)
   {
      printf("\n---------------------------------------------------");
      printf("\n%d",(BOARDSIZE-row));
      for(col=0;col<BOARDSIZE;col++)
      {
         if(gameBoard->board[row][col].type!=EMPTY)
            printf("| %c %c ",gameBoard->board[row][col].color,gameBoard->board[row][col].type);
         else
            printf("|     ");
      }
      printf("|%d",(BOARDSIZE-row));
   }
   printf("\n---------------------------------------------------");
   printf("\n |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |");
}

//does check Detection on all king possible positions
//done after move completes
char checkmateDetection()
{
   return FALSE;
}


//creates a copy of the board where the move is successful and checks if this move leaves your king open
//it does this by checks all pieces move on the king
//this is not optimal but it will work :(
//done before moving piece and after for checkmate
char checkDetection()
{
   return FALSE;
}

//used by rook bishop and queen for making sure path has no pieces in way
char clearPath(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol, int xDir, int yDir)
{
   int i, row, col;
   row=ogRow;
   col=ogCol;
   for(i=0; i<BOARDSIZE; i++)//fix length
   {
      row+=xDir;
      col+=yDir;
      if(row<0||row>BOARDSIZE-1||col<0||col>BOARDSIZE-1)
      {
         return FALSE;
      }
      if(row==newRow&&col==newCol&&gameBoard->board[row][col].color!=playerColor)
      {
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
      else if(gameBoard->board[row][col].type!=EMPTY)
      {
         return FALSE;
      }
   }
   return FALSE;
}

char pawnMoveCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{//works correctly minus queening
   int direction;
   if(playerColor==WHITE)
      direction=-1;
   else
      direction=1;
   if(ogCol==newCol)//forward
   {
      if(newRow==ogRow+(direction*1)&&gameBoard->board[newRow][newCol].type==EMPTY)//one forward
      {
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
      else if(newRow==ogRow+(direction*2)&&gameBoard->board[newRow][newCol].type==EMPTY&&
      gameBoard->board[ogRow+direction][ogCol].type==EMPTY&&gameBoard->board[ogRow][ogCol].moved==FALSE)//two forward
      {
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
   }
   else if((ogCol==newCol-1||ogCol==newCol+1)&&ogRow+direction==newRow)//left right take piece
   {
      if(gameBoard->board[newRow][newCol].color==EMPTY)
      {//en passant checks
         if(gameBoard->board[ogRow][newCol].type==PAWN&&gameBoard->board[ogRow][newCol].color!=playerColor&&gameBoard->board[newRow][ogCol].type==EMPTY)
         {
            movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
            setPiece(gameBoard,ogRow,newCol,EMPTY,EMPTY,FALSE);//gets rid of enemy pawn
            return TRUE;
         }
      }
      else
      {//pawn takes piece normally
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
   }
   return FALSE;
}

char rookMoveCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{//does not work correctly
   int xDir, yDir;
   if(ogCol-newCol>0)
      yDir=-1;
   else if(ogCol-newCol<0)
      yDir=1;
   else
      yDir=0;
   if(ogRow-newRow>0)
      xDir=-1;
   else if(ogRow-newRow<0)
      xDir=1;
   else
      xDir=0;
   if((xDir!=0&&yDir==0)||(xDir==0&&yDir!=0))
   {
      return clearPath(gameBoard, playerColor, ogRow, ogCol, newRow, newCol, xDir, yDir);
   }
   return FALSE;
}

char horseMoveCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{//works correctly
   if(newRow==ogRow+2||newRow==ogRow-2)//up or down 2
   {
      if(newCol==ogCol+1||newCol==ogCol-1)//left or right 1
      {
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
   }
   else if(newRow==ogRow+1||newRow==ogRow-1)//up or down 1
   {
      if(newCol==ogCol+2||newCol==ogCol-2)//left or right 2
      {
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
   }
   return FALSE;
}

char bishopMoveCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{
   int xDir, yDir = 0;
   if(ogCol-newCol>0)
      yDir=-1;
   else if(ogCol-newCol<0)
      yDir=1;
   else
      yDir=0;
   if(ogRow-newRow>0)
      xDir=-1;
   else if(ogRow-newRow<0)
      xDir=1;
   else
      xDir=0;
   if(xDir!=0&&yDir!=0)
   {
      return clearPath(gameBoard, playerColor, ogRow, ogCol, newRow, newCol, xDir, yDir);
   }
   return FALSE;
}

char queenMoveCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{
   int xDir, yDir = 0;
   if(ogCol-newCol>0)
      yDir=-1;
   else if(ogCol-newCol<0)
      yDir=1;
   else
      yDir=0;
   if(ogRow-newRow>0)
      xDir=-1;
   else if(ogRow-newRow<0)
      xDir=1;
   else
      xDir=0;
   return clearPath(gameBoard, playerColor, ogRow, ogCol, newRow, newCol, xDir, yDir);
}

char kingMoveCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{
   if(ogRow-newRow>=-1&&ogRow-newRow<=1)
   {
      if(ogCol-newCol>=-1||ogCol-newCol<=1)
      {
         movePiece(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
         return TRUE;
      }
   }
   return FALSE;
}

char castleCheck(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{
   if(gameBoard->board[ogRow][ogCol].type==KING&&gameBoard->board[newRow][newCol].type==ROOK)//castle-ing
   {
      if(gameBoard->board[ogRow][ogCol].moved==FALSE&&gameBoard->board[newRow][newCol].moved==FALSE)
      {//checks if they havent moved yet
         if(gameBoard->checked!=playerColor)//not implemented
         {//cannot castle while checked
            if(newCol==0)//Queen side castle
            {
               if(gameBoard->board[ogRow][ogCol-1].type==EMPTY&&gameBoard->board[ogRow][ogCol-2].type==EMPTY&&
               gameBoard->board[ogRow][ogCol-3].type==EMPTY)
               {
                  movePiece(gameBoard, playerColor, ogRow, ogCol, ogRow, ogCol-2);
                  movePiece(gameBoard, playerColor, newRow, newCol, ogRow, ogCol-1);
                  return TRUE;
               }
            }
            else if(newCol==BOARDSIZE-1)//king size castle
            {
               if(gameBoard->board[ogRow][ogCol+1].type==EMPTY&&gameBoard->board[ogRow][ogCol+2].type==EMPTY)
               {
                  movePiece(gameBoard, playerColor, ogRow, ogCol, ogRow, ogCol+2);
                  movePiece(gameBoard, playerColor, newRow, newCol, ogRow, ogCol+1);
                  return TRUE;
               }
            }
         }
      }
   }
   return FALSE;
}

char moveValidation(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{//maybe change order as input is different
   if(gameBoard->board[ogRow][ogCol].color!=playerColor)
      return FALSE;//moving empty or not your piece check
   else if(gameBoard->board[ogRow][ogCol].color==gameBoard->board[newRow][newCol].color)
   {
      return castleCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol);
   }
   else
   {
      switch(gameBoard->board[ogRow][ogCol].type)
      {
         case PAWN:
            return pawnMoveCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol); break;
         case ROOK:
            return rookMoveCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol); break;
         case HORSE:
            return horseMoveCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol); break;
         case BISHOP:
            return bishopMoveCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol); break;
         case QUEEN:
            return queenMoveCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol); break;
         case KING:
            return kingMoveCheck(gameBoard, playerColor, ogRow, ogCol, newRow, newCol); break;
         default:
            printf("\nthis shouldnt happen"); return FALSE; break;
      }
   }
}

//returns false if incorrect format or move cannot be made
char boardTurn(struct Game *gameBoard, char playerColor)
{
   char input[100];
   int ogRow, ogCol, newRow, newCol=0;
   char turnOver;
   do{
      printf("\nPlease Enter moves in the following format: (A-H)(1-8)-(A-H)(1-8)");
      printf("\nenter the move you would like to make:");
      fgets(input,99,stdin);//gets user input

      ogCol=(int)input[0]-(int)('A');//converts char to int
      ogRow=BOARDSIZE-((int)(input[1])-(int)('0'));
      newCol=(int)input[3]-(int)('A');
      newRow=BOARDSIZE-((int)(input[4])-(int)('0'));

      if(ogRow<0||ogRow>BOARDSIZE-1||ogCol<0||ogCol>BOARDSIZE-1||
      newRow<0||newRow>BOARDSIZE-1||newCol<0||newCol>BOARDSIZE-1)
         turnOver=FALSE;
      else if(moveValidation(gameBoard,playerColor,ogRow,ogCol,newRow,newCol))
      {
         turnOver=TRUE;
         if(checkDetection(gameBoard,playerColor))//fix this
            printf("check!");
      }
      else
         turnOver=FALSE;
      if(turnOver==FALSE)
         printf("\nInvalid move or input!! Try again");
   }while(!turnOver);
}

int main()
{
   struct Game* gameBoard = createGame();//allocates space for game
   char gameLoop=TRUE;
   while(gameLoop)
   {
      setupBoard(gameBoard);
      printBoard(gameBoard);
      while(gameBoard->win==EMPTY)
      {
         boardTurn(gameBoard, WHITE);
         printBoard(gameBoard);

         boardTurn(gameBoard, BLACK);
         printBoard(gameBoard);
      }

      if(gameBoard->win==WHITE)
         printf("\nWhite Wins!!!!!");
      else if(gameBoard->win==BLACK)
         printf("\nBlack Wins!!!!!");
      else if(gameBoard->win==STALE)
         printf("\nStalemate.... :(");
      else
         printf("\nThis should not happen");
      gameLoop=FALSE;//change this to prompt user
   }
   destroyGame(gameBoard);
   return 1;
}
