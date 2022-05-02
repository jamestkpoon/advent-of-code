import sys
import numpy as np


def parse_input(strs):
    outs = []
    for s in strs:
        out = []
        idx = 0
        while idx < len(s):
            l = 1 if s[idx] == "e" or s[idx] == "w" else 2
            out.append(s[idx : idx + l])
            idx += l
        outs.append(out)

    return outs


def navigate_hex_grid(sequence):
    pos = [0, 0]
    for step in sequence:
        if step == "w":
            pos[1] += 2
        elif step == "e":
            pos[1] -= 2
        else:
            if step[0] == "n":
                pos[0] += 1
            else:
                pos[0] -= 1
            if step[1] == "w":
                pos[1] += 1
            else:
                pos[1] -= 1

    return pos


def get_list_of_tiles_to_flip(sequences):
    out = []
    for s in sequences:
        pos = navigate_hex_grid(s)
        if pos not in out:
            out.append(pos)
        else:
            out.remove(pos)

    return out


def get_neighbors(pos):
    return [
        [pos[0] + 0, pos[1] + 2],
        [pos[0] + 0, pos[1] - 2],
        [pos[0] + 1, pos[1] + 1],
        [pos[0] + 1, pos[1] - 1],
        [pos[0] - 1, pos[1] + 1],
        [pos[0] - 1, pos[1] - 1],
    ]


def is_valid_coord(pos):
    abs = np.abs(pos)
    if abs[0] % 2 != 0 and abs[1] % 2 == 0:
        return False
    elif abs[0] % 2 == 0 and abs[1] % 2 != 0:
        return False
    else:
        return True


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    sequences = parse_input(data)
    flip_list = get_list_of_tiles_to_flip(sequences)

    if sys.argv[2] == "1":
        out = len(flip_list)
    elif sys.argv[2] == "2":
        black_tiles = flip_list
        for day in range(100):
            minimum, maximum = (
                np.min(black_tiles, axis=0) - 1,
                np.max(black_tiles, axis=0) + 2,
            )
            new_black_tiles = [t for t in black_tiles]
            for r in range(minimum[0], maximum[0]):
                for c in range(minimum[1], maximum[1]):
                    coord = [r, c]
                    if not is_valid_coord(coord):
                        continue
                    num_black_neighbors = np.sum(
                        [n in black_tiles for n in get_neighbors(coord)]
                    )
                    if (
                        coord in black_tiles
                        and (num_black_neighbors == 0 or num_black_neighbors > 2)
                        and coord in new_black_tiles
                    ):
                        new_black_tiles.remove(coord)
                    elif (
                        coord not in black_tiles
                        and num_black_neighbors == 2
                        and coord not in new_black_tiles
                    ):
                        new_black_tiles.append(coord)

            black_tiles = new_black_tiles

        out = len(black_tiles)

    print(out)
