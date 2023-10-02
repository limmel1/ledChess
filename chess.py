# ECE Sr Design 2
# Team 6: Tyler Ginn, Logan Immel, Matthew Britt, Krut Patel
# LED Chess

BOARDSIZE = 8
NOMOVES = -9

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

NUM_MOVES = 30 

class Piece:
    def __init__(self, color, type, moved=False):
        self.color = color
        self.type = type
        self.moved = moved
        self.moveList = [[NOMOVES, NOMOVES] for _ in range(NUM_MOVES)]

class Game:
    def __init__(self):
        self.board = [[Piece(EMPTY, EMPTY) for _ in range(BOARDSIZE)] for _ in range(BOARDSIZE)]
        self.checked = EMPTY # not implemented
        self.win = EMPTY # not implemented

# create object for new game
def createGame():
    newGame = Game()
    newGame.win = EMPTY
    newGame.checked = EMPTY

    return newGame

# check if piece in bounds
def outOfBounds(row, col):
    if row < 0 or row >= BOARDSIZE or col < 0 or col >= BOARDSIZE:
        return True
    return False

# update piece info
def setPiece(gameBoard, row, col, newColor, newType, newMove):
    gameBoard.board[row][col].color = newColor
    gameBoard.board[row][col].type = newType
    gameBoard.board[row][col].moved = newMove

# move piece
def movePiece(gameBoard, ogRow, ogCol, newRow, newCol):
    # pawn->queen promotion
    if gameBoard.board[ogRow][ogCol].type = PAWN:
        if newRow == 0 or newRow == BOARDSIZE-1:
            gameBoard.board[newRow][newCol].type = QUEEN

    # en passant
    if newCol != ogCol and gameBoard.board[newRow][newCol].type != PAWN
        setPiece(gameBoard, ogRow, newCol, EMPTY, EMPTY, True)

    # move to new pos
    gameBoard.board[newRow][newCol].type = gameBoard.board[ogRow][ogCol].type
    gameBoard.board[newRow][newCol].color = gameBoard.board[ogRow][ogCol].color
    gameBoard.board[newRow][newCol].moved = True

    # clear old pos
    gameBoard.board[ogRow][ogCol].type = EMPTY
    gameBoard.board[ogRow][ogCol].color = EMPTY
    gameBoard.board[ogRow][ogCol].moved = True

#CONFUSED
# set up game board for new game
def setupBoard(newGame):
    row = 0
    col = 0
    color = ''

    for row in BOARDSIZE:
        color = WHITE
        if row == 0 or row == 1:
            color = BLACK
        elif row == 6:
            for col in BOARDSIZE:
                setPiece(newGame,row,col,color,PAWN,False)
        elif row == 7:
            setPiece(newGame,row,0,color,ROOK,False)
            setPiece(newGame,row,1,color,HORSE,False)
            setPiece(newGame,row,2,color,BISHOP,False)
            setPiece(newGame,row,3,color,QUEEN,False)
            setPiece(newGame,row,4,color,KING,False)
            setPiece(newGame,row,5,color,BISHOP,False)
            setPiece(newGame,row,6,color,HORSE,False)
            setPiece(newGame,row,7,color,ROOK,False)
        else:
            for col in BOARDSIZE:
                setPiece(newGame,row,col,EMPTY,EMPTY,True)

def printBoard():
    return False

def boardTurn():
    return False

def moveValidation():
    return False

def calculateMoves():
    return False

def kingMoveCheck():
    return False

def queenMoveCheck():
    return False

def bishopMoveCheck():
    return False

def horseMoveCheck():
    return False

def rookMoveCheck():
    return False

def pawnMoveCheck(gameBoard, row, col):
    direction = 0
    moveInt = 0
    playerColor = gameBoard.board[row][col].color

    if playerColor == WHITE:
        direction = -1
    else
        direction = 1

    # move pawn forward by 1
    if (not outOfBounds(row+direction, col)) and gameBoard.board[row+direction][col].type==EMPTY:
        gameBoard.board[row][col].moveList[moveInt][0] = row+direction
        gameBoard.board[row][col].moveList[moveInt][1] = col
        moveInt++

    # move pawn forward by 2
    if gameBoard.board[row][col].moved==False and gameBoard.board[row+direction][col].type==EMPTY
            and gameBoard.board[row+direction*2][col].type==EMPTY:
        gameBoard.board[row][col].moveList[moveInt][0] = row+direction*2
        gameBoard.board[row][col].moveList[moveInt][1] = col
        moveInt++

    # take piece on right
    if (not outOfBounds(row+direction, col+1)) and gameBoard.board[row+direction][col+1].color!=gameBoard.board[row][col].color
            and gameBoard.board[row+direction][col+1].type!=EMPTY:
        gameBoard.board[row][col].moveList[moveInt][0] = row+direction
        gameBoard.board[row][col].moveList[moveInt][1] = col+1
        moveInt++

    # take piece on left
    # why compare type to color?
    if (not outOfBounds(row+direction, col-1)) and gameBoard.board[row+direction][col-1].type!=gameBoard.board[row][col].color
            and gameBoard.board[row+direction][col-1].type!=EMPTY:
        gameBoard.board[row][col].moveList[moveInt][0] = row+direction
        gameBoard.board[row][col].moveList[moveInt][1] = col-1
        moveInt++

    # en passant on right
    if (not outOfBounds(row, col+1)) and (not outOfBounds(row+direction,col)) and gameBoard.board[row][col+1].type==PAWN
            and gameBoard.board[row+direction][col].type==EMPTY and gameBoard.board[row+direction][col+1].type==EMPTY
            and gameBoard.board[row][col+1].color!=gameBoard.board[row][col].color:
        gameBoard.board[row][col].moveList[moveInt][0] = row+direction
        gameBoard.board[row][col].moveList[moveInt][1] = col+1
        moveInt++

    # en passant on left
    if (not outOfBounds(row, col-1)) and (not outOfBounds(row+direction,col)) and gameBoard.board[row][col-1].type==PAWN
            and gameBoard.board[row-direction][col].type==EMPTY and gameBoard.board[row+direction][col-1].type==EMPTY
            and gameBoard.board[row][col-1].color!=gameBoard.board[row][col].color:
        gameBoard.board[row][col].moveList[moveInt][0] = row+direction
        gameBoard.board[row][col].moveList[moveInt][1] = col-1
        moveInt++

    gameBoard.board[row][col].moveList[moveInt][0] = NOMOVES

# checks if path is clear for rook, bishop, & queen moves
def clearPath():
    return False

# INCOMPLETE
# looks for checks by copying game board and testing all king moves
def checkDetection():
    return False

# INCOMPLETE
# runs check detection on all possible king positions
def checkmateDetection():
    return False


#### 'main' 
mainGame = createGame() # create object for new game

gameLoop = True 
# main loop
while gameLoop:
