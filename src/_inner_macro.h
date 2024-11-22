#ifndef __MRUBY_LIBBISMITE_INNER_MACRO_H__
#define __MRUBY_LIBBISMITE_INNER_MACRO_H__

#define _GET_INT_(STRUCT,ATTR) \
static mrb_value mrb_##STRUCT##_get_##ATTR (mrb_state *mrb, mrb_value self) { \
  STRUCT* p = DATA_PTR(self); \
  return mrb_fixnum_value(p->ATTR); \
}

#define _GET_FLOAT_(STRUCT,ATTR) \
static mrb_value mrb_##STRUCT##_get_##ATTR (mrb_state *mrb, mrb_value self) { \
  STRUCT* p = DATA_PTR(self); \
  return mrb_float_value(mrb,p->ATTR); \
}

#define _GET_BOOL_(STRUCT,ATTR) \
static mrb_value mrb_##STRUCT##_get_##ATTR (mrb_state *mrb, mrb_value self) { \
  STRUCT* p = DATA_PTR(self); \
  return mrb_bool_value(p->ATTR); \
}

#define _SET_INT_(STRUCT,ATTR) \
static mrb_value mrb_##STRUCT##_set_##ATTR (mrb_state *mrb, mrb_value self) { \
  mrb_int i; \
  mrb_get_args(mrb, "i", &i ); \
  STRUCT* p = DATA_PTR(self); \
  p->ATTR = i; \
  return mrb_fixnum_value(i); \
}

#define _SET_FLOAT_(STRUCT,ATTR) \
static mrb_value mrb_##STRUCT##_set_##ATTR (mrb_state *mrb, mrb_value self) { \
  mrb_int f; \
  mrb_get_args(mrb, "f", &f ); \
  STRUCT* p = DATA_PTR(self); \
  p->ATTR = f; \
  return mrb_float_value(mrb,f); \
}

#define _SET_BOOL_(STRUCT,ATTR) \
static mrb_value mrb_##STRUCT##_set_##ATTR (mrb_state *mrb, mrb_value self) { \
  mrb_bool b; \
  mrb_get_args(mrb, "b", &b ); \
  STRUCT* p = DATA_PTR(self); \
  p->ATTR = b; \
  return mrb_bool_value(b); \
}

#define _GET_INT_F_(FUNC) \
static mrb_value mrb_##FUNC (mrb_state *mrb, mrb_value self) { \
  return mrb_fixnum_value( FUNC( DATA_PTR(self) ) ); \
}

#define _SET_INT_F_(FUNC) \
static mrb_value mrb_##FUNC (mrb_state *mrb, mrb_value self) { \
  mrb_int i; \
  mrb_get_args(mrb, "i", &i ); \
  FUNC(DATA_PTR(self),i); \
  return mrb_fixnum_value(i); \
}

#define _SET_FLOAT_F_(FUNC) \
static mrb_value mrb_##FUNC (mrb_state *mrb, mrb_value self) { \
  mrb_float f; \
  mrb_get_args(mrb, "f", &f ); \
  FUNC(DATA_PTR(self),f); \
  return mrb_float_value(mrb,f); \
}

#define _SET_FLOAT2_F_(FUNC) \
static mrb_value mrb_##FUNC (mrb_state *mrb, mrb_value self) { \
  mrb_float a,b; \
  mrb_value v[2]; \
  mrb_get_args(mrb, "ff", &a, &b); \
  FUNC(DATA_PTR(self),a,b); \
  v[0] = mrb_float_value(mrb,a); \
  v[1] = mrb_float_value(mrb,b); \
  return mrb_ary_new_from_values(mrb,2,v); \
}

#define _SET_INT2_F_(FUNC) \
static mrb_value mrb_##FUNC (mrb_state *mrb, mrb_value self) { \
  mrb_int a,b; \
  mrb_value v[2]; \
  mrb_get_args(mrb, "ii", &a, &b); \
  FUNC(DATA_PTR(self),a,b); \
  v[0] = mrb_fixnum_value(a); \
  v[1] = mrb_fixnum_value(b); \
  return mrb_ary_new_from_values(mrb,2,v); \
}

#endif // __MRUBY_LIBBISMITE_INNER_MACRO_H__
