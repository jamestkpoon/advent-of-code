import sys
import numpy as np

FLOORSPACE_VALUE = 0
EMPTY_SEAT_VALUE = 1
OCCUPIED_SEAT_VALUE = 2


def parse_seats(strs, floorspace=".", empty_seat="L", occupied_seat="#"):
    out = np.empty([len(strs), len(strs[0])], dtype=np.uint8)
    for r in range(out.shape[0]):
        for c in range(out.shape[1]):
            if strs[r][c] == floorspace:
                out[r, c] = FLOORSPACE_VALUE
            elif strs[r][c] == empty_seat:
                out[r, c] = EMPTY_SEAT_VALUE
            elif strs[r][c] == occupied_seat:
                out[r, c] = OCCUPIED_SEAT_VALUE
            else:
                print("Got unrecognized character " + strs[r][c])

    return out


def get_immediate_neighbors(map, _r, _c):
    out = []
    for r in range(_r - 1, _r + 2):
        if r < 0 or r >= map.shape[0]:
            continue
        for c in range(_c - 1, _c + 2):
            if c < 0 or c >= map.shape[1] or (r == _r and c == _c):
                continue
            out.append(map[r, c])

    return out


def get_raycast_neighbors(map, _r, _c):
    def within_bounds(coord):
        if np.any(coord < 0):
            return False
        elif not np.all(coord < map.shape):
            return False
        else:
            return True

    out = []
    origin = [_r, _c]
    for n in np.ndindex(3, 3):
        if n == (1, 1):
            continue
        step = np.asarray(n) - 1
        pos = np.array(origin, copy=True)
        while True:
            pos += step
            if not within_bounds(pos):
                break
            if map[pos[0], pos[1]] != FLOORSPACE_VALUE:
                out.append(map[pos[0], pos[1]])
                break

    return out


def iterate(map, get_neighbors_fn, occupied_seat_threshold):
    out = map.copy()
    for r in range(map.shape[0]):
        for c in range(map.shape[1]):
            if map[r, c] == FLOORSPACE_VALUE:
                continue
            num_occupied_neighbors = get_neighbors_fn(map, r, c).count(
                OCCUPIED_SEAT_VALUE
            )
            if map[r, c] == EMPTY_SEAT_VALUE:
                if num_occupied_neighbors == 0:
                    out[r, c] = OCCUPIED_SEAT_VALUE
            elif map[r, c] == OCCUPIED_SEAT_VALUE:
                if num_occupied_neighbors >= occupied_seat_threshold:
                    out[r, c] = EMPTY_SEAT_VALUE

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    initial_seating_map = parse_seats(data)
    neighbors_fn = (
        get_immediate_neighbors if sys.argv[2] == "1" else get_raycast_neighbors
    )
    occupied_seat_threshold = int(sys.argv[3])

    out = None
    last_map = initial_seating_map
    while True:
        map = iterate(last_map, neighbors_fn, occupied_seat_threshold)
        if np.array_equal(map, last_map):
            out = sum(map.ravel() == OCCUPIED_SEAT_VALUE)
            break
        last_map = map

    print(out)
