#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/presym.h>
#include <bi/context.h>
#include <bi/layer.h>
#include "_inner_macro.h"
#include "_shader_macro.h"
#include "_blend_factor_macro.h"
#include "_node_base.h"

//
// Bi::Layer, Bi::PostProcessLayer
//
static void mrb_bi_layer_free(mrb_state *mrb, void *p)
{
  if (NULL != p) {
    bi_node_base_deinit(p);
    mrb_free(mrb,p);
  }
}
static struct mrb_data_type const mrb_layer_data_type = { "Layer", mrb_bi_layer_free };
static struct mrb_data_type const mrb_pp_layer_data_type = { "PostProcessLayer", mrb_bi_layer_free };

//
// Bi::Layer
//
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
// Scene graph
//

static mrb_value mrb_BiLayer_add_node(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiLayer* layer = DATA_PTR(self);
  BiNode* node = DATA_PTR(obj);
  bi_layer_add_node(layer,node);
  mrb_value iv_children = _iv_children_(mrb,self);
  mrb_ary_push(mrb,iv_children,obj);
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),self);
  return self;
}

static mrb_value mrb_BiLayer_remove_node(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiLayer* layer = DATA_PTR(self);
  BiNode* node = DATA_PTR(obj);
  bi_layer_remove_node(layer,node);
  mrb_value iv_children = _iv_children_(mrb,self);
  mrb_funcall(mrb,iv_children,"delete",1,obj);
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),mrb_nil_value());
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

static mrb_value mrb_BiLayer_set_shader_extra_data(mrb_state *mrb, mrb_value self)
{
  SET_SHADER_EXTRA_DATA(BiLayer);
  return self;
}
static mrb_value mrb_BiLayer_get_shader_extra_data(mrb_state *mrb, mrb_value self)
{
  mrb_int i;
  mrb_get_args(mrb, "i", &i );
  BiLayer* layer = DATA_PTR(self);
  return mrb_float_value(mrb,layer->shader_extra_data[i]);
}

// PostProcessLayer
static mrb_value mrb_pp_layer_initialize(mrb_state *mrb, mrb_value self)
{
  BiLayer* layer = mrb_malloc(mrb,sizeof(BiLayer));
  if (NULL == layer) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  bi_layer_init_as_postprocess(layer);
  mrb_data_init(self, layer, &mrb_pp_layer_data_type);
  layer->userdata = mrb_ptr(self);
  return self;
}

//
void mrb_init_bi_layer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *layer = mrb_define_class_under(mrb, bi, "Layer", mrb->object_class);
  MRB_SET_INSTANCE_TT(layer, MRB_TT_DATA);

  mrb_define_method(mrb, layer, "initialize", mrb_layer_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "add", mrb_BiLayer_add_node, MRB_ARGS_REQ(1)); // node
  mrb_define_method(mrb, layer, "remove", mrb_BiLayer_remove_node, MRB_ARGS_REQ(1)); // node

  mrb_define_method(mrb, layer, "camera_x", mrb_BiLayer_get_camera_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "camera_x=",mrb_BiLayer_set_camera_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "camera_y", mrb_BiLayer_get_camera_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "camera_y=",mrb_BiLayer_set_camera_y, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "z_order", mrb_BiLayer_get_z_order, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "z_order=",mrb_BiLayer_set_z_order, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "shader=",mrb_BiLayer_set_shader, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "set_shader_extra_data",mrb_BiLayer_set_shader_extra_data, MRB_ARGS_REQ(2)); // index,value
  mrb_define_method(mrb, layer, "get_shader_extra_data",mrb_BiLayer_get_shader_extra_data, MRB_ARGS_REQ(1)); // index

  mrb_define_method(mrb, layer, "set_texture",mrb_BiLayer_set_texture, MRB_ARGS_REQ(2));

  mrb_define_method(mrb, layer, "set_blend_factor", mrb_BiLayer_set_blend_factor, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, layer, "get_blend_factor", mrb_BiLayer_get_blend_factor, MRB_ARGS_NONE());

  struct RClass *pp_layer = mrb_define_class_under(mrb, bi, "PostProcessLayer", layer);
  MRB_SET_INSTANCE_TT(pp_layer, MRB_TT_DATA);
  mrb_define_method(mrb, pp_layer, "initialize", mrb_pp_layer_initialize, MRB_ARGS_NONE());
}
