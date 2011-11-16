import random

# Borrowed from http://stackoverflow.com/questions/2140787/select-random-k-elements-from-a-list-whose-elements-have-weights/2149533#2149533
def weighted_sample(items):
    total = float(sum(w for w, v in items))
    i = 0
    w, v = items[0]

    x = total * (1 - random.random() ** (1.0 / 1))
    total -= x
    while x > w:
        x -= w
        i += 1
        w, v = items[i]
    w -= x
    return v
