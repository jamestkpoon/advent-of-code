import sys


def split_input_into_groups(s):
    chunks = []
    chunk = []

    for l in s:
        if len(l) != 0:
            chunk.append(l)
        elif len(chunk) != 0:
            chunks.append(chunk)
            chunk = []

    if len(chunk) != 0:
        chunks.append(chunk)

    return chunks


def find_common_answers(answers_group):
    common_answers = []
    for answer in answers_group:
        for c in answer:
            if c not in common_answers:
                common_answers.append(c)

    return common_answers


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    answers_groups = split_input_into_groups(data)
    out = 0

    if sys.argv[2] == "1":
        for answers_group in answers_groups:
            out += len(find_common_answers(answers_group))

    if sys.argv[2] == "2":
        for answers_group in answers_groups:
            common_answers = find_common_answers(answers_group)
            for c in common_answers:
                common = True
                for answer in answers_group:
                    if c not in answer:
                        common = False
                        break
                if common:
                    out += 1

    print(out)
