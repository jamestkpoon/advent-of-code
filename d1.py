import sys
import numpy as np


def filter_let(l, let):
    return list(filter(lambda x: x <= let, l))


def get_unique_index_sets(l, n):
    # full axis sets
    ranges = [range(l) for i in range(n)]
    axes = np.mgrid[ranges].T.reshape(-1, n)
    # only keep sets with all unique values
    axes = np.asarray(list(filter(lambda r: len(set(r)) == n, axes)))
    # sort columns and then keep unique rows
    axes = np.unique(np.sort(axes, axis=1), axis=0)

    return axes


def find_n_values_that_sum_to(l, n, v):
    for I in get_unique_index_sets(len(l), n):
        subset = [l[i] for i in I]
        if sum(subset) == v:
            return subset

    return None


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        ints = list(map(int, f.readlines()))

    n = int(sys.argv[2])
    sum_target = int(sys.argv[3])

    ints_filtered = filter_let(ints, sum_target)
    v = find_n_values_that_sum_to(ints_filtered, n, sum_target)
    print([v, np.prod(v)])
