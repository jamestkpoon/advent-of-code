import sys


def split_and_strip_sep(s, sep=" "):
    sl = list(s)
    while sep in sl:
        sl.remove(sep)

    return sl


def calc_1(l):
    out = int(l[0])
    idx = 1
    while idx < len(l):
        next_int = int(l[idx + 1])
        if l[idx] == "+":
            out += next_int
        elif l[idx] == "*":
            out *= next_int
        else:
            print("  Got unrecognized operator symbol " + l[idx])

        idx += 2

    return out


def calc_2(l):
    while "+" in l:
        idx = l.index("+")
        l = l[: idx - 1] + [str(int(l[idx - 1]) + int(l[idx + 1]))] + l[idx + 2 :]

    out = 1
    for i in l[::2]:
        out *= int(i)

    return out


def parse(l, calc):
    while "(" in l:
        obi = len(l) - 1 - l[::-1].index("(")
        cbi = obi + 1
        while l[cbi] != ")":
            cbi += 1
        ans = calc(l[obi + 1 : cbi])
        l = l[:obi] + [str(ans)] + l[cbi + 1 :]

    return calc(l)


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    split_lines = [split_and_strip_sep(d) for d in data]

    if sys.argv[2] == "1":
        calc_fn = calc_1
    elif sys.argv[2] == "2":
        calc_fn = calc_2

    out = 0
    for sl in split_lines:
        out += parse(sl, calc_fn)

    print(out)
