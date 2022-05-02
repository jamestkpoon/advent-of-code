import sys


class LinkedListItem:
    def __init__(self, data):
        self._data = data
        self._prev = None
        self._next = None

    def get_data(self):
        return self._data

    def set_prev(self, x):
        self._prev = x

    def set_next(self, x):
        self._next = x

    def get_prev(self):
        return self._prev

    def get_next(self):
        return self._next


class CircularLinkedListOfInts:
    def __init__(self, ints):
        self._items = [LinkedListItem(i) for i in range(max(ints) + 1)]

        for i in range(len(ints) - 1):
            self._items[ints[i]].set_next(self._items[ints[i + 1]])
        for i in range(1, len(ints)):
            self._items[ints[i]].set_prev(self._items[ints[i - 1]])

        self._items[ints[0]].set_prev(self._items[ints[-1]])
        self._items[ints[-1]].set_next(self._items[ints[0]])

    def shift_chunk(self, chunk_start, chunk_size, new_previous_idx):
        new_previous = self._items[new_previous_idx]
        new_previous_next_old = new_previous.get_next()

        chunk_end = chunk_start
        for i in range(1, chunk_size):
            chunk_end = chunk_end.get_next()

        chunk_start.get_prev().set_next(chunk_end.get_next())
        chunk_end.get_next().set_prev(chunk_start.get_prev())

        chunk_start.set_prev(new_previous)
        new_previous.set_next(chunk_start)

        chunk_end.set_next(new_previous_next_old)
        new_previous_next_old.set_prev(chunk_end)

    def __getitem__(self, i):
        return self._items[i]


def play(ints, num_turns, chunk_size):
    ll = CircularLinkedListOfInts(ints)
    active_item = ll[ints[0]]
    minimum, maximum = min(ints), max(ints)
    next_values = [0] * chunk_size

    for _ in range(num_turns):
        target = active_item.get_data() - 1
        item = active_item
        for i in range(chunk_size):
            item = item.get_next()
            next_values[i] = item.get_data()

        while target in next_values:
            target -= 1
        if target < minimum:
            target = maximum
            while target in next_values:
                target -= 1

        ll.shift_chunk(active_item.get_next(), chunk_size, target)
        active_item = active_item.get_next()

    return ll


def gather_data_after(ll, start, n):
    out = []
    item = ll[start].get_next()
    for _ in range(n):
        out.append(item.get_data())
        item = item.get_next()

    return out


if __name__ == "__main__":
    input = list(map(int, list(sys.argv[1])))

    if sys.argv[2] == "1":
        ints_after_1 = gather_data_after(play(input, 100, 3), 1, len(input) - 1)
        out = "".join(map(str, ints_after_1))
    elif sys.argv[2] == "2":
        input += list(range(len(input) + 1, 1000001))
        ints_after_1 = gather_data_after(play(input, 10000000, 3), 1, 2)
        out = ints_after_1[0] * ints_after_1[1]

    print(out)
