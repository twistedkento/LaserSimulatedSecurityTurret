#ifndef __COMMAND_H__
#define __COMMAND_H__

class Command {
    private:
        char data[1];
    public:
        Command();
        Command(char);
        enum class CommandState {
            SERVO_OFF = 0,
            SERVO_INCREASE = 1,
            SERVO_DECREASE = 2,
            SERVO_RESET = 3,
            LASER_OFF = 0,
            LASER_ON = 1
        };
        void setServoX(CommandState);
        void setServoY(CommandState);
        void setLaser(CommandState);
        const char* getValue(void);
        int getValueSize(void);
        void reset(void);
};

#endif

