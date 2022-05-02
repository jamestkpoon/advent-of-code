import sys


def parse_input(strs):
    outs = []

    line_idx = 0
    while line_idx < len(strs):
        line_idx += 1
        out = []

        while line_idx < len(strs) and len(strs[line_idx]) != 0:
            out.append(int(strs[line_idx]))
            line_idx += 1

        outs.append(out)
        line_idx += 1

    return outs


def combat(h1, h2):
    while len(h1) > 0 and len(h2) > 0:
        c1, c2 = h1.pop(0), h2.pop(0)
        if c1 > c2:
            h1 += [c1, c2]
        else:
            h2 += [c2, c1]

    return h1 if len(h1) > 0 else h2


def combat_evolved(h1, h2):
    history = []
    while len(h1) > 0 and len(h2) > 0:
        hash = hash(str([h1, h2]))
        if hash in history:
            return [h1, 1]
        else:
            history.append(hash)

        c1, c2 = h1.pop(0), h2.pop(0)

        if len(h1) >= c1 and len(h2) >= c2:
            h1 = [h1[i] for i in range(c1)]
            h2 = [h2[i] for i in range(c2)]
            winner = combat_evolved(h1, h2)
            if winner[1] == 1:
                h1 += [c1, c2]
            else:
                h2 += [c2, c1]
        else:
            if c1 > c2:
                h1 += [c1, c2]
            else:
                h2 += [c2, c1]

    return [h1, 1] if len(h1) > 0 else [h2, 2]


def score(h):
    out = 0
    for i in range(1, len(h) + 1):
        out += i * h[-i]

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    hand0, hand1 = parse_input(data)

    if sys.argv[2] == "1":
        out = score(combat(hand0, hand1))
    elif sys.argv[2] == "2":
        out = score(combat_evolved(hand0, hand1)[0])

    print(out)
