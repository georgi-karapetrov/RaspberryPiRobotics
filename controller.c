#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

#include "conio.h"

#define MOTOR_A_FORWARD 10
#define MOTOR_A_BACKWARD 9

#define MOTOR_B_FORWARD 8
#define MOTOR_B_BACKWARD 7


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
    printf("stopMotors()\n");
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
    printf("moveForward()\n");
    motorAForward();
    motorBForward();
}

void moveBackward()
{
    printf("moveBackward()\n");
    motorABackward();
    motorBBackward();
}

void turnLeft()
{
    printf("turnLeft()\n");
//    motorBStop();
    motorAForward();
    motorBBackward();
}

void turnRight()
{
    printf("turnRight()\n");
//    motorAStop();
    motorBForward();
    motorABackward();
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
    printf("Setting up pins...\n");
    wiringPiSetupGpio();
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    printf("Done.\n");
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
