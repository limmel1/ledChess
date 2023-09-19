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
#define NOMOVES -9

#define WHITE 'w'
#define BLACK 'b'
#define STALE 's'//havent implemented

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
   short int** moveList;

};

struct Game
{
   struct Piece** board;
   char checked;//havent implemented
   char win;//havent implemented
};

struct Game* createGame()
{
   int i,j,k;
   struct Game* newGame=(struct Game*)malloc(sizeof(struct Game));
   newGame->board=malloc(BOARDSIZE*sizeof(struct Piece*));

   for(i=0;i<BOARDSIZE;i++)
   {
      newGame->board[i]=malloc(BOARDSIZE*sizeof(struct Piece));
      for(j=0;j<BOARDSIZE;j++)
      {
         newGame->board[i][j].moveList=malloc(30*sizeof(int*));//this is a magic number !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         for(k=0;k<30;k++)//magic number alert BAAAHOOOAAAAHOOO!!!!!
         {
            newGame->board[i][j].moveList[k]=malloc(2*sizeof(int));
            newGame->board[i][j].moveList[k][0]=NOMOVES;
            newGame->board[i][j].moveList[k][1]=NOMOVES;
         }
      }
   }
   newGame->win=EMPTY;
   newGame->checked=EMPTY;
   return newGame;
}

void destroyGame(struct Game* oldGame)
{
   int i,j,k;
   for(i=0;i<BOARDSIZE;i++)
   {
      for(j=0;j<BOARDSIZE;j++)
      {
         for(k=0;k<30;k++)//magic number alert BAAAHOOOAAAAHOOO!!!!!
         {
            free(oldGame->board[i][j].moveList[0]);
            free(oldGame->board[i][j].moveList[1]);
         }
         free(oldGame->board[i][j].moveList);
         free(oldGame->board[i]);
      }
   }
   free(oldGame->board);
   free(oldGame);
}

char outOfBounds(int row, int col)
{
   if(row<0||row>BOARDSIZE-1||col<0||col>BOARDSIZE-1)
      return TRUE;
   return FALSE;
}

void setPiece(struct Game* gameBoard, int row, int col, char newColor, char newType, char newMove)
{
   gameBoard->board[row][col].color=newColor;
   gameBoard->board[row][col].type=newType;
   gameBoard->board[row][col].moved=newMove;
}

