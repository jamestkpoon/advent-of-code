import sys


def binary_split(lb_inclusive, ub_exclusive, splitting_seq, splitting_keys):
    bounds = [lb_inclusive, ub_exclusive]
    for c in splitting_seq:
        half_diff = int((bounds[1] - bounds[0]) / 2)
        if c == splitting_keys[0]:
            bounds[1] -= half_diff
        elif c == splitting_keys[1]:
            bounds[0] += half_diff

    return bounds[0] if bounds[1] - bounds[0] == 1 else None


def get_seat_id(splitting_seq):
    r = binary_split(0, 128, splitting_seq[:-3], ["F", "B"])
    c = binary_split(0, 8, splitting_seq[-3:], ["L", "R"])
    if r is not None and c is not None:
        return r * 8 + c
    else:
        return None


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    if sys.argv[2] == "1":
        out = 0
        for d in data:
            seat_id = get_seat_id(d)
            if seat_id is not None:
                out = max(out, seat_id)
    elif sys.argv[2] == "2":
        id_all = []
        for d in data:
            seat_id = get_seat_id(d)
            if seat_id is not None:
                id_all.append(seat_id)

        out = None
        for i in range(8, 127 * 8):
            if i not in id_all:
                out = i
                break

    print(out)
