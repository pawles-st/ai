import numpy as np
import time

# player == 1   ->   max
# player == -1   ->   min

# game parameters

board_width = 5
board_height = 5
b = np.zeros((board_width, board_height), int)

def get_empty(board): # find all empty tile coordinates on the given board and return them
    empty = []
    for row in range(board_height):
        for column in range(board_width):
            if board[row][column] == 0: # empty iff tile == 0
                empty.insert(0, (row, column))
    return empty

def calc_heuristic(board): # calculate the heuristic value of the given board
    possible_fours = 0
    for row in range(0, board_width): # horizontal fours
        for column in range(0, board_height - 3):
            if board[row][column] == 0 and board[row][column + 1] == 0 and board[row][column + 2] == 0 and board[row][column + 3] == 0:
                possible_fours += 1
    for row in range(0, board_width - 3): # vertical fours
        for column in range(0, board_height):
            if board[row][column] == 0 and board[row + 1][column] == 0 and board[row + 2][column] == 0 and board[row + 3][column] == 0:
                possible_fours += 1
    for row in range(0, board_width - 3): # diagonal fours
        for column in range(0, board_height - 3):
            if board[row][column] == 0 and board[row + 1][column + 1] == 0 and board[row + 2][column + 2] == 0 and board[row + 3][column + 3] == 0:
                possible_fours += 1
        for column in range(board_height - 1, 3, -1):
            if board[row][column] == 0 and board[row + 1][column - 1] == 0 and board[row + 2][column - 2] == 0 and board[row + 3][column - 3] == 0:
                possible_fours += 1
    return possible_fours


def minimax(player, board, max_depth, curr_depth = 1): # find the next move on the given board using minimax

    # check recursion depth limit: return heuristic value of the board in the leaf

    if curr_depth == max_depth:

        # find all empty tiles

        empty_tiles = get_empty(board)
        move_heuristics = {}

        # calculate heuristic value for all possible moves and pick the optimal

        for tile in empty_tiles:
            board[tile[0], tile[1]] = player
            move_heuristics[(tile[0], tile[1])] = calc_heuristic(board)
            board[tile[0], tile[1]] = 0
        #print(move_heuristics)
        if player == 1:
            optimal_move = max(move_heuristics, key = lambda move : move[1])
            return (optimal_move, move_heuristics[optimal_move])
        elif player == -1:
            optimal_move = min(move_heuristics, key = lambda move : move[1])
            return (optimal_move, move_heuristics[optimal_move])
    else: # otherwise recurse further into the analysis tree

        # find all empty tiles

        empty_tiles = get_empty(board)
        move_heuristics = {}

        # recurse further for each possible move and pick the best option

        for tile in empty_tiles:
            board[tile[0], tile[1]] = player
            (optimal_move, heuristic_value) = minimax(-player, board, max_depth, curr_depth + 1)
            move_heuristics[(optimal_move[0], optimal_move[1])] = heuristic_value
            board[tile[0], tile[1]] = 0
        #print(move_heuristics)
        if player == 1:
            optimal_move = max(move_heuristics, key = lambda move : move[1])
            return (optimal_move, move_heuristics[optimal_move])
        elif player == -1:
            optimal_move = min(move_heuristics, key = lambda move : move[1])
            return (optimal_move, move_heuristics[optimal_move])

start = time.time()
minimax(1, b, 5)
end = time.time()
print(end - start)
