#ifndef __MRUBY_LIBBISMITE_COLOR_H__
#define __MRUBY_LIBBISMITE_COLOR_H__

// Color class (static)
static struct mrb_data_type const mrb_bi_color_data_type = { "Color", NULL };

static mrb_value color_obj(mrb_state *mrb,BiColor* color)
{
  struct RClass *color_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Color");
  struct RData *color_data = mrb_data_object_alloc(mrb,color_class,color,&mrb_bi_color_data_type);
  return mrb_obj_value(color_data);
}

#endif
