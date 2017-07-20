#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

#include "conio.h"
#define forever while(1)

#define MOTOR_A_FORWARD 10
#define MOTOR_A_BACKWARD 9

#define MOTOR_B_FORWARD 8
#define MOTOR_B_BACKWARD 7

// Keys mappings
#define MOVE_FORWARD_KEY 'w'
#define MOVE_BACKWARD_KEY 's'
#define TURN_LEFT_KEY 'a'
#define TURN_RIGHT_KEY 'd'
#define STOP_MOTORS_KEY 'h'
#define ESC_KEY 27

// PWM
#define FREQUENCY 20
#define DUTY_CYCLE_MOTOR_A 30
#define DUTY_CYCLE_MOTOR_B 30
#define STOP 0
#define PWM_RANGE 100

int speed = 30;

void setupPWM()
{
    softPwmCreate(MOTOR_A_FORWARD, STOP, PWM_RANGE);
    softPwmCreate(MOTOR_B_FORWARD, STOP, PWM_RANGE);
    softPwmCreate(MOTOR_A_BACKWARD, STOP, PWM_RANGE);
    softPwmCreate(MOTOR_B_BACKWARD, STOP, PWM_RANGE);
}

void motorAStop()
{
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_A_BACKWARD, LOW);
}

void motorBStop()
{
    digitalWrite(MOTOR_B_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, LOW);
}

void stopMotors()
{
    motorAStop();
    motorBStop();
}
void motorAForward()
{
    digitalWrite(MOTOR_A_FORWARD, HIGH);
    digitalWrite(MOTOR_A_BACKWARD, LOW);
}

void motorABackward()
{
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_A_BACKWARD, HIGH);
}

void motorBForward()
{
    digitalWrite(MOTOR_B_FORWARD, HIGH);
    digitalWrite(MOTOR_B_BACKWARD, LOW);
}

void motorBBackward()
{
    digitalWrite(MOTOR_B_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, HIGH);
}

void moveForward()
{
    motorAForward();
    motorBForward();
}

void moveBackward()
{
    motorABackward();
    motorBBackward();
}

void turnLeft()
{
    motorBStop();
    motorAForward();
}

void turnRight()
{
    motorAStop();
    motorBForward();
}

void pwmForward()
{
    softPwmWrite(MOTOR_A_FORWARD, speed);
    softPwmWrite(MOTOR_A_BACKWARD, STOP);
    softPwmWrite(MOTOR_B_FORWARD, speed);
    softPwmWrite(MOTOR_B_BACKWARD, STOP);
}

void pwmStopMotors()
{
    softPwmWrite(MOTOR_A_FORWARD, STOP);
    softPwmWrite(MOTOR_A_BACKWARD, STOP);
    softPwmWrite(MOTOR_B_FORWARD, STOP);
    softPwmWrite(MOTOR_B_BACKWARD, STOP);
}

void setupPins()
{
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
}

void alternatingForward(const int frequency, const int dutyCycle)
{
    int i;
    for (i = 0; i < frequency; ++i)
    {
        motorAForward();
        usleep(dutyCycle / 2);
        motorAStop();
        motorBForward();
        usleep(dutyCycle / 2);
        motorBStop();
    }
}

int main(const int argc, const char * argv[])
{
    //use Broadcom GPIO pin numbers
    wiringPiSetupGpio();

    setupPins();
//    setupPWM();
    int ch;
    while ((ch = getch()) != ESC_KEY)
    {
        if (ch == MOVE_FORWARD_KEY)
        {
            moveForward();
            sleep(1);
            stopMotors();
        }
        else if (ch == MOVE_BACKWARD_KEY)
        {
            moveBackward();
            sleep(1);
            stopMotors();
        }
        else if (ch == TURN_LEFT_KEY)
        {
            stopMotors();
            turnLeft();
            sleep(1);
            stopMotors();
        }
        else if (ch == TURN_RIGHT_KEY)
        {
            stopMotors();
            turnRight();
            sleep(1);
            stopMotors();
        }
        else if (ch == STOP_MOTORS_KEY)
        {
            stopMotors();
        }
        else if (ch == 'm')
        {
            printf("mmmm\n");
            digitalWrite(MOTOR_A_FORWARD, HIGH);
            digitalWrite(MOTOR_A_BACKWARD, HIGH);
        }
        else if (ch == 'n')
        {
            printf("nnnn\n");
            digitalWrite(MOTOR_A_FORWARD, LOW);
            digitalWrite(MOTOR_A_BACKWARD, LOW);
        }
        else if (ch == '>')
        {
            speed += 10;
        }
        else if (ch == '<')
        {
            speed -= 10;
        }
        else if (ch == 'i')
        {
            pwmForward();
        }
        else if (ch == 'o')
        {
            pwmStopMotors();
        }
        else if (ch == 't')
        {
            alternatingForward(2, 10000);
        }
        else if (ch == 'A')
        {
            turnLeft();
        }
        else if (ch == 'D')
        {
            turnRight();
        }
   }

    stopMotors();
    return 0;
}
