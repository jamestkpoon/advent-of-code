import sys


def separate_passports(lines):
    passports = []

    passport = {}
    for line in lines:
        if len(line) == 0 and len(passport.keys()) != 0:
            passports.append(passport)
            passport = {}
        else:
            while len(line) != 0:
                last_space_idx = line.rfind(" ")
                chunk = line[last_space_idx + 1 :] if last_space_idx != -1 else line
                colon_pos = chunk.find(":")
                passport[chunk[:colon_pos]] = chunk[colon_pos + 1 :]
                line = line[:last_space_idx] if last_space_idx != -1 else ""

    if len(passport.keys()) != 0:
        passports.append(passport)

    return passports


def passport_has_all_keys(passport, keys):
    for key in keys:
        if key not in passport.keys():
            return False

    return True


def cast_year_and_check_inclusive_range(s, l, u):
    if len(s) != 4:
        return False

    try:
        si = int(s)
        return si >= l and si <= u
    except:
        return False


def is_valid_passport_id(s, length=9):
    if len(s) != length:
        return False

    try:
        int(s)
        return True
    except:
        return False


def is_valid_hair_color(s, length=7, header="#"):
    if len(s) != length:
        return False
    if s[: len(header)] != header:
        return False

    for c in map(ord, s[1:]):
        if not ((c >= ord("0") and c <= ord("9")) or (c >= ord("a") and c <= ord("f"))):
            return False

    return True


def is_valid_height(s, range_dict):
    for k, v in range_dict.items():
        neg_key_length = -len(k)
        if s[neg_key_length:] == k:
            try:
                h = int(s[:neg_key_length])
                return h >= v[0] and h <= v[1]
            except:
                return False

    return False


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    passports = separate_passports(data)
    keys = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
    valid = 0

    if sys.argv[2] == "1":
        for passport in passports:
            if passport_has_all_keys(passport, keys):
                valid += 1
    elif sys.argv[2] == "2":
        for passport in passports:
            if not passport_has_all_keys(passport, keys):
                continue

            if not cast_year_and_check_inclusive_range(passport["byr"], 1920, 2002):
                continue
            if not cast_year_and_check_inclusive_range(passport["iyr"], 2010, 2020):
                continue
            if not cast_year_and_check_inclusive_range(passport["eyr"], 2020, 2030):
                continue
            if passport["ecl"] not in ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]:
                continue
            if not is_valid_passport_id(passport["pid"]):
                continue
            if not is_valid_hair_color(passport["hcl"]):
                continue
            if not is_valid_height(passport["hgt"], {"cm": [150, 193], "in": [59, 76]}):
                continue

            valid += 1

    print(valid)
