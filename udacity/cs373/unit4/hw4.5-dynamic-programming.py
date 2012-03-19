# --------------
# USER INSTRUCTIONS
#
# Write a function called stochastic_value that 
# takes no input and RETURNS two grids. The
# first grid, value, should contain the computed
# value of each cell as shown in the video. The
# second grid, policy, should contain the optimum
# policy for each cell.
#
# Stay tuned for a homework help video! This should
# be available by Thursday and will be visible
# in the course content tab.
#
# Good luck! Keep learning!
#
# --------------
# GRADING NOTES
#
# We will be calling your stochastic_value function
# with several different grids and different values
# of success_prob, collision_cost, and cost_step.
# In order to be marked correct, your function must
# RETURN (it does not have to print) two grids,
# value and policy.
#
# When grading your value grid, we will compare the
# value of each cell with the true value according
# to this model. If your answer for each cell
# is sufficiently close to the correct answer
# (within 0.001), you will be marked as correct.
#
# NOTE: Please do not modify the values of grid,
# success_prob, collision_cost, or cost_step inside
# your function. Doing so could result in your
# submission being inappropriately marked as incorrect.

# -------------
# GLOBAL VARIABLES
#
# You may modify these variables for testing
# purposes, but you should only modify them here.
# Do NOT modify them inside your stochastic_value
# function.

grid = [[0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 1, 1, 0]]

#grid = [[0, 0, 0],
#        [0, 0, 0]]
       
goal = [0, len(grid[0])-1] # Goal is in top right corner


delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['^', '<', 'v', '>'] # Use these when creating your policy grid.

success_prob = 0.5                     
failure_prob = (1.0 - success_prob)/2.0 # Probability(stepping left) = prob(stepping right) = failure_prob
collision_cost = 100                    
cost_step = 1        
                     

############## INSERT/MODIFY YOUR CODE BELOW ##################
#
# You may modify the code below if you want, but remember that
# your function must...
#
# 1) ...be called stochastic_value().
# 2) ...NOT take any arguments.
# 3) ...return two grids: FIRST value and THEN policy.

def stochastic_value():
    value = []
    for i in range(len(grid)):
        value.append([1000.] * len(grid[0]))


    policy = []
    for i in range(len(grid)):
        policy.append([' '] * len(grid[0]))


    value[goal[0]][goal[1]] = 0.
    policy[goal[0]][goal[1]] = '*'

    # This variable is used for tracking convergence.
    changed = True
    while changed:
        changed = False
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

                if npos[0] < 0 or npos[0] >= len(grid) or npos[1] < 0 or npos[1] >= len(grid[0]) \
                    or grid[npos[0]][npos[1]]:
                    continue
                
                policies = []
                costs = []
                for nd in delta:
                    # Go in that direction
                    tpos = (npos[0] + nd[0], npos[1] + nd[1])
                    if tpos[0] < 0 or tpos[0] >= len(grid) or tpos[1] < 0 or tpos[1] >= len(grid[0]) \
                        or grid[tpos[0]][tpos[1]]:
                        tcost = collision_cost
                    else:
                        tcost = value[tpos[0]][tpos[1]]
                    
                    lpos = (npos[0] + nd[1], npos[1] + nd[0])
                    if lpos[0] < 0 or lpos[0] >= len(grid) or lpos[1] < 0 or lpos[1] >= len(grid[0]) \
                        or grid[lpos[0]][lpos[1]]:
                        lcost = collision_cost
                    else:
                        lcost = value[lpos[0]][lpos[1]]

                    rpos = (npos[0] - nd[1], npos[1] - nd[0])
                    if rpos[0] < 0 or rpos[0] >= len(grid) or rpos[1] < 0 or rpos[1] >= len(grid[0]) \
                        or grid[rpos[0]][rpos[1]]:
                        rcost = collision_cost
                    else:
                        rcost = value[rpos[0]][rpos[1]]
                        
                    costs.append(tcost * success_prob + lcost * failure_prob + rcost * failure_prob + cost_step)
                    policies.append(delta_name[delta.index(nd)])

                mincost = min(costs)

                # If we are changing any value by more than 0.0001 we haven't converged.
                if abs(mincost - value[npos[0]][npos[1]]) > 0.0001:
                    changed = True

                value[npos[0]][npos[1]] = mincost
                policy[npos[0]][npos[1]] = policies[costs.index(mincost)]
                frontier.append(npos)

            frontier.remove(pos)
            closed.append(pos)

    return value, policy

value, policy = stochastic_value()  
for line in value:
    print line
for line in policy:
    print line
    
