#ifndef __MRUBY_LIBBISMITE_BLEND_FACTOR_MACRO_H__
#define __MRUBY_LIBBISMITE_BLEND_FACTOR_MACRO_H__

#define SET_BLEND_FACTOR(CLASS,ATTR) \
  mrb_int src,dst,alpha_src,alpha_dst; \
  mrb_get_args(mrb, "iiii", &src, &dst, &alpha_src, &alpha_dst ); \
  CLASS* target = DATA_PTR(self); \
  bi_set_blend_factor(&target->ATTR,src,dst,alpha_src,alpha_dst); \
  return self;

#define GET_BLEND_FACTOR(CLASS,ATTR) \
  CLASS* target = DATA_PTR(self); \
  mrb_value v[4]; \
  v[0] = mrb_fixnum_value(target->ATTR.src); \
  v[1] = mrb_fixnum_value(target->ATTR.dst); \
  v[2] = mrb_fixnum_value(target->ATTR.alpha_src); \
  v[3] = mrb_fixnum_value(target->ATTR.alpha_dst); \
  return mrb_ary_new_from_values(mrb,4,v); \

#endif
