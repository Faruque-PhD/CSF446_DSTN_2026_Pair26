import json
import unittest

# Import the Disk class from both implementations
from disk import Disk

scores = {"scores": {}}


class TestDiskClass(unittest.TestCase):

    def setUp(self):
        # Fixed parameters
        self.addr = "-1"
        self.lateAddr = "-1"
        self.lateAddrDesc = "0,-1,0"
        self.addrDesc = "5,-1,0"
        self.graphics = False
        self.compute = True
        self.skew = 0
        self.window = -1
        self.policy = "CLOOK"
        self.seekSpeed = 1
        self.rotateSpeed = 1
        self.numTracks = 3
        self.armTrack = 0
        self.initialDir = 1
        self.zoning = "30,30,30"
        self.rValue = 0

    def get_fail_message(self, expected, actual):
        return "Expected: " + str(expected) + "\nActual: " + str(actual)

    def update_scores(self, test_case, score, expr1, expr2):
        if expr1 == expr2:
            scores["scores"][test_case] = score
        else:
            scores["scores"][test_case] = 0

    def test_case_1(self):
        # Test Case 1
        self.addr = "3,34,18,17,1,30,19"
        answer = [
            (3, 0, 255, 30, 285),
            (1, 0, 270, 30, 300),
            (18, 40, 80, 30, 150),
            (17, 0, 300, 30, 330),
            (19, 0, 30, 30, 60),
            (34, 40, 20, 30, 90),
            (30, 0, 210, 30, 240),
        ]
        # python disk.py -c -a 3,34,18,17,1,30,19 -p CLOOK

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()

        fail_message = self.get_fail_message(answer, disk.getBlockStats())

        self.update_scores("test_case_1", 10, disk.getBlockStats(), answer)

        self.assertEqual(
            disk.getBlockStats(), answer, "Test Case 1 failed\n" + fail_message
        )

    def test_case_2(self):
        # Test Case 2
        self.addr = "30,18,13,17,1,16"
        self.window = 3
        answer = [
            (18, 40, 305, 30, 375),
            (13, 0, 180, 30, 210),
            (30, 40, 80, 30, 150),
            (1, 80, 100, 30, 210),
            (17, 40, 50, 30, 120),
            (16, 0, 300, 30, 330),
        ]
        # python disk.py -c -a 30,18,13,17,1,16 -w 3 -p CLOOK

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()
        fail_message = self.get_fail_message(answer, disk.getBlockStats())

        self.update_scores("test_case_2", 10, disk.getBlockStats(), answer)
        self.assertEqual(
            disk.getBlockStats(), answer, "Test Case 2 failed\n" + fail_message
        )

    def test_case_3(self):
        # Test Case 3
        self.addr = "50,40,1,10,23,36,28,13,8,4"
        self.armTrack = 2
        self.numTracks = 5
        self.initialDir = 0
        self.seekSpeed = 0.5
        self.rotateSpeed = 4
        self.zoning = "30" + ",30" * (self.numTracks - 1)
        answer = [
            (28, 0, 71, 7, 78),
            (23, 80, 55, 8, 143),
            (13, 0, 7, 8, 15),
            (1, 80, 2, 8, 90),
            (10, 0, 60, 7, 67),
            (8, 0, 68, 7, 75),
            (4, 0, 53, 7, 60),
            (50, 320, 18, 7, 345),
            (40, 80, 18, 7, 105),
            (36, 0, 53, 7, 60),
        ]
        # python disk.py -c -a 50,40,1,10,23,36,28,13,8,4 -t 2 -n 5 -i 0 -p CLOOK -S 0.5 -R 4

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()
        fail_message = self.get_fail_message(answer, disk.getBlockStats())

        self.update_scores("test_case_3", 10, disk.getBlockStats(), answer)

        self.assertEqual(
            disk.getBlockStats(), answer, "Test Case 3 failed\n" + fail_message
        )

    def test_case_4(self):
        # Test Case 4
        self.addr = "16,40,37,9,38,28,1,41,11,13,18,25"
        self.window = 5
        self.numTracks = 6
        self.armTrack = 5
        self.zoning = "30,30,60,60,90,90"
        # python disk.py -c -t 5 -n 6 -p CLOOK -z 30,30,60,60,90,90 -a 16,40,37,9,38,28,1,41,11,13,18,25 -w 5

        answer = [
            (40, 0, 135, 90, 225),
            (9, 200, 10, 30, 240),
            (16, 40, 140, 30, 210),
            (37, 120, 150, 90, 360),
            (38, 0, 0, 90, 90),
            (41, 40, 140, 90, 270),
            (1, 200, 40, 30, 270),
            (11, 0, 270, 30, 300),
            (13, 40, 350, 30, 420),
            (28, 40, 125, 60, 225),
            (25, 0, 120, 60, 180),
            (18, 40, 35, 30, 105),
        ]

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()
        fail_message = self.get_fail_message(answer, disk.getBlockStats())

        self.update_scores("test_case_4", 10, disk.getBlockStats(), answer)

        self.assertEqual(
            disk.getBlockStats(), answer, "Test Case 4 failed\n" + fail_message
        )


    def test_case_5(self):
        # Test Case 5
        self.addr = "1,90,25,71,61,48,9,36,57,17,70,60,33,90,84,30,109,117,97,56,37,87,107,21,56"
        self.window = 6
        self.numTracks = 10
        self.armTrack = 5
        self.zoning = "30" + ",30" * (self.numTracks - 1)
        # python disk.py -c -t 5 -n 10 -p CLOOK -a 1,90,25,71,61,48,9,36,57,17,70,60,33,90,84,30,109,117,97,56,37,87,107,21,56 -w 6

        answer = [
            (71, 0, 135, 30, 165),
            (61, 0, 30, 30, 60),
            (90, 80, 40, 30, 150),
            (1, 280, 260, 30, 570),
            (25, 80, 250, 30, 360),
            (48, 80, 220, 30, 330),
            (57, 0, 240, 30, 270),
            (70, 40, 320, 30, 390),
            (60, 0, 30, 30, 60),
            (9, 200, 40, 30, 270),
            (17, 40, 170, 30, 240),
            (36, 80, 100, 30, 210),
            (90, 160, 350, 30, 540),
            (84, 0, 150, 30, 180),
            (109, 80, 280, 30, 390),
            (117, 0, 210, 30, 240),
            (33, 280, 50, 30, 360),
            (30, 0, 240, 30, 270),
            (37, 40, 140, 30, 210),
            (56, 40, 140, 30, 210),
            (87, 120, 60, 30, 210),
            (97, 40, 230, 30, 300),
            (107, 0, 270, 30, 300),
            (21, 280, 350, 30, 660),
            (56, 120, 180, 30, 330),
        ]

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()
        fail_message = self.get_fail_message(answer, disk.getBlockStats())
        self.update_scores("test_case_5", 20, disk.getBlockStats(), answer)
        self.assertEqual(
            disk.getBlockStats(), answer, "Test Case 5 failed\n" + fail_message
        )

    def test_case_6(self):
        # Test Case 6
        self.addr = (
            "56,78,79,17,1,44,32,97,82,72,66,79,17,52,19,108,7,98,8,82,40,48,101,2,7"
        )
        self.window = 7
        self.numTracks = 10
        self.armTrack = 9
        self.zoning = "30" + ",30" * (self.numTracks - 1)
        # python disk.py -c -t 9 -n 10 -p CLOOK -a 56,78,79,17,1,44,32,97,82,72,66,79,17,52,19,108,7,98,8,82,40,48,101,2,7 -w 7

        answer = [
            (1, 360, 195, 30, 585),
            (17, 40, 50, 30, 120),
            (32, 40, 20, 30, 90),
            (44, 40, 290, 30, 360),
            (56, 40, 290, 30, 360),
            (78, 80, 190, 30, 300),
            (79, 0, 0, 30, 30),
            (82, 0, 60, 30, 90),
            (72, 0, 30, 30, 60),
            (79, 0, 180, 30, 210),
            (97, 80, 70, 30, 180),
            (17, 280, 170, 30, 480),
            (52, 120, 180, 30, 330),
            (66, 40, 350, 30, 420),
            (82, 40, 50, 30, 120),
            (98, 80, 10, 30, 120),
            (108, 40, 230, 30, 300),
            (7, 360, 180, 30, 570),
            (8, 0, 0, 30, 30),
            (19, 40, 260, 30, 330),
            (40, 80, 160, 30, 270),
            (48, 40, 170, 30, 240),
            (101, 160, 320, 30, 510),
            (2, 320, 280, 30, 630),
            (7, 0, 120, 30, 150),
        ]

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()
        fail_message = self.get_fail_message(answer, disk.getBlockStats())

        self.update_scores("test_case_6", 30, disk.getBlockStats(), answer)
        self.assertEqual(
            disk.getBlockStats(), answer, "Test Case 6 failed\n" + fail_message
        )

    # V(R) test cases - NON-EVALUATIVE (self-study only)
    # These test cases are for self-study purposes and do not contribute to the grade
    def test_vr_case_1(self):
        # V(R) Test Case 1 - Non-evaluative
        self.policy = "VR"
        self.addr = "5,8,17,57,52,3"
        self.numTracks = 5
        self.armTrack = 0
        self.window = 3
        self.rValue = 0.3
        self.zoning = "30" + ",30" * (self.numTracks - 1)
        # python disk.py -c -a 5,8,17,57,52,3 -w 3 -n 5 -p VR -r 0.3
        answer = [
            (5, 0, 255, 30, 285),
            (8, 0, 0, 30, 30),
            (17, 0, 0, 30, 30),
            (3, 40, 270, 30, 340),
            (57, 160, 0, 30, 190),
            (52, 0, 150, 30, 180),
        ]

        disk = Disk(
            self.addr,
            self.addrDesc,
            self.lateAddr,
            self.lateAddrDesc,
            self.policy,
            self.seekSpeed,
            self.rotateSpeed,
            self.skew,
            self.window,
            self.compute,
            self.graphics,
            self.zoning,
            self.armTrack,
            self.numTracks,
            self.initialDir,
            self.rValue,
        )

        disk.Go()
        fail_message = self.get_fail_message(answer, disk.getBlockStats())

        # Score is 0 for non-evaluative test cases
        self.update_scores("test_vr_case_1", 0, disk.getBlockStats(), answer)
        self.assertEqual(
            disk.getBlockStats(), answer, "V(R) Test Case 1 failed\n" + fail_message
        )

# If this script is executed directly, run the tests
if __name__ == "__main__":
    unittest.main(exit=False)
    # iterate from 1 to 6 (CLOOK test cases) and if test_case_i is not in scores, add it with 0
    for i in range(1, 7):
        if "test_case_" + str(i) not in scores["scores"]:
            scores["scores"]["test_case_" + str(i)] = 0
    # V(R) test cases are non-evaluative (already set to 0)

    print(json.dumps({"_presentation": "semantic"}, separators=(',', ': ')))
    print(json.dumps(scores, separators=(',', ': ')))
