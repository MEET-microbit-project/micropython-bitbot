#ifndef BITBOT_MOTOR
#define BITBOT_MOTOR


#include "MicroBitPin.h"

extern "C" {
  #include "microbitpin.h"
}

#define FORWARDS 0
#define BACKWARDS 1

// for some reason PIN_CAPABILITY_DIGITAL_IN etc. are not included properly
const PinCapability pc_digital_in = (PinCapability) 0x01;
const PinCapability pc_digital_out = (PinCapability)MODE_WRITE_DIGITAL; //(PinCapability) 0x02;
const PinCapability pc_analog_in = (PinCapability) 0x04;
const PinCapability pc_analog_out = (PinCapability) MODE_WRITE_ANALOG; //(PinCapability) 0x08;

/**
  @brief Get the event-handling id given a pin name
  @param[in] pin name (e.g. MICROBIT_PIN_P6)
  @return pin IO ID (e.g. MICROBIT_ID_IO_P6)
  @rtval 0 Pin was not found (no events)
*/
inline int getIoId(PinName pin)
{
  return pin==MICROBIT_PIN_P0 ? MICROBIT_ID_IO_P0:
         pin==MICROBIT_PIN_P1 ? MICROBIT_ID_IO_P1:
         pin==MICROBIT_PIN_P2 ? MICROBIT_ID_IO_P2:
         pin==MICROBIT_PIN_P3 ? MICROBIT_ID_IO_P3:
         pin==MICROBIT_PIN_P4 ? MICROBIT_ID_IO_P4:
         pin==MICROBIT_PIN_P5 ? MICROBIT_ID_IO_P5:
         pin==MICROBIT_PIN_P6 ? MICROBIT_ID_IO_P6:
         pin==MICROBIT_PIN_P7 ? MICROBIT_ID_IO_P7:
         pin==MICROBIT_PIN_P8 ? MICROBIT_ID_IO_P8:
         pin==MICROBIT_PIN_P9 ? MICROBIT_ID_IO_P9:
         pin==MICROBIT_PIN_P10 ? MICROBIT_ID_IO_P10:
         pin==MICROBIT_PIN_P11 ? MICROBIT_ID_IO_P11:
         pin==MICROBIT_PIN_P12 ? MICROBIT_ID_IO_P12:
         pin==MICROBIT_PIN_P13 ? MICROBIT_ID_IO_P13:
         pin==MICROBIT_PIN_P14 ? MICROBIT_ID_IO_P14:
         pin==MICROBIT_PIN_P15 ? MICROBIT_ID_IO_P15:
         pin==MICROBIT_PIN_P16 ? MICROBIT_ID_IO_P16:
         pin==MICROBIT_PIN_P19 ? MICROBIT_ID_IO_P19:
         pin==MICROBIT_PIN_P20 ? MICROBIT_ID_IO_P20: 0;
}

/**
  Class for controlling the motor(s) (i.e. the movement) of the bitbot
*/
class Motor {
private:
  MicroBitPin pin_left_dir, pin_left_speed, pin_right_dir, pin_right_speed;
  struct {
    int dir_left, dir_right;
    int speed_left, speed_right;
  } state;
public:
  /**
    @brief Initialize Motor with the control pins
    @param[in] name of the pin controlling the direction
        of the left motor (e.g. MICROBIT_PIN_P9)
  	@param[in] name of the pin controlling the speed of the left motor
    @param[in] name of the pin controlling the direction of the right motor
    @param[in] name of the pin controlling the speed of the right motor
  */
  Motor(PinName left_dir = MICROBIT_PIN_P8, PinName left_speed = MICROBIT_PIN_P0,
        PinName right_dir = MICROBIT_PIN_P12, PinName right_speed = MICROBIT_PIN_P1);

  /**
    @brief Set direction and speed of the left motor
    @param[in] direction
    @param[in] speed (0-MICROBIT_PIN_MAX_OUTPUT (1023)) of the moter,
        higher values mean faster
  */
  void setLeft(int dir, int speed);

  /**
  @brief Set direction and speed of the right motor
  @param[in] direction
  @param[in] speed (0-MICROBIT_PIN_MAX_OUTPUT (1023)) of the moter,
  higher values mean faster
  */
  void setRight(int dir, int speed);

  /**
    @brief Set direction and speed of the left motor
    @param[in] direction
    @param[in] speed of the moter in percent (0.0-100.0)
  */
  void setLeftPercent(int dir, float speed);

  /**
    @brief Set speed of left motor, infer direction from sign
    @param[in] speed (-100.0 - 100.0)
  */
  void setLeftPercent(float speed);

  /**
    @brief Set direction and speed of the right motor
    @param[in] direction
    @param[in] speed of the moter in percent (0.0-100.0)
  */
  void setRightPercent(int dir, float speed);

  /**
    @brief Set speed of right motor, infer direction from sign
    @param[in] speed (-100.0 - 100.0)
  */
  void setRightPercent(float speed);


  /**
    @brief Stop moving
  */
  void stop();

  /**
    @brief Set direction, and speed for each side
    @param[in] direction
    @param[in] speed of left wheel; range [0,1023]
    @param[in] speed of right wheel; range [0,1023]
  */
  void setSpeed(int dir, int speed_left, int speed_right);

  /**
    @brief Set speed for each side and infer direction from sign
    @param[in] speed of left wheel; range [-1023, 1023]
    @param[in] speed of right wheel; range [-1023, 1023]
  */
  void setSpeed(int speed_left, int speed_right);

  /**
    @brief Move straight at a given velocity
    @param[in] speed where negative implies backwards; range [-1023, 1023]
  */
  void setSpeed(int speed);

  /**
    @brief Set direction, and speed for each side in percent
    @param[in] direction
    @param[in] speed of left wheel; range [0.0,100.0]
    @param[in] speed of right wheel; range [0.0,100.0]
  */
  void setSpeedPercent(int dir, float speed_left, float speed_right);

  /**
    @brief Set speed for each side in percent and infer direction from sign
    @param[in] speed of left wheel in percent; range [-100.0, 100.0]
    @param[in] speed of right wheel in percent; range [-100.0, 100.0]
  */
  void setSpeedPercent(float speed_left, float speed_right);

  /**
    @brief Move straight at a given velocity
    @param[in] speed where negative implies backwards; range [-1023, 1023]
  */
  void setSpeedPercent(float speed);

  /**
    @return current speed of left wheel in range [0, 1023]
  */
  int getSpeedLeft();

  /**
    @return current speed of left wheel in percent
  */
  float getSpeedLeftPercent();

  /**
    @return current direction of left wheel
  */
  int getDirectionLeft();

  /**
    @return current speed of right wheel in range [0, 1023]
  */
  int getSpeedRight();

  /**
    @return current speed of right wheel in percent
  */
  float getSpeedRightPercent();

  /**
    @return current direction of right wheel
  */
  int getDirectionRight();
};

#endif /* end of include guard: BITBOT_MOTOR */
