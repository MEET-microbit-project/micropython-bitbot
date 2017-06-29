#include "microbitobj.h"
#include "MicroBit.h"


// set pins
#define PIN_BUZZER            ((microbit_pin_obj_t*)&microbit_p14_obj)
#define PIN_LINE_LEFT         ((microbit_pin_obj_t*)&microbit_p11_obj)
#define PIN_LIGHT             ((microbit_pin_obj_t*)&microbit_p2_obj)
#define PIN_LIGHT_CONTROL     ((microbit_pin_obj_t*)&microbit_p16_obj)
#define PIN_LINE_RIGHT        ((microbit_pin_obj_t*)&microbit_p5_obj)
#define PIN_MOTOR_LEFT_DIR    ((microbit_pin_obj_t*)&microbit_p8_obj)
#define PIN_MOTOR_RIGHT_DIR   ((microbit_pin_obj_t*)&microbit_p12_obj)
#define PIN_MOTOR_LEFT_SPEED  ((microbit_pin_obj_t*)&microbit_p0_obj)
#define PIN_MOTOR_RIGHT_SPEED ((microbit_pin_obj_t*)&microbit_p1_obj)
#define NEOPIXEL_NUM_LEDS     12
#define NAME_PIN_NEOPIXEL     (microbit_p13_obj.name)

extern "C" {

#include "py/runtime.h"
#include "py/nlr.h" // raise exceptions
#include "py/mphal.h"
#include "lib/neopixel.h"
#include "lib/pwm.h"
#include "microbit/modmicrobit.h"
#include "microbit/microbitpin.h"
#include "microbit/modneopixel.h"

extern const mp_obj_type_t bitbot_type;

// for convenience
const mp_obj_t mp_const_int_zero = mp_obj_new_int(0);
const mp_obj_t mp_const_int_one = mp_obj_new_int(1);

// define a constant neopixel object
color_t *bitbot_neopixel_init() {
  nrf_gpio_cfg_output(NAME_PIN_NEOPIXEL);
  NRF_GPIO->OUTCLR = (1UL << NAME_PIN_NEOPIXEL);
  color_t * leds = (color_t*) malloc(sizeof(color_t) * NEOPIXEL_NUM_LEDS);
	for (int i = 0; i < NEOPIXEL_NUM_LEDS; i++)
	{
		leds[i].simple.g = 0;
		leds[i].simple.r = 0;
		leds[i].simple.b = 0;
	}
  return leds;
}

const neopixel_obj_t bitbot_neopixel_obj = {
  &neopixel_type,
  {
    NAME_PIN_NEOPIXEL,
    NEOPIXEL_NUM_LEDS,
    bitbot_neopixel_init()
  }
};

// motor controls
void inline setPercent(microbit_pin_obj_t *pin_dir, microbit_pin_obj_t *pin_speed, float speed)
{
  int speed_exact;
  if (speed < 0) {
    speed_exact = 1023 * (1 + speed / 100.0);
    microbit_pin_write_digital(pin_dir, mp_const_int_one);
  } else {
    speed_exact = 1023 * (speed / 100.0);
    microbit_pin_write_digital(pin_dir, mp_const_int_zero);
  }
  microbit_pin_write_analog(pin_speed, mp_obj_new_int(speed_exact));
}

// python - C - interface
STATIC mp_obj_t bitbot_buzz(mp_obj_t ms_in) {
  microbit_pin_write_digital(PIN_BUZZER, mp_const_int_one);
  microbit_sleep(ms_in);
  microbit_pin_write_digital(PIN_BUZZER, mp_const_int_zero);
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bitbot_buzz_obj, bitbot_buzz);

STATIC mp_obj_t bitbot_get_brightness()
{
  mp_obj_t light[2];
  microbit_pin_write_digital(PIN_LIGHT_CONTROL, mp_const_int_zero);
  light[0] = mp_obj_new_float(mp_obj_get_float(microbit_pin_read_analog(PIN_LIGHT)) / 10.23);
  microbit_pin_write_digital(PIN_LIGHT_CONTROL, mp_const_int_one);
  light[1] = mp_obj_new_float(mp_obj_get_float(microbit_pin_read_analog(PIN_LIGHT)) / 10.23);
  return mp_obj_new_tuple(2, light);
}
MP_DEFINE_CONST_FUN_OBJ_0(bitbot_get_brightness_obj, bitbot_get_brightness);

STATIC mp_obj_t bitbot_set_speed(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
  static const mp_arg_t allowed_args[] = {
      { MP_QSTR_a,    MP_ARG_OBJ, {.u_obj = mp_const_none } },
      { MP_QSTR_b,    MP_ARG_OBJ, {.u_obj = mp_const_none } },
      { MP_QSTR_left, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
      { MP_QSTR_right, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
  };

  // parse args
  mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
  mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

  bool at_least_one_arg = false;
  switch (n_args) {
    case 0:
      if (args[2].u_obj != mp_const_none) {
        setPercent(PIN_MOTOR_LEFT_DIR, PIN_MOTOR_LEFT_SPEED, mp_obj_get_float(args[2].u_obj));
        at_least_one_arg = true;
      }
      if (args[3].u_obj != mp_const_none) {
        setPercent(PIN_MOTOR_RIGHT_DIR, PIN_MOTOR_RIGHT_SPEED, mp_obj_get_float(args[3].u_obj));
        at_least_one_arg = true;
      }
      if (!at_least_one_arg) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TypeError,
                  "function expects 1 or 2 positional arguments OR keyword arguments 'left', 'right'"));
      }
      return mp_const_none;
    case 1:
      setPercent(PIN_MOTOR_LEFT_DIR, PIN_MOTOR_LEFT_SPEED, mp_obj_get_float(args[0].u_obj));
      setPercent(PIN_MOTOR_RIGHT_DIR, PIN_MOTOR_RIGHT_SPEED, mp_obj_get_float(args[0].u_obj));
      break;
    case 2:
      setPercent(PIN_MOTOR_LEFT_DIR, PIN_MOTOR_LEFT_SPEED, mp_obj_get_float(args[0].u_obj));
      setPercent(PIN_MOTOR_RIGHT_DIR, PIN_MOTOR_RIGHT_SPEED, mp_obj_get_float(args[1].u_obj));
  }
  // check that not both positional and keyword parameter was given
  if (args[2].u_obj != mp_const_none || args[3].u_obj != mp_const_none) {
    nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TypeError,
              "function expects 1 or 2 positional arguments OR keyword arguments 'left', 'right'"));
  }
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(bitbot_set_speed_obj, 0, bitbot_set_speed);



STATIC const mp_map_elem_t bitbot_module_globals_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_bitbot) },
  { MP_OBJ_NEW_QSTR(MP_QSTR_buzz), (mp_obj_t)&bitbot_buzz_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_neopixel), (mp_obj_t)&bitbot_neopixel_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_get_brightness), (mp_obj_t)&bitbot_get_brightness_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_set_speed), (mp_obj_t)&bitbot_set_speed_obj },
};

STATIC MP_DEFINE_CONST_DICT(bitbot_module_globals, bitbot_module_globals_table);

const mp_obj_module_t bitbot_module = {
    .base = { &mp_type_module },
    .name = MP_QSTR_bitbot,
    .globals = (mp_obj_dict_t*)&bitbot_module_globals,
};

}
