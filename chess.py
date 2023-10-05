# ECE Sr Design 2
# Team 6: Tyler Ginn, Logan Immel, Matthew Britt, Krut Patel
# LED Chess

BOARDSIZE = 8

WHITE = 'w'
BLACK = 'b'
STALE = 's' # not implemented

EMPTY = 'E'
PAWN = 'P'
ROOK = 'R'
HORSE = 'H'
BISHOP = 'B'
QUEEN = 'Q'
KING = 'K'

class Piece:
    def __init__(self, color, type, moved=False):
        self.color = color
        self.type = type
        self.moved = moved

class Game:
    def __init__(self):
        self.board = [[Piece(EMPTY, EMPTY) for _ in range(BOARDSIZE)] for _ in range(BOARDSIZE)]
        self.checked = EMPTY # not implemented
        self.win = EMPTY # not implemented
        self.blackMoves = []
        self.whiteMoves = []
        self.history = []  # not implemented

# create object for new game
def createGame():
    newGame = Game()
    newGame.win = EMPTY
    newGame.checked = EMPTY
    newGame.blackMoves = []
    newGame.whiteMoves = []

    return newGame

# check if piece in bounds
def outOfBounds(row, col):
    return row < 0 or row >= BOARDSIZE or col < 0 or col >= BOARDSIZE

# update piece info
def setPiece(gameBoard, row, col, newColor, newType, newMove):
    gameBoard.board[row][col].color = newColor
    gameBoard.board[row][col].type = newType
    gameBoard.board[row][col].moved = newMove

# move piece
def movePiece(gameBoard, ogRow, ogCol, newRow, newCol):
    # pawn->queen promotion
    if gameBoard.board[ogRow][ogCol].type == PAWN:
        if newRow == 0 or newRow == BOARDSIZE-1:
            gameBoard.board[newRow][newCol].type = QUEEN

    # en passant not functioning correctly
    if newCol != ogCol and gameBoard.board[newRow][newCol].type != PAWN:
        setPiece(gameBoard, ogRow, newCol, EMPTY, EMPTY, True)

    # move to new pos
    gameBoard.board[newRow][newCol].type = gameBoard.board[ogRow][ogCol].type
    gameBoard.board[newRow][newCol].color = gameBoard.board[ogRow][ogCol].color
    gameBoard.board[newRow][newCol].moved = True

    # clear old pos
    gameBoard.board[ogRow][ogCol].type = EMPTY
    gameBoard.board[ogRow][ogCol].color = EMPTY
    gameBoard.board[ogRow][ogCol].moved = True

# fixed CONFUSED no longer
# set up game board for new game
def setupBoard(newGame):
    color = BLACK
    for row in range(BOARDSIZE):
        if row == 3:
            color = WHITE
        if row == 6 or row == 1:
            for col in range(BOARDSIZE):
                setPiece(newGame,row,col,color,PAWN,False)
        elif row == 7 or row == 0:
            setPiece(newGame,row,0,color,ROOK,False)
            setPiece(newGame,row,1,color,HORSE,False)
            setPiece(newGame,row,2,color,BISHOP,False)
            setPiece(newGame,row,3,color,QUEEN,False)
            setPiece(newGame,row,4,color,KING,False)
            setPiece(newGame,row,5,color,BISHOP,False)
            setPiece(newGame,row,6,color,HORSE,False)
            setPiece(newGame,row,7,color,ROOK,False)
        else:
            for col in range(BOARDSIZE):
                setPiece(newGame,row,col,EMPTY,EMPTY,True)

def printBoard(gameBoard):
    print('  0  1  2  3  4  5  6  7')
    for row in range(BOARDSIZE):
        print(row,'',end='')
        for col in range(BOARDSIZE):
            print(gameBoard.board[row][col].color, gameBoard.board[row][col].type, ' ' ,sep='', end='')
        print('')

def boardTurn(gameBoard, playerColor):
    turn = True
    while turn:
        print(playerColor, 'enter moves in the following format: (0-7)(0-7)-(0-7)(0-7)')
        print('enter the move you would like to make:')
        inputt=input()
        ogCol=int(inputt[0])
        ogRow=int(inputt[1])
        newCol=int(inputt[3])
        newRow=int(inputt[4])
        if moveValid(gameBoard,playerColor,ogRow,ogCol,newRow,newCol):
            turn=False
        else:
            print('invalid move or input!! try again...')

