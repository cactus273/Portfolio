# Reach the Target Game
# Made by Jesslyn Andriono, March 2023

def format_row(row):
    row_string = ""

    for elem in row:
        if (len(elem) == 2):
            row_string += elem
        elif (elem == "|"):
            row_string += (elem + " ")
        else:
            row_string += "  "
            
    return row_string

def format_grid(grid):
    grid_string = ""
    
    for row in grid:
        grid_string += (format_row(row) + "\n")
                        
    grid_string = grid_string[:-1]
    
    return grid_string

def remove_player(grid, coord):
    if (grid[coord[0] - 1][coord[1] - 1] in "|x|T"):
        grid[coord[0] - 1][coord[1] - 1] = "|"
    else:
        grid[coord[0] - 1][coord[1] - 1] = " "
        
    return grid

def insert(grid, coord, character):
    coord = list(coord)
    coord[0] -= 1
    coord[1] -= 1
    
    if (grid[coord[0]][coord[1]] in "|T"):
        grid[coord[0]][coord[1]] = ("|" + character)
    else:
        grid[coord[0]][coord[1]] = (" " + character)
        
    return grid

def find_player(grid):
    player_location = [0, 0]
    row_number = 1
    column_number = 1
    boolean = True
    
    while (boolean == True):
        for i, row in enumerate(grid):
            for j, elem in enumerate(row):
                if (elem == "|x") or (elem == " x"):
                    player_location[0] = i + 1
                    player_location[1] = j + 1
                    boolean = False

    player_location = tuple(player_location)   
    return player_location

def move_player(grid, new_location):
    current_location = find_player(grid)
    remove_player(grid, current_location)
    return insert(grid, new_location, 'x')

def allowed_directions(grid, coord):
    allowed_directions = []
    coord = list(coord)
    coord[0] -= 1
    coord[1] -= 1
    count = 0
    
    if (coord[0] != 0):
        allowed_directions += "U"
        
    if (coord[0] != (len(grid))):
        allowed_directions += "D"
        
    if (coord[1] != 0) and (grid[coord[0]][coord[1]] != "|x"):
        allowed_directions += "L"
        
    if (coord[1] != (len(grid[0]))) and (grid[coord[0]][coord[1] + 1] != "|"):
        allowed_directions += "R"
        
    return allowed_directions

def convert_letters_to_coords(player_coords, letter):
    player_coords = list(player_coords)
    
    if letter in "U":
        player_coords[0] -= 1
    elif letter in "D":
        player_coords[0] += 1
    elif letter in "L":
        player_coords[1] -= 1
    elif letter in "R":
        player_coords[1] += 1
        
    return tuple(player_coords)

def get_random_start_location(grid):
    import random
    
    player_location = (random.randint(1, len(grid)), 
                       random.randint(1, len(grid[0]) - 1))
    target_location = (random.randint(1, len(grid)), 
                       random.randint(1, len(grid[0]) - 1))
    
    if player_location == target_location:
        target_location = (random.randint(1, len(grid)), 
                           random.randint(1, len(grid[0]) - 1))
        
    coords = (player_location, target_location)
    return coords

def play(grid, target):
    boolean = True
    number_of_steps = 0
    directions_list = []
    print(format_grid(grid) + "\n")
    new_coord = (0, 0)
    
    while boolean == True:
        letter = input("Give a direction (L,R,U,D): ").upper()
        
        current_coord = find_player(grid)
        
        if letter in allowed_directions(grid, current_coord):
            remove_player(grid, current_coord)
            new_coord = convert_letters_to_coords(current_coord, letter)
            print(format_grid(insert(grid, new_coord, "x")) + "\n")
            number_of_steps += 1
            directions_list += letter
        else:
            print("Invalid direction.")
            
        if (new_coord == target):
            print("Congratulations, you've reached the target in ", end = "")
            print(f"{number_of_steps} steps, ", end = "")
            print(f"with the directions {directions_list}.")
            boolean = False
    
    play_data = (number_of_steps, directions_list)
    return play_data
    
grid = [['|','|','|','|','|'],
        ['|',' ',' ','|','|'],
        ['|','|','|',' ','|']]
x_coords = get_random_start_location(grid)[0]
target_coords = get_random_start_location(grid)[1]
grid = insert(grid, x_coords, 'x')
grid = insert(grid, target_coords, 'T')
num_steps, directions = play(grid, target_coords)