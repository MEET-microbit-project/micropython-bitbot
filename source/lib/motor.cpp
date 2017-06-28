#include <algorithm> // min, max
#include "lib/motor.h"

Motor::Motor(PinName left_dir, PinName left_speed,
             PinName right_dir, PinName right_speed)
  : pin_left_dir(getIoId(left_dir), left_dir, pc_digital_out),
    pin_left_speed(getIoId(left_speed), left_speed, pc_analog_out),
    pin_right_dir(getIoId(right_dir), right_dir, pc_digital_out),
    pin_right_speed(getIoId(right_speed), right_speed, pc_analog_out)
{ }

inline void setMotorPins(MicroBitPin& pin_dir, int dir,
                         MicroBitPin& pin_speed, int speed)
{
  // limit speed to minimal/maximal values
  speed = std::max(0, std::min(MICROBIT_PIN_MAX_OUTPUT, speed));
  // calculate actual analog value, considering the direction
  speed = (dir == FORWARDS) ? speed : MICROBIT_PIN_MAX_OUTPUT - speed;
  // write direction & speed
  pin_dir.setDigitalValue(dir);
  pin_speed.setAnalogValue(speed);
}

void Motor::setLeft(int dir, int speed)
{
  setMotorPins(pin_left_dir, dir, pin_left_speed, speed);
  state.dir_left = dir;
  state.speed_left = speed;
}

void Motor::setRight(int dir, int speed)
{
  setMotorPins(pin_right_dir, dir, pin_right_speed, speed);
  state.dir_right = dir;
  state.speed_right = speed;
}

void Motor::setLeftPercent(int dir, float speed)
{
  setLeft(dir, speed / 100 * MICROBIT_PIN_MAX_OUTPUT);
}

void Motor::setLeftPercent(float speed)
{
  if (speed < 0) {
    setLeft(1, -speed / 100 * MICROBIT_PIN_MAX_OUTPUT);
  } else {
    setLeft(0, speed / 100 * MICROBIT_PIN_MAX_OUTPUT);
  }
}

void Motor::setRightPercent(int dir, float speed)
{
  setRight(dir, speed / 100 * MICROBIT_PIN_MAX_OUTPUT);
}

void Motor::setRightPercent(float speed)
{
  if (speed < 0) {
    setRight(1, -speed / 100 * MICROBIT_PIN_MAX_OUTPUT);
  } else {
    setRight(0, speed / 100 * MICROBIT_PIN_MAX_OUTPUT);
  }
}

void Motor::stop()
{
  setLeft(FORWARDS, 0);
  setRight(FORWARDS, 0);
}

void Motor::setSpeed(int dir, int speed_left, int speed_right)
{
  setLeft(dir, speed_left);
  setRight(dir, speed_right);
}

void Motor::setSpeed(int speed_left, int speed_right)
{
  if (speed_left >= 0) {
    setLeft(FORWARDS, speed_left);
  } else {
    setLeft(BACKWARDS, -speed_left);
  }

  if (speed_right >= 0) {
    setRight(FORWARDS, speed_right);
  } else {
    setRight(BACKWARDS, -speed_right);
  }
}

void Motor::setSpeed(int speed)
{
  int dir = FORWARDS;
  if (speed < 0) {
    dir = BACKWARDS;
    speed = -speed;
  }
  setLeft(dir, speed);
  setRight(dir, speed);
}

void Motor::setSpeedPercent(int dir, float speed_left, float speed_right)
{
  setLeftPercent(dir, speed_left);
  setRightPercent(dir, speed_right);
}

void Motor::setSpeedPercent(float speed_left, float speed_right)
{
  if (speed_left >= 0) {
    setLeftPercent(FORWARDS, speed_left);
  } else {
    setLeftPercent(BACKWARDS, -speed_left);
  }

  if (speed_right >= 0) {
    setRightPercent(FORWARDS, speed_right);
  } else {
    setRightPercent(BACKWARDS, -speed_right);
  }
}

void Motor::setSpeedPercent(float speed)
{
  int dir = FORWARDS;
  if (speed < 0) {
    dir = BACKWARDS;
    speed = -speed;
  }
  setLeftPercent(dir, speed);
  setRightPercent(dir, speed);
}

int Motor::getSpeedLeft()
{
  return state.speed_left;
}

float Motor::getSpeedLeftPercent()
{
  return state.speed_left / 100.0;
}

int Motor::getDirectionLeft()
{
  return state.dir_left;
}

int Motor::getSpeedRight()
{
  return state.speed_right;
}

float Motor::getSpeedRightPercent()
{
  return state.speed_right / 100.0;
}

int Motor::getDirectionRight()
{
  return state.dir_right;
}
