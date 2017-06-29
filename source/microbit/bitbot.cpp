#include "microbitobj.h"
#include "MicroBit.h"


// set pins
#define PIN_BUZZER            ((microbit_pin_obj_t*)&microbit_p14_obj)
#define PIN_LINE_LEFT         ((microbit_pin_obj_t*)&microbit_p11_obj)
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

const neopixel_obj_t bitbot_neopixel = {
  &neopixel_type,
  {
    NAME_PIN_NEOPIXEL,
    NEOPIXEL_NUM_LEDS,
    bitbot_neopixel_init()
  }
};

STATIC mp_obj_t bitbot_buzz(mp_obj_t ms_in) {
  microbit_pin_write_digital(PIN_BUZZER, mp_obj_new_int(1));
  microbit_sleep(ms_in);
  microbit_pin_write_digital(PIN_BUZZER, mp_obj_new_int(0));
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bitbot_buzz_obj, bitbot_buzz);


STATIC const mp_map_elem_t bitbot_module_globals_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_bitbot) },
  { MP_OBJ_NEW_QSTR(MP_QSTR_buzz), (mp_obj_t)&bitbot_buzz_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_neopixel), (mp_obj_t)&bitbot_neopixel },
};

STATIC MP_DEFINE_CONST_DICT(bitbot_module_globals, bitbot_module_globals_table);

const mp_obj_module_t bitbot_module = {
    .base = { &mp_type_module },
    .name = MP_QSTR_bitbot,
    .globals = (mp_obj_dict_t*)&bitbot_module_globals,
};

}
