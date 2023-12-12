# Bejeweled Game and AI Player
# Made by Jesslyn Andriono, April 2023

DIR_UP = "u"
DIR_DOWN = "d"
DIR_LEFT = "l"
DIR_RIGHT = "r"
BLANK_PIECE = "Z"

def validate_input(board, position, direction):
    """ Accepts the user's inputs: game's board grid, position of the value to
    move, and direction of movement. Checks whether said inputs are valid.
    Returns True if all valid, returns False is any are invalid.
    """
    if not (check_board_spec(board) and check_board_value(board) 
            and check_position(position, board) 
            and check_direction(direction)):
        return False
    else:
        return True
    
def check_board_spec(board):
    """ Accepts the board grid. Checks if the board grid's specification, its
    number of rows and columns and the length of each row. Returns True if
    all conditions are met, returns False otherwise.
    """
    # Checks if the board contains at least 2 rows and at least 2 columns
    if (len(board) < 2) or (len(board[0]) < 2):
        return False
    
    # Checks if each row in the board has the same length
    for row in board:
        if len(board[0]) != len(row):
            return False
        
    return True
    
def check_board_value(board):
    """ Accepts the board grid. Checks if board values are one upper case
    alphabetical character and if the number of same-coloured pieces are
    multiples of 4. Returns True if all conditions are met, returns False 
    otherwise. 
    """
    # Checks if each board value is a single upper case alphabetical character
    # Records the number of each piece colour in the dictionary "colour_count"
    colour_count = {}
    for row in board:
        for value in row:
            if not (value.isupper() and value.isalpha() and (len(value) == 1)):
                return False
            
            if value in colour_count:
                colour_count[value] += 1
            else:
                colour_count[value] = 1

    # Checks if the number of pieces in each colour, excluding blanks, is a 
    # multiple of 4
    if BLANK_PIECE in colour_count:
        del colour_count[BLANK_PIECE]
    for colour in colour_count:
        if (colour_count[colour] % 4) != 0:
            return False
        
    return True


def check_position(position, board):
    """ Accepts the position of the value to move and the board grid. Checks
    if the position input is within the board and not negative values. Returns
    True if all conditions are met, returns False otherwise.
    """
    if not ((0 <= position[0] <= (len(board) - 1)) 
            and (0 <= position[1] <= (len(board[0]) - 1))):
        return False
    else:
        return True
    
    
def check_direction(direction):
    """ Accepts the direction of movement. Checks if direction input is among 
    the allowed direction values. Returns True if condition is met, returns
    False otherwise.
    """ 
    allowed_directions = DIR_UP + DIR_DOWN + DIR_LEFT + DIR_RIGHT
    if not ((len(direction) == 1) and (direction in allowed_directions)):
        return False
    else: 
        return True
    
def legal_move(board, position, direction):
    """ Accepts the board grid, position of piece wanting to be moved, and
    direction of movement. Returns True if the move is legal, being within 
    the board and either of the switching pieces becoming adjacent to another
    of the same color, returns False if not legal. """
    # Determine the position of the piece that is going to be replaced
    new_position = switched_piece_position(position, direction)
    
    # Check if destination position is on board, not negative, and not blank
    new_row = new_position[0]
    new_column = new_position[1]
    if not ((0 <= new_row <= (len(board) - 1)) 
            and (0 <= new_column <= (len(board[0]) - 1)) 
            and (board[new_row][new_column] not in BLANK_PIECE)):
        return False
     
    # Determine the piece values of the piece moved and the piece replaced
    moving_piece = board[position[0]][position[1]]
    replaced_piece = board[new_row][new_column]
   
    # List the pieces adjacent to the moving and replaced pieces after switch
    new_adjacent_pieces = adjacent_pieces(new_position, board, 
                                          moving_piece, replaced_piece)
    old_adjacent_pieces = adjacent_pieces(position, board, 
                                          replaced_piece, moving_piece)
    
    # Check if at least 1 of pieces switching becomes adjacent to same colour
    if moving_piece in new_adjacent_pieces:
        return True
    elif replaced_piece in old_adjacent_pieces:
        return True
    else:
        return False

    
