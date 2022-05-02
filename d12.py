import sys
import numpy as np


def parse_cmds(strs):
    out = []
    for s in strs:
        out.append([s[0], int(s[1:])])

    return out


class Ship:
    def __init__(self, initial_pose=[0, 0, -np.pi / 2], wp_pos=[1, -10]):
        self._pose = np.array(initial_pose)
        self._wp_pos = np.array(wp_pos)

    def get_manhattan_distance_from_origin(self):
        return np.sum(np.abs(self._pose[:2]))

    def execute_direct_commands(self, cmds):
        for cmd in cmds:
            if cmd[0] == "N":
                self._pose[0] += cmd[1]
            elif cmd[0] == "W":
                self._pose[1] += cmd[1]
            elif cmd[0] == "S":
                self._pose[0] -= cmd[1]
            elif cmd[0] == "E":
                self._pose[1] -= cmd[1]
            elif cmd[0] == "L":
                self._pose[2] += np.deg2rad(cmd[1])
            elif cmd[0] == "R":
                self._pose[2] -= np.deg2rad(cmd[1])
            elif cmd[0] == "F":
                self._pose[:2] += np.round(
                    cmd[1] * np.asarray([np.cos(self._pose[2]), np.sin(self._pose[2])])
                )
            else:
                print("Got unrecognized cmd " + str(cmd))

    def execute_waypoint_commands(self, cmds):
        for cmd in cmds:
            if cmd[0] == "N":
                self._wp_pos[0] += cmd[1]
            elif cmd[0] == "W":
                self._wp_pos[1] += cmd[1]
            elif cmd[0] == "S":
                self._wp_pos[0] -= cmd[1]
            elif cmd[0] == "E":
                self._wp_pos[1] -= cmd[1]
            elif cmd[0] == "L":
                self.rotate_waypoint(cmd[1])
            elif cmd[0] == "R":
                self.rotate_waypoint(-cmd[1])
            elif cmd[0] == "F":
                self._pose[:2] += cmd[1] * self._wp_pos
            else:
                print("Got unrecognized cmd " + str(cmd))

    def rotate_waypoint(self, deg):
        waypoint_distance = np.sqrt(np.sum(np.square(self._wp_pos)))
        waypoint_heading = np.arctan2(self._wp_pos[1], self._wp_pos[0]) + np.deg2rad(
            deg
        )
        self._wp_pos = np.round(
            waypoint_distance
            * np.asarray([np.cos(waypoint_heading), np.sin(waypoint_heading)])
        )


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    cmds = parse_cmds(data)

    ship = Ship()

    if sys.argv[2] == "1":
        ship.execute_direct_commands(cmds)
    elif sys.argv[2] == "2":
        ship.execute_waypoint_commands(cmds)

    out = ship.get_manhattan_distance_from_origin()

    print(out)
