import sys
from functools import reduce


def chinese_remainder(n, a):
    sum = 0
    prod = reduce(lambda a, b: a * b, n)
    for n_i, a_i in zip(n, a):
        p = prod // n_i
        sum += a_i * mul_inv(p, n_i) * p
    return sum % prod


def mul_inv(a, b):
    b0 = b
    x0, x1 = 0, 1
    if b == 1:
        return 1
    while a > 1:
        q = a // b
        a, b = b, a % b
        x0, x1 = x1 - q * x0, x0
    if x1 < 0:
        x1 += b0
    return x1


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    if sys.argv[2] == "1":
        t0 = int(data[0])
        intervals = sorted([int(x) for x in data[1].split(",") if x != "x"])
        t = t0
        out = None
        while out == None:
            for i in intervals:
                if t % i == 0:
                    out = i * (t - t0)
                    break
            t += 1
    elif sys.argv[2] == "2":
        offsets, values = [], []
        split = data[1].split(",")
        for i in range(len(split)):
            if split[i] != "x":
                offsets.append(-i)
                values.append(int(split[i]))

        out = chinese_remainder(values, offsets)

    print(out)
