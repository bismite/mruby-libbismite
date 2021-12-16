#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/canvas.h>
#include <bi/texture.h>
#include <bi_core.h>
#include "_shader_inner_macro.h"

//
// Bi::Canvas class
//
static struct mrb_data_type const mrb_canvas_data_type = { "Canvas", mrb_free };

static mrb_value mrb_bi_canvas_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int w,h;
  mrb_get_args(mrb, "ii", &w,&h );
  BiCanvas* canvas = bi_canvas_init(mrb_malloc(mrb,sizeof(BiCanvas)),w,h);
  mrb_data_init(self, canvas, &mrb_canvas_data_type);
  return self;
}

static mrb_value mrb_bi_canvas_clear(mrb_state *mrb, mrb_value self)
{
  mrb_int r,g,b,a;
  mrb_get_args(mrb, "iiii", &r,&g,&b,&a );
  BiCanvas* canvas = DATA_PTR(self);
  bi_canvas_clear(canvas,r,g,b,a);
  return self;
}

static mrb_value mrb_bi_canvas_set_texture(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_value texture_obj;
  mrb_get_args(mrb, "io", &index,&texture_obj);
  BiCanvas* canvas = DATA_PTR(self);
  BiTexture* texture = DATA_PTR(texture_obj);
  canvas->textures[index] = texture;
  return self;
}

static mrb_value mrb_bi_canvas_set_shader(mrb_state *mrb, mrb_value self)
{
  mrb_value shader_obj;
  mrb_get_args(mrb, "o", &shader_obj );
  BiCanvas* canvas = DATA_PTR(self);
  set_shader(mrb,self,"@shader",&canvas->shader,shader_obj);
  return self;
}

static mrb_value mrb_bi_canvas_set_shader_attribute(mrb_state *mrb, mrb_value self)
{
  SET_SHADER_ATTRIBUTE(BiCanvas,shader_attributes);
  return self;
}

static mrb_value mrb_bi_canvas_draw(mrb_state *mrb, mrb_value self)
{
  mrb_value node_obj;
  mrb_get_args(mrb, "o", &node_obj );
  BiNode* node = DATA_PTR(node_obj);
  BiCanvas* canvas = DATA_PTR(self);
  bi_canvas_draw(canvas,node);
  return self;
}

static mrb_value mrb_bi_canvas_to_texture(mrb_state *mrb, mrb_value self)
{
  BiCanvas* canvas = DATA_PTR(self);
  BiTexture* texture = mrb_malloc(mrb,sizeof(BiTexture));
  mrb_value texture_obj = create_bi_texture(mrb,texture);
  bi_canvas_to_texture(canvas,texture);
  return texture_obj;
}

static mrb_value mrb_bi_canvas_save_png(mrb_state *mrb, mrb_value self)
{
  const char* filename;
  mrb_get_args(mrb, "z", &filename );
  BiCanvas* canvas = DATA_PTR(self);
  bi_canvas_save_png_image(canvas,filename);
  return self;
}

void mrb_init_bi_canvas(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *canvas = mrb_define_class_under(mrb, bi, "Canvas", mrb->object_class);
  MRB_SET_INSTANCE_TT(canvas, MRB_TT_DATA);
  mrb_define_method(mrb,canvas,"initialize", mrb_bi_canvas_initialize, MRB_ARGS_REQ(2)); // w,h
  mrb_define_method(mrb,canvas,"clear",      mrb_bi_canvas_clear,      MRB_ARGS_REQ(4)); // r,g,b,a
  mrb_define_method(mrb,canvas,"set_texture",mrb_bi_canvas_set_texture,MRB_ARGS_REQ(2)); // index,texture
  mrb_define_method(mrb,canvas,"shader=",    mrb_bi_canvas_set_shader, MRB_ARGS_REQ(1)); // shader
  mrb_define_method(mrb,canvas,"set_shader_attribute",mrb_bi_canvas_set_shader_attribute,MRB_ARGS_REQ(2)); // index,value
  mrb_define_method(mrb,canvas,"draw",       mrb_bi_canvas_draw,       MRB_ARGS_REQ(1)); // Node
  mrb_define_method(mrb,canvas,"to_texture", mrb_bi_canvas_to_texture, MRB_ARGS_NONE());
  mrb_define_method(mrb,canvas,"save_png",   mrb_bi_canvas_save_png,   MRB_ARGS_REQ(1)); // filename

}