void movePiece(struct Game* gameBoard, int ogRow, int ogCol, int newRow, int newCol)
{//needs to be updated to handle castling
   if(gameBoard->board[ogRow][ogCol].type==PAWN)
   {//pawn promotion to queen (add option to pick)
      if(newRow==0||newRow==BOARDSIZE-1)
      {//pawn promotion handling
         //ADD OPTION TO SELECT TYPE
         gameBoard->board[newRow][newCol].type=QUEEN;
      }
      if(newCol!=ogCol&&gameBoard->board[newRow][newCol].type!=PAWN)
      {//en pasant handling
         setPiece(gameBoard, ogRow, newCol,EMPTY, EMPTY, TRUE);
      }
   }
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
      switch(row){
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
{//not done
   return FALSE;
}


//creates a copy of the board where the move is successful and checks if this move leaves your king open
//it does this by checks all pieces move on the king
//this is not optimal but it will work :(
//done before moving piece and after for checkmate
char checkDetection()
{//not done
   return FALSE;
}

//used by rook bishop and queen for making sure path has no pieces in way
void clearPath(struct Game* gameBoard, int ogRow, int ogCol, int xDir, int yDir, int* moveInt)
{
   int i, row, col;
   row=ogRow;
   col=ogCol;
   for(i=0; i<BOARDSIZE; i++)
   {//this for loop will never reach BOARDSIZE but this is an out of bounds check
      row+=xDir;
      col+=yDir;
      if(outOfBounds(row,col))
      {//out of bounds and quits
         return;
      }
      else if(gameBoard->board[row][col].type==EMPTY)
      {//adds empty spot to list of possible moves
         gameBoard->board[ogRow][ogCol].moveList[*(moveInt)][0]=row;
         gameBoard->board[ogRow][ogCol].moveList[*(moveInt)][1]=col;
         (*(moveInt))++;//this is so ugly but it works
      }
      else if(gameBoard->board[row][col].color!=gameBoard->board[ogRow][ogCol].color)
      {//adds last enemy piece in path to list of possible moves and stops loop
         gameBoard->board[ogRow][ogCol].moveList[*(moveInt)][0]=row;
         gameBoard->board[ogRow][ogCol].moveList[*(moveInt)][1]=col;
         (*(moveInt))++;//this is so ugly but it works
         return;
      }
      else
         return;
   }
}

void pawnMoveCheck(struct Game* gameBoard, int row, int col)
{//done
   int direction, moveInt=0;
   char playerColor=gameBoard->board[row][col].color;
   if(playerColor==WHITE)
      direction=-1;
   else
      direction=1;
   if(!outOfBounds(row+direction,col),gameBoard->board[row+direction][col].type==EMPTY)//one forward
   {
      gameBoard->board[row][col].moveList[moveInt][0]=row+direction;
      gameBoard->board[row][col].moveList[moveInt][1]=col;
      moveInt++;
   }
   if(gameBoard->board[row][col].moved==FALSE&&gameBoard->board[row+direction][col].type==EMPTY&&
      gameBoard->board[row+direction*2][col].type==EMPTY)//two forward
   {
      gameBoard->board[row][col].moveList[moveInt][0]=row+direction*2;
      gameBoard->board[row][col].moveList[moveInt][1]=col;
      moveInt++;
   }
   if(!outOfBounds(row+direction,col+1) && gameBoard->board[row+direction][col+1].color!=gameBoard->board[row][col].color &&
      gameBoard->board[row+direction][col+1].type!=EMPTY)//right take piece
   {
      gameBoard->board[row][col].moveList[moveInt][0]=row+direction;
      gameBoard->board[row][col].moveList[moveInt][1]=col+1;
      moveInt++;
   }
   if(!outOfBounds(row+direction,col-1) && gameBoard->board[row+direction][col-1].type!=gameBoard->board[row][col].color &&
      gameBoard->board[row+direction][col-1].type!=EMPTY)//right take piece
   {
      gameBoard->board[row][col].moveList[moveInt][0]=row+direction;
      gameBoard->board[row][col].moveList[moveInt][1]=col-1;
      moveInt++;
   }
   if(!outOfBounds(row,col+1)&&!outOfBounds(row+direction,col)&&gameBoard->board[row][col+1].type==PAWN&&gameBoard->board[row+direction][col].type==EMPTY&&
      gameBoard->board[row+direction][col+1].type==EMPTY&&gameBoard->board[row][col+1].color!=gameBoard->board[row][col].color)//right en passant
   {
      gameBoard->board[row][col].moveList[moveInt][0]=row+direction;
      gameBoard->board[row][col].moveList[moveInt][1]=col+1;
      moveInt++;
   }
   if(!outOfBounds(row,col-1)&&!outOfBounds(row+direction,col)&&gameBoard->board[row][col-1].type==PAWN&&gameBoard->board[row-direction][col].type==EMPTY&&
      gameBoard->board[row+direction][col-1].type==EMPTY&&gameBoard->board[row][col-1].color!=gameBoard->board[row][col].color)//left en passant
   {
      gameBoard->board[row][col].moveList[moveInt][0]=row+direction;
      gameBoard->board[row][col].moveList[moveInt][1]=col-1;
      moveInt++;
   }
   gameBoard->board[row][col].moveList[moveInt][0]=NOMOVES;
}

void rookMoveCheck(struct Game* gameBoard, int row, int col)
{//done
   int moveInt = 0;
   clearPath(gameBoard, row, col, 0, 1, &moveInt);
   clearPath(gameBoard, row, col, 1, 0, &moveInt);
   clearPath(gameBoard, row, col, 0, -1, &moveInt);
   clearPath(gameBoard, row, col, -1, 0, &moveInt);
   gameBoard->board[row][col].moveList[moveInt][0]=NOMOVES;
}

void horseMoveCheck(struct Game* gameBoard, int row, int col)
{//done
   int moveInt=0;
   int newRow = 0;
   int newCol = 0;
   const int horseMoves[8][2] = {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};//magic number land !!!!!
   for(int i=0; i<8;i++)
   {
      newRow=row+horseMoves[i][0];
      newCol=col+horseMoves[i][1];
      if(!outOfBounds(newRow,newCol))
      {
         if(gameBoard->board[newRow][newCol].color!=gameBoard->board[row][col].color)
         {
            gameBoard->board[row][col].moveList[moveInt][0]=newRow;
            gameBoard->board[row][col].moveList[moveInt][1]=newCol;
            moveInt++;
         }
      }
   }
   gameBoard->board[row][col].moveList[moveInt][0]=NOMOVES;
}

void bishopMoveCheck(struct Game* gameBoard, int row, int col)
{//done
   int moveInt = 0;
   clearPath(gameBoard, row, col, 1, 1, &moveInt);
   clearPath(gameBoard, row, col, 1, -1, &moveInt);
   clearPath(gameBoard, row, col, -1, 1, &moveInt);
   clearPath(gameBoard, row, col, -1, -1, &moveInt);
   gameBoard->board[row][col].moveList[moveInt][0]=NOMOVES;
}

void queenMoveCheck(struct Game* gameBoard, int row, int col)
{//done
   int moveInt = 0;
   clearPath(gameBoard, row, col, 0, 1, &moveInt);//rook
   clearPath(gameBoard, row, col, 1, 0, &moveInt);
   clearPath(gameBoard, row, col, 0, -1, &moveInt);
   clearPath(gameBoard, row, col, -1, 0, &moveInt);
   clearPath(gameBoard, row, col, 1, 1, &moveInt);//bishop
   clearPath(gameBoard, row, col, 1, -1, &moveInt);
   clearPath(gameBoard, row, col, -1, 1, &moveInt);
   clearPath(gameBoard, row, col, -1, -1, &moveInt);
   gameBoard->board[row][col].moveList[moveInt][0]=NOMOVES;
}

void kingMoveCheck(struct Game* gameBoard, int row, int col)
{
   int r, c, moveInt = 0;
   for(r=row-1;r<row+2;r++)
   {
      for(c=col-1;c<col+2;c++)
      {
         if(!((r==row&&c==col)||outOfBounds(r,c)||gameBoard->board[r][c].color==gameBoard->board[row][col].color))
         {
            gameBoard->board[row][col].moveList[moveInt][0]=r;
            gameBoard->board[row][col].moveList[moveInt][1]=c;
            moveInt++;
         }
      }
   }
   if(gameBoard->board[row][col].moved==FALSE&&gameBoard->checked==FALSE)
   {//castle check
      if(gameBoard->board[row][0].moved==FALSE)
      {//queen side castle
         if(gameBoard->board[row][1].type==EMPTY&&gameBoard->board[row][2].type==EMPTY&&gameBoard->board[row][3].type==EMPTY)
         {
            gameBoard->board[row][col].moveList[moveInt][0]=row;
            gameBoard->board[row][col].moveList[moveInt][1]=0;
            moveInt++;
         }
      }
      if(gameBoard->board[row][BOARDSIZE-1].moved==FALSE)
      {//king side castle
         if(gameBoard->board[row][BOARDSIZE-2].type==EMPTY&&gameBoard->board[row][BOARDSIZE-3].type==EMPTY)
         {
            gameBoard->board[row][col].moveList[moveInt][0]=row;
            gameBoard->board[row][col].moveList[moveInt][1]=BOARDSIZE-1;
            moveInt++;
         }
      }
   }

   gameBoard->board[row][col].moveList[moveInt][0]=NOMOVES;
}

void calculateMoves(struct Game* gameBoard)
{
   int row,col;
   for(row=0;row<BOARDSIZE;row++)
   {
      for(col=0;col<BOARDSIZE;col++)
      {
         switch(gameBoard->board[row][col].type)
         {
         case PAWN:
            pawnMoveCheck(gameBoard, row, col); break;
         case ROOK:
            rookMoveCheck(gameBoard, row, col); break;
         case HORSE:
            horseMoveCheck(gameBoard, row, col); break;
         case BISHOP:
            bishopMoveCheck(gameBoard, row, col); break;
         case QUEEN:
            queenMoveCheck(gameBoard, row, col); break;
         case KING:
            kingMoveCheck(gameBoard, row, col); break;
         case EMPTY:
            gameBoard->board[row][col].moveList[0][0]=NOMOVES;//if first element in list is nomoves no more moves exist
         default:
             break;
         }
         int moveInt=0;
         printf("\n%c%d: %c %c: ",(char)(col+((int)'A')), BOARDSIZE-row, gameBoard->board[row][col].color,gameBoard->board[row][col].type);
         while(gameBoard->board[row][col].moveList[moveInt][0]!=NOMOVES)
         {
            printf("%c%d, ",(char)(gameBoard->board[row][col].moveList[moveInt][1]+((int)'A')),BOARDSIZE-gameBoard->board[row][col].moveList[moveInt][0]);
            moveInt++;
         }
      }
   }
}

char moveValidation(struct Game* gameBoard, char playerColor, int ogRow, int ogCol, int newRow, int newCol)
{//maybe change order as input is different
   int moveInt=0;
   if(gameBoard->board[ogRow][ogCol].color!=playerColor)
      return FALSE;//moving empty or not your piece check
   else
   {
      while(gameBoard->board[ogRow][ogCol].moveList[moveInt][0]!=NOMOVES)
      {//goes through move list until end
         if(newRow==gameBoard->board[ogRow][ogCol].moveList[moveInt][0]&&
            newCol==gameBoard->board[ogRow][ogCol].moveList[moveInt][1])
         {
            movePiece(gameBoard, ogRow, ogCol, newRow, newCol);
            return TRUE;
         }
         moveInt++;
      }
   }
   return FALSE;
}

//returns false if incorrect format or move cannot be made
char boardTurn(struct Game *gameBoard, char playerColor)
{
   char input[100];
   int ogRow, ogCol, newRow, newCol=0;
   char turnOver=FALSE;
   do{
      if(playerColor==WHITE)
          printf("\nWhite ");
      else
          printf("\nBlack ");
      printf("enter moves in the following format: (A-H)(1-8)-(A-H)(1-8)");
      printf("\nenter the move you would like to make:");
      fgets(input,99,stdin);//gets user input

      ogCol=(int)input[0]-(int)('A');//converts char to int
      ogRow=BOARDSIZE-((int)(input[1])-(int)('0'));
      newCol=(int)input[3]-(int)('A');
      newRow=BOARDSIZE-((int)(input[4])-(int)('0'));

      if(moveValidation(gameBoard,playerColor,ogRow,ogCol,newRow,newCol))
         turnOver=TRUE;
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
         calculateMoves(gameBoard);
         boardTurn(gameBoard, WHITE);
         printBoard(gameBoard);

         calculateMoves(gameBoard);
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
