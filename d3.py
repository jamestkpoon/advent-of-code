import sys
import numpy as np


def to_binary_map(strs, key="#"):
    out = np.empty([len(strs), len(strs[0])])
    for i in range(out.shape[0]):
        for j in range(out.shape[1]):
            out[i, j] = 1 if strs[i][j] == key else 0

    return out.astype(np.bool)


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    map = to_binary_map(data)

    out = 1
    for s in sys.argv[2:]:
        step = list(map(int, s.split(",")))
        pos = [0, 0]
        count = 0
        while pos[0] < map.shape[0]:
            if map[pos[0], pos[1]]:
                count += 1
            pos[0] += step[0]
            pos[1] = (pos[1] + step[1]) % map.shape[1]

        out *= count

    print(out)
