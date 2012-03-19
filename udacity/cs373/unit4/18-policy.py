# ----------
# User Instructions:
# 
# Create a function optimum_policy() that returns
# a grid which shows the optimum policy for robot
# motion. This means there should be an optimum
# direction associated with each navigable cell.
# 
# un-navigable cells must contain an empty string
# WITH a space, as shown in the previous video.
# Don't forget to mark the goal with a '*'

# ----------

grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['^', '<', 'v', '>']

cost_step = 1 # the cost associated with moving from a cell to an adjacent one.

# ----------------------------------------
# modify code below
# ----------------------------------------

def optimum_policy():
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
        
        
    policy = []
    for i in range(len(grid)):
        policy.append([' '] * len(grid[0]))

    for i, line in enumerate(value):
        for j, col in enumerate(line):
            if value[i][j] == 99:
                policy[i][j] = ' '
                continue
                
            if goal[0] == i and goal[1] == j:
                policy[i][j] = '*'
                continue
                
            if grid[i][j]:
                policy[i][j] = ' '
                continue

            costs = []
            ds = []

            for d in delta:
                npos = [i + d[0], j + d[1]]

                if npos[0] < 0 or npos[0] >= len(grid) or npos[1] < 0 or npos[1] >= len(grid[0]):
                    continue

                costs.append(value[i + d[0]][j + d[1]])
                ds.append([d[0], d[1]])

            idx = costs.index(min(costs))
            d = ds[idx]
            policy[i][j] = delta_name[delta.index(d)]

    return policy # Make sure your function returns the expected grid.

for line in optimum_policy():
    print line