def adjacent_pieces(position, board, current_piece, replacing_piece):
    """ Accepts the position of a piece, the current board grid, the current 
    piece in the position, and the potential piece replacing it after 
    switching. Returns the list of letters adjacent to the piece 
    (above, below, left, right) if one exists after the switch.
    """
    # Creates a list of coordinates of adjacent pieces
    adjacent_position = [(position[0] - 1, position[1]),
                            (position[0] + 1, position[1]), 
                            (position[0], position[1] - 1),
                            (position[0], position[1] + 1)]
    
    # Checks if position values are on the board and not negative
    # Adds surrounding piece values into list if position is valid
    adjacent_pieces = []
    for coordinate in adjacent_position:
        if ((0 <= coordinate[0] <= (len(board) - 1)) 
                and (0 <= coordinate[1] <= (len(board[0]) - 1))):
            adjacent_pieces.append(board[coordinate[0]][coordinate[1]])
    
    # Switching the position's current piece value with the potential new value
    # Simulating the change in board grid if the switch is made
    adjacent_pieces[adjacent_pieces.index(current_piece)] = replacing_piece
    
    return adjacent_pieces


def switched_piece_position(position, direction):
    """ Accepts the position of a piece and the direction of movement. Finds
    the position of the piece adjacent to the piece in the direction of 
    movement. Returns the position of said adjacent piece.
    """
    row = position[0]
    column = position[1]
    
    # Creates a tuple of the adjacent piece in the direction of movement
    if direction in DIR_UP:
        new_position = (row - 1, column)
    elif direction in DIR_DOWN:
        new_position = (row + 1, column)
    elif direction in DIR_LEFT:
        new_position = (row, column - 1)
    elif direction in DIR_RIGHT:
        new_position = (row, column + 1)
        
    return new_position

def make_move(board, position, direction):
    """ Accepts the board grid, position of piece being moved, and direction 
    of movement. The board grid is mutated to eliminate 2x2 squares of the same
    color, replacing it with blank piece "Z", and moving non blank piece values 
    to fill gaps created by the blank pieces. Returns the mutated board.
    """
    # Make the move
    switch_pieces(board, position, direction)
        
    # Eliminate 2x2 squares and fill gaps until no more changes can be made
    while board != fill_gap(eliminate_square(board)):
        board = fill_gap(eliminate_square(board))

    return board   
    
    
def switch_pieces(board, position, direction):
    """ Accepts the board grid, position of piece being moved, and direction 
    of movement. Mutates the board, switching the the piece at position and the 
    adjacent piece in the direction movement. Returns None"""
    row = position[0]
    column = position[1]
    
    # Determine the position of the piece being replaced
    if direction in DIR_UP:
        new_position = (row - 1, column)
    elif direction in DIR_DOWN:
        new_position = (row + 1, column)
    elif direction in DIR_LEFT:
        new_position = (row, column - 1)
    elif direction in DIR_RIGHT:
        new_position = (row, column + 1)
    
    # Switch the pieces
    temp = board[row][column]
    board[row][column] = board[new_position[0]][new_position[1]]
    board[new_position[0]][new_position[1]] = temp
    
    return None


def eliminate_square(board):
    """ Accepts the board grid, identifies 2x2 same-coloured squares and
    replace the square at the lowest row number with blank pieces "Z". Returns
    the mutated board.
    """
    # If the pieces in a 2x2 square are all same-coloured, store the top left
    # piece's position in a list
    square_position = []
    for row in range(len(board) - 1):
        for column in range(len(board) - 1):
            if (board[row][column] == board[row + 1][column] 
                == board[row][column + 1] == board[row + 1][column + 1]) and (
                board[row][column] != BLANK_PIECE):
                square_position.append((row, column))
    
    # If there is a 2x2 same-coloured square on the board, replace the square
    # at the lowest row number with blank pieces
    if square_position != []:
        row = square_position[0][0]
        column = square_position[0][1]
        board[row][column] = BLANK_PIECE
        board[row + 1][column] = BLANK_PIECE
        board[row][column + 1] = BLANK_PIECE
        board[row + 1][column + 1] = BLANK_PIECE
    
    return board 


