import rpi.GPIO as GPIO
import time
import curses

__sleep_time = 15


if __name__ == '__main__':
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    stdscr.keypad(1)

    try:
        GPIO.setmode(GPIO.BOARD)

        stdscr.nodelay(1)
        while True:
            c = stdscr.getch()
            if c == 27:
                break
            elif c == curses.KEY_LEFT:
                curses.flushinp()
                print(c)
            elif c == curses.KEY_RIGHT:
                curses.flushinp()
                print(c)
            elif not c == -1:
                curses.flushinp()
                print(c)
            else:
                curses.flushinp()
            time.sleep(0.1)

            GPIO.output(7, True)
            time.sleep(__sleep_time/10000)
            GPIO.output(7, False)
            time.sleep((20 - __sleep_time)/10000)
            time.sleep((200)/10000)
    except

    except:
        pass
    finally:
        GPIO.cleanup()
        curses.nocbreak()
        stdscr.keypad(0)
        curses.echo()
        curses.endwin()
