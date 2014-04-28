from tkinter import *
import time
import math
master = Tk()

canvas_width = 800
canvas_height = 600
w = Canvas(master, 
           width=canvas_width,
           height=canvas_height)
w.pack()

coords = []
i = 0
x1 = 0
y1 = 0
x2 = 0
y2 = 0

def timer_function():
    test_function()
    master.after(20, timer_function)

def test_function():
    global coords, i
    i += 1
    x = int(100*math.sin(math.radians(i)*4 )) 
    y = int(100*math.cos(math.radians(i)*4 ))

    coords.append((x,y))

    w.delete('all')
    for a,b in zip([i for i in coords ],[i for i in coords[1:]]):
        w.create_line(a[0]*2+(canvas_width/2), a[1]*2+(canvas_height/2),
                b[0]*2+(canvas_width/2), b[1]*2+(canvas_height/2),
                fill="#476042", width=4)
    if len(coords) > 10:
        coords.pop(0)


master.after(200, timer_function )
mainloop()
