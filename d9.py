import sys
from d1 import find_n_values_that_sum_to


def find_value_that_doesnt_sum_from_prev(ints, candidates_length, n):
    for i in range(candidates_length, len(ints)):
        if (
            find_n_values_that_sum_to(ints[i - candidates_length : i], n, ints[i])
            is None
        ):
            return i, ints[i]

    return None


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    ints = list(map(int, data))

    if sys.argv[2] == "1":
        out = find_value_that_doesnt_sum_from_prev(
            ints, int(sys.argv[3]), int(sys.argv[4])
        )
    elif sys.argv[2] == "2":
        idx_from_last = int(sys.argv[3])
        found = False

        for i in range(idx_from_last - 2):
            for j in range(i + 2, idx_from_last):
                candidates = ints[i:j]
                if sum(candidates) == ints[idx_from_last]:
                    out = min(candidates) + max(candidates)
                    found = True
                    break
            if found:
                break

    print(out)
