import sys


def parse_bag_dict(lines):
    out = {}
    for line in lines:
        key = line[: line.find(" bags")]
        children = {}

        if "no other" not in line:
            for content in line[line.find("contain ") + 8 :].split(", "):
                first_space_idx = content.find(" ")
                last_space_idx = content.rfind(" ")
                quantity = int(content[:first_space_idx])
                color = content[first_space_idx + 1 : last_space_idx]
                children[color] = quantity

        out[key] = children

    return out


def check_if_key_leads_down_to(dict, start_key, desired_key):
    frontier_keys = dict[start_key].keys()
    if desired_key in frontier_keys:
        return True

    while len(frontier_keys) != 0:
        frontier_keys_new = set()
        for frontier_key in frontier_keys:
            child_keys = dict[frontier_key].keys()
            if desired_key in child_keys:
                return True
            frontier_keys_new = frontier_keys_new.union(child_keys)
        frontier_keys = frontier_keys_new

    return False


def count_child_values(dicto, start_key, multiplier=1):
    out = multiplier * sum(list(dicto[start_key].values()))
    for k, v in dicto[start_key].items():
        out += count_child_values(dicto, k, v * multiplier)

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    bag_dict = parse_bag_dict(data)

    if sys.argv[2] == "1":
        out = 0
        for key in bag_dict.keys():
            if key == sys.argv[3]:
                continue
            if check_if_key_leads_down_to(bag_dict, key, sys.argv[3]):
                out += 1
    elif sys.argv[2] == "2":
        out = count_child_values(bag_dict, sys.argv[3])

    print(out)
