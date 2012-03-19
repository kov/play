# ----------
# User Instructions:
#
# Modify the the search function so that it returns
# a shortest path as follows:
# 
# [['>', 'v', ' ', ' ', ' ', ' '],
#  [' ', '>', '>', '>', '>', 'v'],
#  [' ', ' ', ' ', ' ', ' ', 'v'],
#  [' ', ' ', ' ', ' ', ' ', 'v'],
#  [' ', ' ', ' ', ' ', ' ', '*']]
#
# Where '>', '<', '^', and 'v' refer to right, left, 
# up, and down motions. NOTE: the 'v' should be 
# lowercase.
#
# Your function should be able to do this for any
# provided grid, not just the sample grid below.
# ----------


# Sample Test case
grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 1, 0, 1, 0],
        [0, 0, 1, 0, 1, 0],
        [0, 0, 1, 0, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['^', '<', 'v', '>']

cost = 1

# ----------------------------------------
# modify code below
# ----------------------------------------

def search():
    closed = []
    frontier = { tuple(init) : 0 }
    expand = []
    for i in range(len(grid)):
        expand.append([-1] * len(grid[0]))

    while True:
        if not frontier:
            final = 'fail'
            break

        minpair = {}
        for position, pos_cost in frontier.items():
            if not minpair:
                minpair = { 'position': tuple(position), 'cost': pos_cost }
                continue

            if pos_cost < minpair['cost']:
                minpair = { 'position': tuple(position), 'cost': pos_cost }

        position = minpair['position']

        if position == tuple(goal):
            final = [minpair['cost'], goal[0], goal[1]]
            break

        expand[position[0]][position[1]] = minpair['cost']

        del frontier[position]
        closed.append(position)

        for d in delta:
            newpos = (position[0] + d[0], position[1] + d[1])

            # check for out of bounds
            if newpos[0] < 0 or newpos[0] >= len(grid) \
                    or newpos[1] < 0 or newpos[1] >= len(grid[0]):
                continue

            # this is not an open block
            if grid[newpos[0]][newpos[1]]:
                closed.append(newpos)

            # check for visited position
            if newpos in closed:
                continue

            frontier[tuple(newpos)] = minpair['cost'] + cost

    optimal_path = [goal]
    position = goal
    closed = []
    while True:
        costs = []
        positions = []

        for d in delta:
            newpos = (position[0] + d[0], position[1] + d[1])

            # check for out of bounds
            if newpos[0] < 0 or newpos[0] >= len(grid) \
                    or newpos[1] < 0 or newpos[1] >= len(grid[0]):
                continue

            # this is not an open block
            if expand[newpos[0]][newpos[1]] == -1:
                continue
                
            if newpos in closed:
                continue

            costs.append(expand[newpos[0]][newpos[1]])
            positions.append(newpos)

        mincost = min(costs)
        idx = costs.index(mincost)
        optimal_path.append(positions[idx])

        position = positions[idx]
        closed.append(position)
        if position[0] == init[0] and position[1] == init[1]:
            break

    pathmap = []
    for i in range(len(grid)):
        pathmap.append([' '] * len(grid[0]))

    optimal_path.reverse()
    for i, pos in enumerate(optimal_path):
        try:
            nextpos = optimal_path[i + 1]
            if nextpos[0] > pos[0]:
                arrow = 'v'
            elif nextpos[0] < pos[0]:
                arrow = '^'
            elif nextpos[1] > pos[1]:
                arrow = '>'
            else:
                arrow = '<'
            pathmap[pos[0]][pos[1]] = arrow
        except IndexError:
            pathmap[pos[0]][pos[1]] = '*'
        
    return pathmap

for line in search():
	print line
