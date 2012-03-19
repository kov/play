# -----------
# User Instructions:
# 
# Modify the function search() so that it returns
# a table of values called expand. This table
# will keep track of which step each node was
# expanded.
#
# For grading purposes, please leave the return
# statement at the bottom.
# ----------


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
    count = 0

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

        print position, count, expand
        expand[position[0]][position[1]] = count
        count = count + 1

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

    return expand #Leave this line for grading purposes!

for line in search():
    print line
