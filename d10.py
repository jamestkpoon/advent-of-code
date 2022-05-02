import sys
from collections import Counter


def parse_list_to_dict_based_on_leap(sorted_list, max_leap=3):
    out = {}
    for i in range(len(sorted_list)):
        out[sorted_list[i]] = {}
        for j in range(i + 1, len(sorted_list)):
            if sorted_list[j] - sorted_list[i] <= max_leap:
                out[sorted_list[i]][sorted_list[j]] = 1

    return out


def cumulative_sum_down_tree_with_numerical_keys(dicto):
    out = {}
    desc_keys = sorted(dicto.keys())[::-1]
    out[desc_keys[0]] = 1
    for desc_key in desc_keys[1:]:
        out[desc_key] = 0
        for k in dicto[desc_key].keys():
            out[desc_key] += out[k]

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    ints = list(map(int, data))

    if sys.argv[2] == "1":
        ints.append(0)
        sorted = sorted(ints)
        diffs = [sorted[i] - sorted[i - 1] for i in range(1, len(sorted))]
        counter = Counter(diffs)
        out = counter[1] * (counter[3] + 1)
    elif sys.argv[2] == "2":
        max_leap = int(sys.argv[3])
        ints_sorted = sorted(ints)
        d = parse_list_to_dict_based_on_leap(ints_sorted, max_leap)
        cumsum_d = cumulative_sum_down_tree_with_numerical_keys(d)

        out = 0
        for i in ints_sorted:
            if i <= max_leap:
                out += cumsum_d[i]
            else:
                break

    print(out)
