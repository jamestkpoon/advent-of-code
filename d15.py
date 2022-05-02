import sys
from copy import deepcopy


def play(seed, stop):
    series = deepcopy(list(seed))

    d = {}
    for i in range(len(series)):
        d[series[i]] = [i]

    turn_idx = len(series) - 1
    while len(series) != stop:
        turn_idx += 1
        prev_number = series[-1]
        if prev_number not in d.keys():
            series.append(0)
        elif len(d[prev_number]) < 2:
            series.append((turn_idx - 1) - d[prev_number][-1])
        else:
            series.append(d[prev_number][-1] - d[prev_number][-2])

        if series[-1] in d.keys():
            d[series[-1]].append(turn_idx)
        else:
            d[series[-1]] = [turn_idx]

    return series[-1]


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    input = list(map(int, data[0].split(",")))
    out = play(input, int(sys.argv[3]))

    print(out)
