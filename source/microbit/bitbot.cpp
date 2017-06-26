extern "C" {

#include "py/runtime0.h"
#include "py/runtime.h"
#include "microbitobj.h"

extern const mp_obj_type_t bitbot_type;

typedef struct _bitbot_obj_t {
  mp_obj_base_t base;
  //bitbot_t bitbot;
  int i;
} bitbot_obj_t;

STATIC mp_obj_t bitbot_make_new(const mp_obj_type_t *type_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
  (void)type_in;

  bitbot_obj_t *self = m_new_obj(bitbot_obj_t);
  self->base.type = &bitbot_type;
  // bitbot_init(&self->bitbot, 42); TODO actually handle input parameters as necessary
  self->i = 42;

  return self;
}

STATIC mp_obj_t bitbot_test(mp_obj_t self_in) {
  bitbot_obj_t *self = (bitbot_obj_t*) self_in;
  return mp_obj_new_int(self->i);
}
MP_DEFINE_CONST_FUN_OBJ_1(bitbot_test_obj, bitbot_test);

STATIC const mp_map_elem_t bitbot_locals_dict_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR_test), (mp_obj_t)&bitbot_test_obj },
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
