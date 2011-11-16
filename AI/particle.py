#!/usr/bin/python
import random
from utils import weighted_sample

# This is the world; the robot may sense true or false
world = [ 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 ]

# This is the initial set of particles, all with the same weights
particles = [ [0.1, 0], [0.1, 1], [0.1, 2], [0.1, 3],
              [0.1, 4], [0.1, 5], [0.1, 6], [0.1, 7],
              [0.1, 8], [0.1, 9], [0.1, 10], [0.1, 11],
              [0.1, 12], [0.1, 13], [0.1, 14] ]

# This is the robot's sensor. It has a 90% chance of sensing correctly.
def sense(position):
    try:
        actual = bool(world[position])
        # add stochacity
        possible = [bool(actual)] * 9 + [not bool(actual)]
        return random.sample(possible, 1)
    except IndexError:
        raise IndexError("You tried sensing %d but the world only goes from 0 to %d" % (position, len(world) - 1))

# measurement probability:
# P(1|1) = 0.9
# P(0|0) = 0.9

def particles_filter(particles, measurement):
    new_set = list(particles)
    total_weights = 0

    for i in range(len(particles)):
        j = weighted_sample(particles)

        # w' = P(measurement|x')
        if world[j]:
            x = 0.9
            if measurement:
                w = x * 0.9
            else:
                w = x * 0.1
        else:
            x = 0.1
            if measurement:
                w = x * 0.1
            else:
                w = x * 0.9

        new_set[j][0] = w
        total_weights += w

    for i in range(len(particles)):
        new_set[i][0] /= total_weights

    return new_set

def plot(real_position, particles):
    ws = [('%5.1f' % (w)) for w in world]
    print '\t', ws

    real_world = ['  0.0'] * len(world)
    real_world[real_position] = '  R  '
    print '\t', real_world

    robots_view = [('%5.1f' % (p[0] * 1000)) for p in particles]
    print '\t', robots_view

for robot_position in range(len(world)):
    measurement = sense(robot_position)
    particles = particles_filter(particles, measurement)

    print 'Iteration #%d' % (robot_position + 1)
    plot(robot_position, particles)

    # remove the last position
    particles.pop()
    particles = [[0.1, 0]] + [[p[0], p[1] + 1] for p in particles]

