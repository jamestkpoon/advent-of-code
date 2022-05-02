import sys


def parse(strs):
    rules = {}

    line_idx = 0
    while len(strs[line_idx]) != 0:
        line = strs[line_idx]
        line_idx += 1

        key = line[: line.find(":")]
        lb1 = int(line[line.find(": ") + 2 : line.find("-")])
        ub1 = int(line[line.find("-") + 1 : line.find(" or")])
        lb2 = int(line[line.rfind(" ") + 1 : line.rfind("-")])
        ub2 = int(line[line.rfind("-") + 1 :])

        rules[key] = [lb1, ub1, lb2, ub2]

    line_idx += 2
    own_ticket = list(map(int, strs[line_idx].split(",")))

    line_idx += 3
    other_tickets = []
    for line in strs[line_idx:]:
        other_tickets.append(list(map(int, line.split(","))))

    return rules, own_ticket, other_tickets


def rule_check(v, rule):
    if v >= rule[0] and v <= rule[1]:
        return True
    elif v >= rule[2] and v <= rule[3]:
        return True
    else:
        return False


def ticket_check(ticket, rules):
    out = None
    for v in ticket:
        valid = False
        for rule in rules.values():
            rule_valid = rule_check(v, rule)
            if rule_valid:
                valid = True
                break
        if not valid:
            out = out + v if out is not None else v

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    rules, own_ticket, other_tickets = parse(data)

    if sys.argv[2] == "1":
        out = 0
        for other_ticket in other_tickets:
            res = ticket_check(other_ticket, rules)
            if res is not None:
                out += res
    elif sys.argv[2] == "2":
        other_tickets = list(
            filter(lambda t: ticket_check(t, rules) is None, other_tickets)
        )
        valid_keys_all = {}
        for c in range(len(own_ticket)):
            valid_keys = []
            for key, rule in rules.items():
                valid = True
                for other_ticket in other_tickets:
                    if not rule_check(other_ticket[c], rule):
                        valid = False
                        break
                if valid:
                    valid_keys.append(key)

            valid_keys_all[c] = valid_keys

        ordered_keys = [""] * len(own_ticket)
        unidentified_columns = list(range(len(own_ticket)))
        while len(unidentified_columns) > 0:
            for c in unidentified_columns:
                if len(valid_keys_all[c]) == 1:
                    key = valid_keys_all[c][0]
                    ordered_keys[c] = key
                    unidentified_columns.remove(c)
                    for v in valid_keys_all.values():
                        if key in v:
                            v.remove(key)
                    break

        out = 1
        for i in range(len(ordered_keys)):
            if "departure" in ordered_keys[i]:
                out *= own_ticket[i]

    print(out)
