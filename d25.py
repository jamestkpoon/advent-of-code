import sys


def get_loop_size(target, subject_number=7, mod=20201227):
    v, num_loops = 1, 0
    while v != target:
        v = (v * subject_number) % mod
        num_loops += 1

    return num_loops


def transform(subject_number, num_loops, mod=20201227):
    out = 1
    for _ in range(num_loops):
        out = (out * subject_number) % mod

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    public_keys = list(map(int, data))
    secret_loop_sizes = list(map(get_loop_size, public_keys))

    if sys.argv[2] == "1":
        out = transform(public_keys[0], secret_loop_sizes[1])
    # elif sys.argv[2] == '2':

    print(out)
