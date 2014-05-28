import time
from servo import ServoClass

servo = ServoClass(0)
servob = ServoClass(1)
for i in range(300):
    servo.go_circle((i+45)*6, 0)
    servob.go_circle((i+45)*6, 1)
    time.sleep(0.05)
