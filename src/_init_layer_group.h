#ifndef __MRUBY_LIBBISMITE_INIT_LAYER_GROUP_H__
#define __MRUBY_LIBBISMITE_INIT_LAYER_GROUP_H__

static struct mrb_data_type const mrb_framebuffer_data_type = { "Framebuffer", NULL };

static inline mrb_value _init_layer_group_(mrb_state *mrb,mrb_value self,BiLayerGroup *lg){
  bi_layer_group_init(lg);
  lg->userdata = mrb_ptr(self);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_children"), mrb_ary_new(mrb));
  // framebuffer
  struct RClass *fb_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Framebuffer");
  struct RData *fb_data = mrb_data_object_alloc(mrb,fb_class,&lg->framebuffer,&mrb_framebuffer_data_type);
  mrb_value fb_obj = mrb_obj_value(fb_data);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@framebuffer"),fb_obj);
  return self;
}

#endif
