import sys

board = list(sys.argv[1])

def isWin(board):
    w = '012345678036147258048246'
    for i in range(0, len(w), 3):
        set_values = {board[int(w[i])], board[int(w[i+1])], board[int(w[i+2])]}
        if len(set_values) == 1 and '.' not in set_values:
            return 1 if set_values.pop() == 'O' else -1
    return 0

def isDraw(board):
    return '.' not in board

def minimax(board, isMax):
    if isDraw(board):
        return 0
    if isWin(board) != 0:
        return isWin(board)
    
    best = -2 if isMax else 2
    for i in range(9):
        if board[i] == '.':
            board[i] = 'O' if isMax else 'X'
            val = minimax(board, not isMax)
            board[i] = '.'
            best = max(best, val) if isMax else min(best, val)
    return best

def modify(board):
    best = -2
    move = -1
    for i in range(9):
        if board[i] == '.':
            board[i] = 'O'
            val = minimax(board, False)
            board[i] = '.'
            if val > best:
                best = val
                move = i
    board[move] = 'O'
    return ''.join(board), move

new_board, move = modify(board)
print(new_board, move)
