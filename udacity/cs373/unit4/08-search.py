# ----------
# User Instructions:
#
# Define a function, search() that takes no input
# and returns a list
# in the form of [optimal path length, x, y]. For
# the grid shown below, your function should output
# [11, 4, 5].
#
# If there is no valid path from the start point
# to the goal, your function should return the string
# 'fail'
# ----------

# Grid format:
#   0 = Navigable space
#   1 = Occupied space

grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1] # Make sure that the goal definition stays in the function.

delta = [[-1, 0 ], # go up
        [ 0, -1], # go left
        [ 1, 0 ], # go down
        [ 0, 1 ]] # go right

delta_name = ['^', '<', 'v', '>']

cost = 1

def search():
    closed = []
    frontier = { tuple(init) : 0 }

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

    print final

search()
