#ifndef __MRUBY_BI_EXT_INNER_MACRO_H__
#define __MRUBY_BI_EXT_INNER_MACRO_H__

#define bi_mrb_fixnum_value(mrb,i) mrb_fixnum_value(i)
#define bi_mrb_bool_value(mrb,i) mrb_bool_value(i)

#define _GET_(STRUCT,ATTR,FUNC) \
  static mrb_value mrb_##STRUCT##_get_##ATTR (mrb_state *mrb, mrb_value self) \
  { \
    STRUCT* p; \
    p = DATA_PTR(self); \
    return FUNC(mrb,p->ATTR); \
  }

#define _SET_(STRUCT,ATTR,TYPE,SYMBOL) \
  static mrb_value mrb_##STRUCT##_set_##ATTR (mrb_state *mrb, mrb_value self) \
  { \
    TYPE obj; \
    STRUCT* p; \
    mrb_get_args(mrb, #SYMBOL, &obj ); \
    p = DATA_PTR(self); \
    p->ATTR = obj; \
    return self; \
  }


#endif
