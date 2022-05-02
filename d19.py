import sys
from collections import Counter


def parse_input(strs):
    idx = 0

    rules = {}
    while len(strs[idx]) != 0:
        split = strs[idx].split(" ")
        if split[1] == '"a"':
            rule = "a"
        elif split[1] == '"b"':
            rule = "b"
        elif "|" not in split:
            rule = [list(map(int, split[1:]))]
        else:
            pipe_index = split.index("|")
            ruleA = list(map(int, split[1:pipe_index]))
            ruleB = list(map(int, split[pipe_index + 1 :]))
            rule = [ruleA, ruleB]

        rules[int(split[0][:-1])] = rule
        idx += 1

    return rules, strs[idx + 1 :]


def find_terminal_keys(rules):
    t, nt = [], []
    for k, v in rules.items():
        if type(v) is str:
            t.append(k)
        else:
            nt.append(k)

    return t, nt


def cyk(input_string, rules, terminal_keys, non_terminal_keys, target_check_fn):
    if input_string in [rules[k] for k in terminal_keys]:
        return True

    input_string_length = len(input_string)
    keys_and_strides = [None] * input_string_length
    for i in range(input_string_length):
        for k in terminal_keys:
            if rules[k][0] == input_string[i]:
                keys_and_strides[i] = [[k, 1]]
                break

    def get_rule_stride(c, rule):
        cc = c
        for rule_key in rule:
            ok = False
            if cc >= input_string_length:
                return None
            for key, stride in keys_and_strides[cc]:
                if key == rule_key:
                    ok = True
                    cc += stride
                    break
            if not ok:
                return None

        return cc - c

    while True:
        new_accumulations = False
        for c in range(input_string_length):
            for k in non_terminal_keys:
                if k in [key_and_stride[0] for key_and_stride in keys_and_strides[c]]:
                    continue
                for rule in rules[k]:
                    stride = get_rule_stride(c, rule)
                    if stride is not None:
                        keys_and_strides[c].append([k, stride])
                        new_accumulations = True
                        break
        if target_check_fn(keys_and_strides):
            return True
        elif not new_accumulations:
            return False


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    rules, received_messages = parse_input(data)
    terminal_keys, non_terminal_keys = find_terminal_keys(rules)

    if sys.argv[2] == "1":

        def target_check_fn(keys_and_strides):
            for key, stride in keys_and_strides[0]:
                if key == 0 and stride == len(keys_and_strides):
                    return True
            return False

    elif sys.argv[2] == "2":

        def target_check_fn(keys_and_strides):
            c, keys = 0, ""
            while c < len(keys_and_strides):
                hit = False
                for key, stride in keys_and_strides[c]:
                    if key == 42 or key == 31:
                        c += stride
                        keys += chr(key)
                        hit = True
                        break
                if not hit:
                    return False
            counter = Counter(keys)
            return (
                c == len(keys_and_strides)
                and counter[chr(42)] > counter[chr(31)]
                and keys.find(chr(31)) > keys.rfind(chr(42))
            )

    print(
        sum(
            [
                cyk(m, rules, terminal_keys, non_terminal_keys, target_check_fn)
                for m in received_messages
            ]
        )
    )
