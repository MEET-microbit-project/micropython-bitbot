//#include "libbitbot.h"

#include "MicroBit.h"


extern "C" {

#include "gpio_api.h"
#include "py/runtime0.h"
#include "py/runtime.h"
#include "py/nlr.h"
#include "microbitobj.h"
#include "lib/neopixel.h"

extern const mp_obj_type_t bitbot_type;

typedef struct _bitbot_obj_t {
  mp_obj_base_t base;

  neopixel_strip_t neopixel_strip;
} bitbot_obj_t;

STATIC mp_obj_t bitbot_make_new(const mp_obj_type_t *type_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
  (void)type_in;

  bitbot_obj_t *self = m_new_obj(bitbot_obj_t);
  self->base.type = &bitbot_type;

  // init neopixel
  neopixel_init(&self->neopixel_strip, MICROBIT_PIN_P13, 12);

  return self;
}

STATIC mp_obj_t bitbot_set_rgb(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
  bitbot_obj_t *self = (bitbot_obj_t*) pos_args[0];
  mp_uint_t index = mp_get_index(self->base.type, self->neopixel_strip.num_leds, pos_args[1], false);

  mp_int_t r = mp_obj_get_int(pos_args[2]);
  mp_int_t g = mp_obj_get_int(pos_args[3]);
  mp_int_t b = mp_obj_get_int(pos_args[4]);
  if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
      nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid colour"));
  }
  neopixel_set_color(&self->neopixel_strip, index, r, g, b);
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bitbot_set_rgb_obj, 5, 5, bitbot_set_rgb);

STATIC mp_obj_t bitbot_get_rgb(mp_obj_t self_in, mp_obj_t index_in)
{
  bitbot_obj_t *self = (bitbot_obj_t*) self_in;
  mp_uint_t index = mp_get_index(self->base.type, self->neopixel_strip.num_leds, index_in, false);

  mp_obj_t rgb[3] = {
      MP_OBJ_NEW_SMALL_INT(self->neopixel_strip.leds[index].simple.r),
      MP_OBJ_NEW_SMALL_INT(self->neopixel_strip.leds[index].simple.g),
      MP_OBJ_NEW_SMALL_INT(self->neopixel_strip.leds[index].simple.b),
  };
  return mp_obj_new_tuple(3, rgb);
}
MP_DEFINE_CONST_FUN_OBJ_2(bitbot_get_rgb_obj, bitbot_get_rgb);

STATIC mp_obj_t bitbot_clear(mp_obj_t self_in) {
  bitbot_obj_t *self = (bitbot_obj_t*)self_in;
  neopixel_clear(&self->neopixel_strip);
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bitbot_clear_obj, bitbot_clear);

STATIC mp_obj_t bitbot_show(mp_obj_t self_in) {
  bitbot_obj_t *self = (bitbot_obj_t*)self_in;
  neopixel_show(&self->neopixel_strip);
  return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bitbot_show_obj, bitbot_show);

STATIC const mp_map_elem_t bitbot_locals_dict_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR_set_rgb), (mp_obj_t)&bitbot_set_rgb_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_get_rgb), (mp_obj_t)&bitbot_get_rgb_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_clear), (mp_obj_t)&bitbot_clear_obj },
  { MP_OBJ_NEW_QSTR(MP_QSTR_show), (mp_obj_t)&bitbot_show_obj },
  // { MP_OBJ_NEW_QSTR(MP_QSTR_test), (mp_obj_t)&bitbot_test_obj },
};

STATIC MP_DEFINE_CONST_DICT(bitbot_locals_dict, bitbot_locals_dict_table);

const mp_obj_type_t bitbot_type = {
  { &mp_type_type },
  .name = MP_QSTR_BitBot,
  .print = NULL,
  .make_new = bitbot_make_new,
  .call = NULL,
  .unary_op = NULL,
  .binary_op = NULL,
  .attr = NULL,
  .subscr = NULL,
  .getiter = NULL,
  .iternext = NULL,
  .buffer_p = {NULL},
  .stream_p = NULL,
  .bases_tuple = NULL,
  .locals_dict = (mp_obj_dict_t*)&bitbot_locals_dict,
};

STATIC const mp_map_elem_t bitbot_module_globals_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_bitbot) },
  { MP_OBJ_NEW_QSTR(MP_QSTR_BitBot), (mp_obj_t)&bitbot_type },
};

STATIC MP_DEFINE_CONST_DICT(bitbot_module_globals, bitbot_module_globals_table);

const mp_obj_module_t bitbot_module = {
    .base = { &mp_type_module },
    .name = MP_QSTR_bitbot,
    .globals = (mp_obj_dict_t*)&bitbot_module_globals,
};

}
