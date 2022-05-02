import sys
import numpy as np
from copy import deepcopy


def generate_pows(length=36):
    out = []
    for i in range(length):
        out.append(int(2**i))

    return out[::-1]


def register_to_decimal(register, pows):
    out = 0
    for i in range(len(register)):
        if register[i]:
            out += pows[i]

    return out


def decimal_to_register(decimal, pows):
    out = [False] * len(pows)
    n = decimal

    for i in range(len(pows)):
        if n >= pows[i]:
            out[i] = True
            n -= pows[i]

    return out


def apply_mask_1(register, mask):
    out = []
    for i in range(len(register)):
        if mask[i] == "1":
            out.append(True)
        elif mask[i] == "0":
            out.append(False)
        else:
            out.append(register[i])

    return out


def apply_mask_2(register, mask):
    out_raw = []
    x_indices = []
    for i in range(len(register)):
        if mask[i] == "1":
            out_raw.append(True)
        elif mask[i] == "0":
            out_raw.append(register[i])
        else:
            out_raw.append("X")
            x_indices.append(i)

    outs = []
    for n in np.ndindex(*([2] * len(x_indices))):
        out = deepcopy(out_raw)
        for i in range(len(x_indices)):
            out[x_indices[i]] = True if n[i] == 1 else False
        outs.append(out)

    return outs


class Program:
    def __init__(self, strs):
        self._pows = generate_pows()

        self._cmds = []
        cmd = {}
        for s in strs:
            if s[:7] == "mask = ":
                if cmd != {}:
                    self._cmds.append(cmd)
                    cmd = {}
                cmd["mask"] = s[7:]
                cmd["operations"] = []
            else:
                address = int(s[s.find("[") + 1 : s.find("]")])
                value = int(s[s.rfind(" ") + 1 :])
                cmd["operations"].append([address, value])

        if cmd != {}:
            self._cmds.append(cmd)

    def run_1(self):
        memory = {}
        for c in self._cmds:
            for op in c["operations"]:
                value_register = decimal_to_register(op[1], self._pows)
                memory[op[0]] = register_to_decimal(
                    apply_mask_1(value_register, c["mask"]), self._pows
                )

        return sum(memory.values())

    def run_2(self):
        memory = {}
        for c in self._cmds:
            for op in c["operations"]:
                address_registers = apply_mask_2(
                    decimal_to_register(op[0], self._pows), c["mask"]
                )
                for address_register in address_registers:
                    memory[register_to_decimal(address_register, self._pows)] = op[1]

        return sum(memory.values())


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    program = Program(data)

    if sys.argv[2] == "1":
        out = program.run_1()
    elif sys.argv[2] == "2":
        out = program.run_2()

    print(out)
