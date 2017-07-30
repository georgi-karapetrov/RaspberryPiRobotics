#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#include <sys/time.h>

// Motor pins
#define MOTOR_A_FORWARD 10
#define MOTOR_A_BACKWARD 9

#define MOTOR_B_FORWARD 8
#define MOTOR_B_BACKWARD 7

// Distance sensor pins
#define TRIGGER 17
#define ECHO 18

// Distance sensor constants
#define START_STOP_TIME_EPS 4000
#define SPEED_OF_SOUND 34326

// PWM
#define FREQUENCY 20
#define DUTY_CYCLE_MOTOR_A 30
#define DUTY_CYCLE_MOTOR_B 30
#define STOP 0
#define PWM_RANGE 100

int speed = 30;

void SetupPWM()
{
    softPwmCreate(MOTOR_A_FORWARD, STOP, PWM_RANGE);
    softPwmCreate(MOTOR_B_FORWARD, STOP, PWM_RANGE);
    softPwmCreate(MOTOR_A_BACKWARD, STOP, PWM_RANGE);
    softPwmCreate(MOTOR_B_BACKWARD, STOP, PWM_RANGE);
}

void MotorAStop()
{
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_A_BACKWARD, LOW);
}

void MotorBStop()
{
    digitalWrite(MOTOR_B_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, LOW);
}

void StopMotors()
{
    printf("StopMotors()\n");
    MotorAStop();
    MotorBStop();
}
void MotorAForward()
{
    digitalWrite(MOTOR_A_FORWARD, HIGH);
    digitalWrite(MOTOR_A_BACKWARD, LOW);
}

void MotorABackward()
{
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_A_BACKWARD, HIGH);
}

void MotorBForward()
{
    digitalWrite(MOTOR_B_FORWARD, HIGH);
    digitalWrite(MOTOR_B_BACKWARD, LOW);
}

void MotorBBackward()
{
    digitalWrite(MOTOR_B_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, HIGH);
}

void MoveForward()
{
    printf("MoveForward()\n");
    MotorAForward();
    MotorBForward();
}

void MoveBackward()
{
    printf("MoveBackward()\n");
    MotorABackward();
    MotorBBackward();
}

void TurnLeft()
{
    printf("TurnLeft()\n");
//    MotorBStop();
    MotorAForward();
    MotorBBackward();
}

void TurnRight()
{
    printf("TurnRight()\n");
//    MotorAStop();
    MotorBForward();
    MotorABackward();
}

void PWMForward()
{
    softPwmWrite(MOTOR_A_FORWARD, speed);
    softPwmWrite(MOTOR_A_BACKWARD, STOP);
    softPwmWrite(MOTOR_B_FORWARD, speed);
    softPwmWrite(MOTOR_B_BACKWARD, STOP);
}

void PWMStopMotors()
{
    softPwmWrite(MOTOR_A_FORWARD, STOP);
    softPwmWrite(MOTOR_A_BACKWARD, STOP);
    softPwmWrite(MOTOR_B_FORWARD, STOP);
    softPwmWrite(MOTOR_B_BACKWARD, STOP);
}

void SetupMotorPins()
{
    printf("Setting up motor pins...\n");
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    printf("Done.\n");
}

void SetupDistanceSensorPins()
{
    printf("Setting up distance sensor pins...\n");
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
    delay(30);
    printf("Done.\n");
}

void SetupPins()
{
    wiringPiSetupGpio();
    SetupMotorPins();
    SetupDistanceSensorPins();
}

void SendImpulse()
{
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIGGER, LOW);
}

int DistanceFromSensor()
{
    SendImpulse();
    
    long startTime = micros();
    while (digitalRead(ECHO) == LOW)
    {
        startTime = micros();
    }

    long stopTime = 0;
    while (digitalRead(ECHO) == HIGH)
    {
        stopTime = micros();
        printf("delta = %d\n", stopTime - startTime);
        if (stopTime - startTime >= START_STOP_TIME_EPS)
        {
            return -1;
        }
    }

    int distance = (stopTime - startTime) / 58;

    return distance;
}

void AlternatingForward(const int frequency, const int dutyCycle)
{
    int i;
    for (i = 0; i < frequency; ++i)
    {
        MotorAForward();
        usleep(dutyCycle / 2);
        MotorAStop();
        MotorBForward();
        usleep(dutyCycle / 2);
        MotorBStop();
    }
}
