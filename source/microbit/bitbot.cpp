#include "microbitobj.h"

// set pins
#define PIN_BUZZER            ((microbit_pin_obj_t*)&microbit_p14_obj)
#define PIN_LIGHT             ((microbit_pin_obj_t*)&microbit_p2_obj)
#define PIN_LIGHT_CONTROL     ((microbit_pin_obj_t*)&microbit_p16_obj)
#define PIN_MOTOR_LEFT_DIR    ((microbit_pin_obj_t*)&microbit_p8_obj)
#define PIN_MOTOR_RIGHT_DIR   ((microbit_pin_obj_t*)&microbit_p12_obj)
#define PIN_MOTOR_LEFT_SPEED  ((microbit_pin_obj_t*)&microbit_p0_obj)
#define PIN_MOTOR_RIGHT_SPEED ((microbit_pin_obj_t*)&microbit_p1_obj)

extern "C" {

#include "py/runtime.h"
#include "py/nlr.h" // raise exceptions
#include "py/mphal.h"
#include "microbit/modmicrobit.h"
#include "microbit/microbitpin.h"

extern const mp_obj_type_t bitbot_type;


// python - C - interface
// STATIC mp_obj_t bitbot_buzz(mp_obj_t ms_in) {
//   for (int i = 1; i != -1; --i) {
//     microbit_pin_write_digital(PIN_BUZZER, mp_obj_new_int(i));
//     if (i != -1) microbit_sleep(ms_in);
//   }
//   return mp_const_none;
// }
// MP_DEFINE_CONST_FUN_OBJ_1(bitbot_buzz_obj, bitbot_buzz);

mp_obj_t bitbot_light[2];
STATIC mp_obj_t bitbot_get_brightness()
{
  for (int i = 0; i < 2; ++i) {
    microbit_pin_write_digital(PIN_LIGHT_CONTROL, mp_obj_new_int(i));
    bitbot_light[i] = mp_obj_new_float(mp_obj_get_float(microbit_pin_read_analog(PIN_LIGHT)) / 10.23);
  }
  return mp_obj_new_tuple(2, bitbot_light);
}
MP_DEFINE_CONST_FUN_OBJ_0(bitbot_get_brightness_obj, bitbot_get_brightness);

STATIC mp_obj_t bitbot_set_speed(mp_obj_t left, mp_obj_t right)
{
  float speed;
  speed = mp_obj_get_float(left);
  microbit_pin_write_digital(PIN_MOTOR_LEFT_DIR, mp_obj_new_int(speed<0?1:0));
  microbit_pin_write_analog(PIN_MOTOR_LEFT_SPEED, mp_obj_new_int((int)(10.23 * (speed + ((speed<0)?100:0)))));
  microbit_pin_write_digital(PIN_MOTOR_RIGHT_DIR, mp_obj_new_int(speed<0?1:0));
  speed = mp_obj_get_float(right);
  microbit_pin_write_analog(PIN_MOTOR_RIGHT_SPEED, mp_obj_new_int((int)(10.23 * (speed + ((speed<0)?100:0)))));
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(bitbot_set_speed_obj, bitbot_set_speed);


STATIC const mp_map_elem_t bitbot_module_globals_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_bitbot) },
  // { MP_OBJ_NEW_QSTR(MP_QSTR_buzz), (mp_obj_t)&bitbot_buzz_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_brightness), (mp_obj_t)&bitbot_get_brightness_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_set_speed), (mp_obj_t)&bitbot_set_speed_obj },
};

STATIC MP_DEFINE_CONST_DICT(bitbot_module_globals, bitbot_module_globals_table);

const mp_obj_module_t bitbot_module = {
    .base = { &mp_type_module },
    .name = MP_QSTR_bitbot,
    .globals = (mp_obj_dict_t*)&bitbot_module_globals,
};

}
