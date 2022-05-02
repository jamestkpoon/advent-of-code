import sys
import numpy as np
from d3 import to_binary_map


def in_bounds(coord, shape):
    return np.all(np.logical_and(coord >= 0, coord < np.asarray(shape)))


def gen_neighbors(nd):
    out = []
    for n in np.ndindex(*([3] * nd)):
        if n.count(1) < nd:
            out.append(n)

    return np.asarray(out) - 1


def iterate(world, local_neighbors):
    out = np.zeros(np.asarray(world.shape) + 2, dtype=np.bool)
    for world_coord in np.ndindex(*out.shape):
        world_coord_old = np.asarray(world_coord) - 1

        active_neighbors = 0
        for n in filter(
            lambda c: in_bounds(c, world.shape), world_coord_old + local_neighbors
        ):
            if world[tuple(n)]:
                active_neighbors += 1

        if in_bounds(world_coord_old, world.shape):
            if world[tuple(world_coord_old)]:
                if active_neighbors == 2 or active_neighbors == 3:
                    out[world_coord] = True
            elif active_neighbors == 3:
                out[world_coord] = True
        elif active_neighbors == 3:
            out[world_coord] = True

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    if sys.argv[2] == "1":
        world = to_binary_map(data)[np.newaxis, :, :]
    elif sys.argv[2] == "2":
        world = to_binary_map(data)[np.newaxis, np.newaxis, :, :]

    local_neighbors = gen_neighbors(len(world.shape))

    for i in range(6):
        world = iterate(world, local_neighbors)
    out = np.sum(world)

    print(out)
