import unittest
import turret
import servo
import laser
import camera

class IsOddTests(unittest.TestCase):
    def test_camera(self):
        camera.visible_target()

    def test_servo(self):
        serv = servo.servo()
        angle = serv.get_angle()
        serv.turn_right()
        self.assertTrue(serv.get_angle() > angle)
        
    def test_laser(self):
        laser.turn_on()
        self.assertTrue(laser.get_powerstate())
        laser.turn_off()
        self.assertFalse(laser.get_powerstate())
    
    def test_fire_laser(self):
        turret.fire_laser()
		
    def test_automatic_firing(self):
        turret.start(50)
		
def main():
    unittest.main()

if __name__ == '__main__':
    main()