def moveValid(gameBoard,playerColor,ogRow,ogCol,newRow,newCol):
    if gameBoard.board[ogRow][ogCol].color==playerColor:
        if playerColor==WHITE:
            if [[ogRow,ogCol],[newRow,newCol]] in gameBoard.whiteMoves:
                movePiece(gameBoard, ogRow, ogCol, newRow, newCol)
                return True        
        if playerColor==BLACK:
            if [[ogRow,ogCol],[newRow,newCol]] in gameBoard.blackMoves:
                movePiece(gameBoard, ogRow, ogCol, newRow, newCol)
                return True
    return False

def calculateMoves(gameBoard):
    gameBoard.whiteMoves=[]
    gameBoard.blackMoves=[]
    for row in range(BOARDSIZE):
        for col in range(BOARDSIZE):
            moveList=[]
            type=gameBoard.board[row][col].type
            if type == PAWN:
                moveList=pawnMoveCheck(gameBoard, row, col)
            elif type == ROOK:
                moveList=rookMoveCheck(gameBoard, row, col)
            elif type == HORSE:
                moveList=horseMoveCheck(gameBoard, row, col)
            elif type == BISHOP:
                moveList=bishopMoveCheck(gameBoard, row, col)
            elif type == QUEEN:
                moveList=queenMoveCheck(gameBoard, row, col)
            elif type == KING:
                moveList=kingMoveCheck(gameBoard, row, col)
            if moveList != []:
              if gameBoard.board[row][col].color == WHITE:
                  for newRow, newCol in moveList:
                      gameBoard.whiteMoves.append([[row,col],[newRow, newCol]])
              if gameBoard.board[row][col].color == BLACK:
                  for newRow, newCol in moveList:
                      gameBoard.blackMoves.append([[row,col],[newRow, newCol]])

def kingMoveCheck(gameBoard, row, col):
    moveList=[]
    for r in range(BOARDSIZE):
        for c in range(BOARDSIZE):
            if not ((r==row and c==col) or outOfBounds(r,c) or gameBoard.board[r][c].color==gameBoard.board[row][col].color):
                moveList.append([r,c])
    if gameBoard.board[row][col].moved==False and gameBoard.checked==False:
        if gameBoard.board[row][0].moved==False:
            if gameBoard.board[row][1].type==EMPTY and gameBoard.board[row][2].type==EMPTY and gameBoard.board[row][3].type==EMPTY:
                moveList.append([row,col-2])
        if gameBoard.board[row][BOARDSIZE-1].moved==False:
            if gameBoard.board[row][BOARDSIZE-2].type==EMPTY and gameBoard.board[row][BOARDSIZE-3].type==EMPTY:
                moveList.append([row,col+2])
    return moveList

# checks if path is clear for rook, bishop, & queen moves
def clearPath(gameBoard, ogRow, ogCol, dir):
    moveList = []
    for xDir, yDir in dir:
        row=ogRow
        col=ogCol
        for _ in range(BOARDSIZE):
            row+=xDir
            col+=yDir
            if outOfBounds(row,col):
              break
            elif gameBoard.board[row][col].type==EMPTY:
                moveList.append([row,col])
            elif gameBoard.board[row][col].color!=gameBoard.board[ogRow][ogCol].color:
                moveList.append([row,col])
                break
            else:
                break
    return moveList

def queenMoveCheck(gameBoard, row, col):
    return clearPath(gameBoard,row,col,[[1,1],[1,-1],[-1,1],[-1,-1],[0,1],[1,0],[0,1],[-1,0]])

def bishopMoveCheck(gameBoard, row, col):
    return clearPath(gameBoard, row, col, [[1,1],[1,-1],[-1,1],[-1,-1]])

def rookMoveCheck(gameBoard, row, col):
    return clearPath(gameBoard, row, col, [[0,1],[1,0],[0,1],[-1,0]])

def horseMoveCheck(gameBoard, row, col):
    moveList=[]
    horseMoves = [[1,2],[1,-2],[-1,2],[-1,-2],[2,1],[2,-1],[-2,1],[-2,-1]]
    for i in range(BOARDSIZE):
        newRow=row+horseMoves[i][0]
        newCol=col+horseMoves[i][1]
        if not outOfBounds(newRow,newCol):
            if gameBoard.board[newRow][newCol].color!=gameBoard.board[row][col].color:
                moveList.append([newRow,newCol])
    return moveList

