import unittest
import turret

class IsOddTests(unittest.TestCase):

    def testOne(self):
        angle = turret.getAngle()
        turret.turnRight()
        self.assertTrue(turret.getAngle() > angle)
def main():
    unittest.main()

if __name__ == '__main__':
    main()
