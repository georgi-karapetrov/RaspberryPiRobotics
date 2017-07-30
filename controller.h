#ifndef CONTROLLER_H
#define CONTROLLER_H

void SetupPins();
void MoveForward();
void MoveBackward();
void TurnLeft();
void TurnRight();
void StopMotors();

float DistanceFromSensor();

#endif
