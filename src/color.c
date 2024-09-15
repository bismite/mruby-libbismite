#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/color.h>
#include <stdlib.h>

static struct mrb_data_type const mrb_bi_color_data_type = { "Color", mrb_free };

static mrb_value mrb_bi_color_init(mrb_state *mrb, mrb_value self)
{
  mrb_int r,g,b,a;
  mrb_get_args(mrb, "iiii", &r,&g,&b,&a );
  BiColor *color = mrb_malloc(mrb, sizeof(BiColor));
  *color = RGBA(r,g,b,a);
  mrb_data_init(self, color, &mrb_bi_color_data_type);
  return self;
}

#define GETCOLOR(x) return(mrb_fixnum_value(((BiColor*)DATA_PTR(self))->x));

static mrb_value mrb_bi_color_get_r(mrb_state *mrb, mrb_value self) { GETCOLOR(r); }
static mrb_value mrb_bi_color_get_g(mrb_state *mrb, mrb_value self) { GETCOLOR(g); }
static mrb_value mrb_bi_color_get_b(mrb_state *mrb, mrb_value self) { GETCOLOR(b); }
static mrb_value mrb_bi_color_get_a(mrb_state *mrb, mrb_value self) { GETCOLOR(a); }

#define SETCOLOR(x) mrb_int c; \
  mrb_check_frozen_value(mrb,self); \
  mrb_get_args(mrb, "i", &c ); \
  ((BiColor*)DATA_PTR(self))->x = c; \
  return mrb_fixnum_value(c);

static mrb_value mrb_bi_color_set_r(mrb_state *mrb, mrb_value self) { SETCOLOR(r) }
static mrb_value mrb_bi_color_set_g(mrb_state *mrb, mrb_value self) { SETCOLOR(g) }
static mrb_value mrb_bi_color_set_b(mrb_state *mrb, mrb_value self) { SETCOLOR(b) }
static mrb_value mrb_bi_color_set_a(mrb_state *mrb, mrb_value self) { SETCOLOR(a) }

// gem

void mrb_init_bi_color(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *color = mrb_define_class_under(mrb, bi, "Color", mrb->object_class);
  MRB_SET_INSTANCE_TT(color, MRB_TT_DATA);

  mrb_define_method(mrb, color, "initialize", mrb_bi_color_init, MRB_ARGS_REQ(4)); //r,g,b,a

  mrb_define_method(mrb, color, "r", mrb_bi_color_get_r, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "g", mrb_bi_color_get_g, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "b", mrb_bi_color_get_b, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "a", mrb_bi_color_get_a, MRB_ARGS_NONE());

  mrb_define_method(mrb, color, "r=", mrb_bi_color_set_r, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, color, "g=", mrb_bi_color_set_g, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, color, "b=", mrb_bi_color_set_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, color, "a=", mrb_bi_color_set_a, MRB_ARGS_REQ(1));
}
