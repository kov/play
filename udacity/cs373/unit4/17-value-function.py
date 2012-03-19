# ----------
# User Instructions:
# 
# Create a function compute_value() which returns
# a grid of values. Value is defined as the minimum
# number of moves required to get from a cell to the
# goal. 
#
# If it is impossible to reach the goal from a cell
# you should assign that cell a value of 99.

# ----------

grid = [[0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['^', '<', 'v', '>']

cost_step = 1 # the cost associated with moving from a cell to an adjacent one.

# ----------------------------------------
# insert code below
# ----------------------------------------

def compute_value():
    value = []
    for i in range(len(grid)):
        value.append([99] * len(grid[0]))

    value[goal[0]][goal[1]] = 0
    frontier = [tuple(goal)]
    closed = []
    while True:
        if not frontier:
            break
            
        pos = frontier[0]
        for d in delta:
            npos = (pos[0] + d[0], pos[1] + d[1])

            if npos in closed or npos in frontier:
                continue

            if npos[0] < 0 or npos[0] >= len(grid) or npos[1] < 0 or npos[1] >= len(grid[0]):
                continue

            if grid[npos[0]][npos[1]]:
                value[npos[0]][npos[1]] = 99
                continue
                
            value[npos[0]][npos[1]] = value[pos[0]][pos[1]] + cost_step
            frontier.append(npos)

        frontier.remove(pos)
        closed.append(pos)
    return value #make sure your function returns a grid of values as demonstrated in the previous video.


for line in compute_value():
    for i in line:
        print '%2d' % (i),
    print ''
