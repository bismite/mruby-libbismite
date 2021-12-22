#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/context.h>
#include <bi/layer.h>
#include "_inner_macro.h"
#include "_shader_macro.h"
#include "_blend_factor_macro.h"

//
// Bi::Layer class
//

static struct mrb_data_type const mrb_layer_data_type = { "Layer", mrb_free };

static mrb_value mrb_layer_initialize(mrb_state *mrb, mrb_value self)
{
  BiLayer* layer = mrb_malloc(mrb,sizeof(BiLayer));
  if (NULL == layer) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  bi_layer_init(layer);
  mrb_data_init(self, layer, &mrb_layer_data_type);
  layer->userdata = mrb_ptr(self);
  return self;
}

//
// define accessors
//
_GET_(BiLayer,camera_x,bi_mrb_fixnum_value);
_SET_(BiLayer,camera_x,mrb_int,i);
_GET_(BiLayer,camera_y,bi_mrb_fixnum_value);
_SET_(BiLayer,camera_y,mrb_int,i);

_GET_FUNC_(BiLayer,z_order,bi_layer_get_z_order,bi_mrb_fixnum_value);
_SET_FUNC_(BiLayer,z_order,mrb_int,i,bi_layer_set_z_order);

static mrb_value mrb_BiLayer_set_root(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  // TODO: error check
  BiLayer* layer = DATA_PTR(self);
  BiNode* node = DATA_PTR(obj);
  layer->root = node;
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@root"), obj);
  return self;
}

static mrb_value mrb_BiLayer_get_root(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@root"));
}

static mrb_value mrb_BiLayer_set_texture(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_value texture_obj;
  mrb_get_args(mrb, "io", &index, &texture_obj );
  BiLayer* layer = DATA_PTR(self);
  BiTexture* texture = DATA_PTR(texture_obj);
  layer->textures[index] = texture;
  return self;
}

static mrb_value mrb_BiLayer_set_blend_factor(mrb_state *mrb, mrb_value self)
{
  SET_BLEND_FACTOR(BiLayer,blend_factor);
}

static mrb_value mrb_BiLayer_get_blend_factor(mrb_state *mrb, mrb_value self)
{
  GET_BLEND_FACTOR(BiLayer,blend_factor);
}

//
// Shader
//
static mrb_value mrb_BiLayer_set_shader(mrb_state *mrb, mrb_value self)
{
  mrb_value shader_obj;
  mrb_get_args(mrb, "o", &shader_obj );
  BiLayer* layer = DATA_PTR(self);
  set_shader(mrb,self,"@shader",&layer->shader,shader_obj);
  return self;
}

static mrb_value mrb_BiLayer_set_post_process_shader(mrb_state *mrb, mrb_value self)
{
  mrb_value shader_obj;
  mrb_get_args(mrb, "o", &shader_obj );
  BiLayer* layer = DATA_PTR(self);
  set_shader(mrb,self,"@post_process_shader",&layer->post_process.shader,shader_obj);
  return self;
}

static mrb_value mrb_BiLayer_set_shader_attribute(mrb_state *mrb, mrb_value self)
{
  SET_SHADER_ATTRIBUTE(BiLayer,shader_attributes);
  return self;
}

static mrb_value mrb_BiLayer_set_post_process_shader_attribute(mrb_state *mrb, mrb_value self)
{
  SET_SHADER_ATTRIBUTE(BiLayer,post_process.shader_attributes);
  return self;
}

//
// post process
//

static mrb_value mrb_BiLayer_set_post_process_framebuffer_enabled(mrb_state *mrb, mrb_value self)
{
  mrb_bool framebuffer_enabled;
  mrb_get_args(mrb, "b", &framebuffer_enabled );
  BiLayer* layer = DATA_PTR(self);
  layer->post_process.framebuffer_enabled = framebuffer_enabled;
  return self;
}


static mrb_value mrb_BiLayer_set_post_process_blend_factor(mrb_state *mrb, mrb_value self)
{
  SET_BLEND_FACTOR(BiLayer,post_process.blend_factor);
}

static mrb_value mrb_BiLayer_get_post_process_blend_factor(mrb_state *mrb, mrb_value self)
{
  GET_BLEND_FACTOR(BiLayer,post_process.blend_factor);
}

//
void mrb_init_bi_layer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *layer;
  layer = mrb_define_class_under(mrb, bi, "Layer", mrb->object_class);
  MRB_SET_INSTANCE_TT(layer, MRB_TT_DATA);

  mrb_define_method(mrb, layer, "initialize", mrb_layer_initialize, MRB_ARGS_NONE());

  mrb_define_method(mrb, layer, "camera_x", mrb_BiLayer_get_camera_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "camera_x=",mrb_BiLayer_set_camera_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "camera_y", mrb_BiLayer_get_camera_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "camera_y=",mrb_BiLayer_set_camera_y, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "z_order", mrb_BiLayer_get_z_order, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "z_order=",mrb_BiLayer_set_z_order, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "root", mrb_BiLayer_get_root, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "root=",mrb_BiLayer_set_root, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "shader=",mrb_BiLayer_set_shader, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "set_shader_attribute",mrb_BiLayer_set_shader_attribute, MRB_ARGS_REQ(2)); // index,value

  mrb_define_method(mrb, layer, "set_texture",mrb_BiLayer_set_texture, MRB_ARGS_REQ(2));

  mrb_define_method(mrb, layer, "set_blend_factor", mrb_BiLayer_set_blend_factor, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, layer, "get_blend_factor", mrb_BiLayer_get_blend_factor, MRB_ARGS_NONE());

  mrb_define_method(mrb, layer, "set_post_process_shader",mrb_BiLayer_set_post_process_shader, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "set_post_process_shader_attribute",mrb_BiLayer_set_post_process_shader_attribute, MRB_ARGS_REQ(2)); // index,value
  mrb_define_method(mrb, layer, "set_post_process_framebuffer_enabled",mrb_BiLayer_set_post_process_framebuffer_enabled, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "set_post_process_blend_factor",mrb_BiLayer_set_post_process_blend_factor, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, layer, "get_post_process_blend_factor",mrb_BiLayer_get_post_process_blend_factor, MRB_ARGS_NONE());
}