def fill_gap(board):
    """ Accepts the board grid, move all non-blank pieces up to fill gaps, then
    moves left to fill remaining gaps. Returns mutated board."""
    # First, arrange the board array to by column (board[column][row] = value), 
    # move blank pieces to the the bottom of the grid. Second, arrange the 
    # board grid back to by row, moving blank pieces to the right of the grid.
    for i in range(2):
        board = list(map(list, list(zip(*board))))
        for column in board:
            count = column.count(BLANK_PIECE)
            for blanks in range(count):
                column.remove(BLANK_PIECE)
                column.append(BLANK_PIECE)
    
    return board 

def ai_player(board):
    """ Accepts the board grid, tests every possible combination of three
    moves and returns the shortest possible sequence of moves to win.
    """
    # Determine what the board grid looks like when all pieces are cleared
    winning_board = [[BLANK_PIECE] * len(board[0])] * len(board)
    
    # Determine all possible positions and directions
    all_positions = []
    all_directions = [DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT]
    for i in range(len(board)):
        for j in range(len(board[0])):
            all_positions.append((i, j))
    
    # Tests every possible move until a winning move is found every next 
    # possible move after each move is tried until a winning move is found. 
    # Ideally, this process is recursive. When a winning move is found, a list
    # of all the moves leading up to the winning move is returned.
    move = []
    found = False
    for position in all_positions:
        for direction in all_directions:
            if (validate_input(board, position, direction) 
                and legal_move(board, position, direction)):
                # Create a deep copy of the board to not mutate the original
                temp_board1 = []
                for row in board:
                    temp_list = []
                    for elem in row:
                        temp_list.append(elem)
                    temp_board1.append(temp_list)
                
                # Mutates the board's copy to simulate move    
                temp_board1 = make_move(temp_board1, position, direction)
                # Breaks if one winning move is found
                if temp_board1 == winning_board:
                    move.append((position, direction))
                    found = True
                # Tests next possible moves until winning move is found
                else:
                    for pos in all_positions:
                        for dire in all_directions:
                            if (validate_input(temp_board1, pos, dire) 
                                and legal_move(temp_board1, pos, dire)):
                                # Create deep copy of mutated board
                                temp_board2 = []
                                for row in temp_board1:
                                    temp_list = []
                                    for elem in row:
                                        temp_list.append(elem)
                                    temp_board2.append(temp_list)
                                    
                                temp_board2 = make_move(temp_board2, pos, dire)
                                if temp_board2 == winning_board:
                                    move.append((position, direction))
                                    move.append((pos, dire))
                                    found = True
                                    break
                                # Tests next possible moves until win is found
                                else:
                                    for posi in all_positions:
                                        for direc in all_directions:
                                            if (validate_input(temp_board2, posi, direc) 
                                                and legal_move(temp_board2, posi, direc)):
                                                temp_board3 = []
                                                for row in temp_board2:
                                                    temp_list = []
                                                    for elem in row:
                                                        temp_list.append(elem)
                                                    temp_board3.append(temp_list)
                                                    
                                                temp_board3 = make_move(temp_board3, posi, direc)
                                                if temp_board3 == winning_board:
                                                    move.append((position, direction))
                                                    move.append((pos, dire))
                                                    move.append((posi, direc))
                                                    found = True
                                                    break
                                               
                                        if found:
                                            break
                        if found:
                            break
        if found:
            break
    
    # Returns None if there is no sequence of three moves that lead to win
    if move == []:
        return None
    
    # Returns list of winning sequence of three moves if one exists
    return move

