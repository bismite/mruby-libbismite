#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <bi/node.h>
#include <bi/ext/label.h>
#include <stdlib.h>
#include "_color.h"

// Bi::Label class
static void label_free(mrb_state *mrb, void *p)
{
  if(p==NULL) return;
  BiLabel* label = p;
  bi_label_deinit(label);
  mrb_free(mrb, p);
}

static struct mrb_data_type const mrb_label_data_type = { "Label", label_free };

static mrb_value mrb_label_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value font_obj;
  mrb_get_args(mrb, "o", &font_obj);
  BiFont *font = DATA_PTR(font_obj);
  BiLabel* label = bi_label_init(mrb_malloc(mrb, sizeof(BiLabel)),font);
  mrb_data_init(self, label, &mrb_label_data_type);
  label->node.userdata = mrb_ptr(self);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@font"), font_obj);
  // Color
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_tint_"), color_obj(mrb,&label->tint) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_color_"), color_obj(mrb,&label->color) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_background_color_"), color_obj(mrb,&label->node.color) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_background_tint_"), color_obj(mrb,&label->node.tint) );
  return self;
}

static mrb_value mrb_label_set_text(mrb_state *mrb, mrb_value self)
{
  mrb_value text;
  mrb_get_args(mrb, "S", &text );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@text"), text);
  BiLabel* n = DATA_PTR(self);
  bi_label_set_text(n,mrb_string_value_cstr(mrb,&text));
  return self;
}

//
// text color
//
static mrb_value mrb_node_get_color(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_color_") );
}
static mrb_value mrb_node_get_tint(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_tint_") );
}

static mrb_value mrb_label_set_color(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiLabel* label = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  bi_label_set_color(label, *color );
  return self;
}
static mrb_value mrb_label_set_tint(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiLabel* label = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  bi_label_set_tint(label, *color );
  return self;
}

static mrb_value mrb_label_set_color_with_range(mrb_state *mrb, mrb_value self)
{
  mrb_int start,end;
  mrb_value color_obj;
  mrb_get_args(mrb, "iio", &start, &end, &color_obj );
  BiLabel* label = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  bi_label_set_color_with_range(label,start,end,*color);
  return self;
}
static mrb_value mrb_label_set_tint_with_range(mrb_state *mrb, mrb_value self)
{
  mrb_int start,end;
  mrb_value color_obj;
  mrb_get_args(mrb, "iio", &start, &end, &color_obj );
  BiLabel* label = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  bi_label_set_tint_with_range(label,start,end,*color);
  return self;
}

//
// background color
//
static mrb_value mrb_label_set_background_color(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiLabel* label = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  bi_label_set_background_color(label,*color);
  return self;
}
static mrb_value mrb_label_set_background_tint(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiLabel* label = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  bi_label_set_background_tint(label,*color);
  return self;
}

//
static mrb_value mrb_label_anchor_reposition(mrb_state *mrb, mrb_value self)
{
  BiLabel* label = DATA_PTR(self);
  bi_label_anchor_reposition(label);
  return self;
}

void mrb_init_label(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *node = mrb_class_get_under(mrb, bi, "Node");
  struct RClass *label = mrb_define_class_under(mrb, bi, "Label", node);
  MRB_SET_INSTANCE_TT(label, MRB_TT_DATA);
  mrb_define_method(mrb, label, "initialize", mrb_label_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, label, "set_text", mrb_label_set_text, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, label, "text", mrb_label_set_text, MRB_ARGS_NONE());
  // text color
  mrb_define_method(mrb, label, "_set_color_", mrb_label_set_color, MRB_ARGS_REQ(1)); // color
  mrb_define_method(mrb, label, "_set_tint_", mrb_label_set_tint, MRB_ARGS_REQ(1)); // color
  mrb_define_method(mrb, label, "set_color_with_range", mrb_label_set_color_with_range, MRB_ARGS_REQ(3) ); // start,end,color
  mrb_define_method(mrb, label, "set_tint_with_range", mrb_label_set_tint_with_range, MRB_ARGS_REQ(3) ); // start,end,color
  // background color
  mrb_define_method(mrb, label, "_set_background_color_", mrb_label_set_background_color, MRB_ARGS_REQ(1)); // color
  mrb_define_method(mrb, label, "_set_background_tint_", mrb_label_set_background_tint, MRB_ARGS_REQ(1)); // color
  //
  mrb_define_method(mrb, label, "anchor_reposition", mrb_label_anchor_reposition, MRB_ARGS_NONE());
}