def pawnMoveCheck(gameBoard, row, col):
    moveList=[]
    direction = 0
    moveInt = 0
    playerColor = gameBoard.board[row][col].color

    if playerColor == WHITE:
        direction = -1
    else:
        direction = 1

    # move pawn forward by 1
    if (not outOfBounds(row+direction, col)) and gameBoard.board[row+direction][col].type==EMPTY:
        moveList.append([row+direction,col])

    # move pawn forward by 2
    if gameBoard.board[row][col].moved==False and gameBoard.board[row+direction][col].type==EMPTY and gameBoard.board[row+direction*2][col].type==EMPTY:
        moveList.append([row+direction*2,col])

    # take piece on right
    if (not outOfBounds(row+direction, col+1)) and gameBoard.board[row+direction][col+1].color!=gameBoard.board[row][col].color and gameBoard.board[row+direction][col+1].type!=EMPTY:
        moveList.append([row+direction,col+1])

    # take piece on left
    if (not outOfBounds(row+direction, col-1)) and gameBoard.board[row+direction][col-1].color!=gameBoard.board[row][col].color and gameBoard.board[row+direction][col-1].type!=EMPTY:
        moveList.append([row+direction,col-1])

    # en passant on right this is not correct must check prev move
    if (not outOfBounds(row, col+1)) and (not outOfBounds(row+direction,col)) and gameBoard.board[row][col+1].type==PAWN and gameBoard.board[row+direction][col].type==EMPTY and gameBoard.board[row+direction][col+1].type==EMPTY and gameBoard.board[row][col+1].color!=gameBoard.board[row][col].color:
        moveList.append([row+direction,col+1])

    # en passant on left this is not correct must check prev move
    if (not outOfBounds(row, col-1)) and (not outOfBounds(row+direction,col)) and gameBoard.board[row][col-1].type==PAWN and gameBoard.board[row-direction][col].type==EMPTY and gameBoard.board[row+direction][col-1].type==EMPTY and gameBoard.board[row][col-1].color!=gameBoard.board[row][col].color:
        moveList.append([row+direction,col-1])
    return moveList

def copyGame(copyBoard):
  copy=createGame()
  setupBoard(copy)
  for r in range(BOARDSIZE):
    for c in range(BOARDSIZE):
      copy.board[r][c].type=copyBoard.board[r][c].type
      copy.board[r][c].color=copyBoard.board[r][c].color
      copy.board[r][c].moved=copyBoard.board[r][c].moved
  return copy

#removes moves that would place the king in danger by testing all possible moves for playerColor
def checkRemoval(gameBoard, playerColor):
    if playerColor==WHITE:
        posMoves = gameBoard.whiteMoves
    else:
        posMoves = gameBoard.blackMoves
    for [ogRow,ogCol],[newRow,newCol] in posMoves:
        fakeBoard = copyGame(gameBoard)
        movePiece(fakeBoard, ogRow, ogCol, newRow, newCol)
        calculateMoves(fakeBoard)
        if(checkDetection(fakeBoard,playerColor)):#puts/leaves king in danger
            if playerColor==WHITE:
                gameBoard.whiteMoves.remove([[ogRow,ogCol],[newRow,newCol]])
            else:
                gameBoard.blackMoves.remove([[ogRow,ogCol],[newRow,newCol]])

# looks for checks by copying game board and testing all king moves
def checkDetection(gameBoard,playerColor):
    kingX=-1
    kingY=-1
    for row in range(BOARDSIZE):
        for col in range(BOARDSIZE):
            if gameBoard.board[row][col].type==KING and gameBoard.board[row][col].color==playerColor:
                kingX=row
                kingY=col
    if kingX==-1:
        print('this should never happen')
    else:
       for [[ogRow ,ogCol ] ,[newRow,newCol]] in gameBoard.whiteMoves:
            if newRow == kingX and newCol==kingY:
              return True
    return False

def checkmateDetection(gameBoard,playerColor):
    if playerColor==WHITE and gameBoard.whiteMoves==[]:
        return WHITE
    if playerColor==BLACK and gameBoard.blackMoves==[]:
        return BLACK
    return EMPTY

#### 'main'
mainGame = createGame() # create object for new game
gameLoop = True
startButton = 1
# main loop
while gameLoop:
    if startButton:
        setupBoard(mainGame)
        color=WHITE
        while True:
            calculateMoves(mainGame)
            checkRemoval(mainGame,color)
            mainGame.win=checkmateDetection(mainGame,color)
            printBoard(mainGame)
            boardTurn(mainGame,color)
            if mainGame.win==WHITE:
                print('white wins!!')
                break
            elif mainGame.win==BLACK:
                print('black wins!!')
                break
            elif mainGame.win==STALE:
                print('stalemate :(')
                break
            else:
                if color == WHITE:
                    color = BLACK
                else:
                    color = WHITE


