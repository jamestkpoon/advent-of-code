import sys
import numpy as np
from d3 import to_binary_map

UP = 0
RIGHT = 1
DOWN = 2
LEFT = 3
SIDES = [UP, DOWN, LEFT, RIGHT]


def get_opposite_direction(side):
    return (side + 2) % 4


class Tile:
    def __init__(self, data):
        self._data = data

    def flipud(self):
        self._data = np.flipud(self._data)

    def fliplr(self):
        self._data = np.fliplr(self._data)

    def rot90(self):
        self._data = np.rot90(self._data)

    def get_edge(self, side):
        if side is UP:
            return self._data[0, :]
        elif side is DOWN:
            return self._data[-1, :]
        elif side is LEFT:
            return self._data[:, 0]
        elif side is RIGHT:
            return self._data[:, -1]

    def get_symmetrical_edge_hash(self, side):
        side = self.get_edge(side)
        hash_fwd = hash(str(side))
        hash_rev = hash(str(side[::-1]))

        return hash(str(sorted([hash_fwd, hash_rev])))

    def get_transformations(self):
        for i in range(4):
            yield self._data
            self.fliplr()
            yield self._data
            self.flipud()
            yield self._data
            self.fliplr()
            yield self._data

            self.rot90()

    def try_transform_to_match(self, side, edge_to_match):
        for _ in self.get_transformations():
            if np.array_equal(self.get_edge(side), edge_to_match):
                return True

        return False


def parse_input(strs):
    out = {}
    line_index = 0
    while line_index < len(strs):
        key = int(strs[line_index][5:-1])
        line_index += 1

        i = line_index
        while line_index < len(strs) and len(strs[line_index]) > 0:
            line_index += 1

        out[key] = Tile(to_binary_map(strs[i:line_index]))
        line_index += 1

    return out


def count_edge_instances(tiles):
    out = {}
    for tile in tiles.values():
        for side in SIDES:
            hash = tile.get_symmetrical_edge_hash(side)
            if hash not in out.keys():
                out[hash] = 1
            else:
                out[hash] += 1

    return out


def find_corners(tiles, edge_instance_counts):
    corner_tile_keys = []
    for tile_id, tile in tiles.items():
        num_unique_sides = sum(
            [
                edge_instance_counts[tile.get_symmetrical_edge_hash(side)] == 1
                for side in SIDES
            ]
        )
        if num_unique_sides == 2:
            corner_tile_keys.append(tile_id)

    return corner_tile_keys


def grow(tiles, start_idx, direction, candidates, out_size_limit):
    out = [start_idx]
    opposite_direction = get_opposite_direction(direction)

    while len(candidates) > 0 and len(out) < out_size_limit:
        matched = False
        for tile_id in candidates:
            edge_to_match = tiles[out[-1]].get_edge(direction)
            if tiles[tile_id].try_transform_to_match(
                opposite_direction, edge_to_match
            ):
                out.append(tile_id)
                candidates.remove(tile_id)
                matched = True
                break
        if not matched:
            break

    return out


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    tiles = parse_input(data)
    edge_instance_counts = count_edge_instances(tiles)
    corner_tile_keys = find_corners(tiles, edge_instance_counts)

    if sys.argv[2] == "1":
        out = 1
        for n in corner_tile_keys:
            out *= n
    elif sys.argv[2] == "2":
        tl_id = corner_tile_keys.pop()
        if edge_instance_counts[tiles[tl_id].get_symmetrical_edge_hash(UP)] > 1:
            tiles[tl_id].flipud()
        if edge_instance_counts[tiles[tl_id].get_symmetrical_edge_hash(LEFT)] > 1:
            tiles[tl_id].fliplr()
        ids = list(tiles.keys())
        ids.remove(tl_id)

        tile_idx_map_size = int(np.sqrt(len(tiles)))
        tile_idx_map = np.empty([tile_idx_map_size, tile_idx_map_size], dtype=np.int)
        tile_idx_map[:, 0] = grow(tiles, tl_id, DOWN, ids, tile_idx_map_size)
        for r in range(tile_idx_map_size):
            tile_idx_map[r, :] = grow(
                tiles, tile_idx_map[r, 0], RIGHT, ids, tile_idx_map_size
            )

        borderless_tile_shape = np.asarray(tiles[tl_id]._data.shape) - 2
        img_shape = tile_idx_map_size * borderless_tile_shape
        img = np.empty(tuple(img_shape), dtype=np.bool)
        for c in range(tile_idx_map_size):
            c_si = c * borderless_tile_shape[0]
            c_ei = c_si + borderless_tile_shape[0]
            for r in range(tile_idx_map_size):
                r_si = r * borderless_tile_shape[1]
                r_ei = r_si + borderless_tile_shape[1]
                img[c_si:c_ei, r_si:r_ei] = tiles[tile_idx_map[c, r]]._data[
                    1:-1, 1:-1
                ]

        with open(sys.argv[3], "r") as f:
            target_pattern = to_binary_map(f.read().splitlines())
            target_trues = np.where(target_pattern.ravel())[0]

        out = None
        for img in Tile(img).get_transformations():
            num_target_pattern_instances = 0
            for c in range(img.shape[0] - target_pattern.shape[0]):
                for r in range(img.shape[1] - target_pattern.shape[1]):
                    roi_ravel = img[
                        c : c + target_pattern.shape[0],
                        r : r + target_pattern.shape[1],
                    ].ravel()
                    if np.all([roi_ravel[i] for i in target_trues]):
                        num_target_pattern_instances += 1

            if num_target_pattern_instances != 0:
                out = np.sum(img) - (
                    num_target_pattern_instances * len(target_trues)
                )
                break

    print(out)
