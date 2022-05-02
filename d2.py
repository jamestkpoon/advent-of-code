import sys


def separate_string(s):
    first_hyphen_idx = s.find("-")
    first_space_idx = s.find(" ")
    first_colon_idx = s.find(":")
    last_space_idx = s.rfind(" ")

    a = int(s[:first_hyphen_idx])
    b = int(s[first_hyphen_idx + 1 : first_space_idx])
    c = s[first_space_idx + 1 : first_colon_idx]
    d = s[last_space_idx + 1 :]
    if d[-1] == "\n":
        d = d[:-1]

    return [a, b, c, d]


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.readlines()

    total_valid = 0
    if sys.argv[2] == "1":
        for d in data:
            [minimum, maximum, target, seq] = separate_string(d)
            count = seq.count(target)
            if count >= minimum and count <= maximum:
                total_valid += 1
    elif sys.argv[2] == "2":
        for d in data:
            [pos1, pos2, target, seq] = separate_string(d)
            sseq = [seq[pos1 - 1], seq[pos2 - 1]]
            if sseq.count(target) == 1:
                total_valid += 1

    print(total_valid)
