extern const mp_obj_type_t neopixel_type;

typedef struct _neopixel_obj_t {
    mp_obj_base_t base;
    neopixel_strip_t strip;
} neopixel_obj_t;

mp_obj_t neopixel_make_new(const mp_obj_type_t *type_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args);
