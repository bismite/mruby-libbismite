#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/presym.h>
#include <bi/context.h>
#include <bi/shader_node.h>
#include "_inner_macro.h"
#include "_shader_macro.h"
#include "_node_base.h"

//
// Bi::ShaderNode
//
static void mrb_bi_shader_node_free(mrb_state *mrb, void *p)
{
  if (NULL != p) {
    bi_node_base_deinit(p);
    mrb_free(mrb,p);
  }
}
static struct mrb_data_type const mrb_shader_node_data_type = { "ShaderNode", mrb_bi_shader_node_free };

//
// Bi::ShaderNode
//
static mrb_value mrb_shader_node_initialize(mrb_state *mrb, mrb_value self)
{
  BiShaderNode* shader_node = mrb_malloc(mrb,sizeof(BiShaderNode));
  if (NULL == shader_node) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  bi_shader_node_init(shader_node);
  mrb_data_init(self, shader_node, &mrb_shader_node_data_type);
  shader_node->userdata = mrb_ptr(self);
  return self;
}

//
// Scene graph
//

static mrb_value mrb_BiShaderNode_add_node(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiShaderNode* shader_node = DATA_PTR(self);
  BiNode* node = DATA_PTR(obj);
  bi_node_add_node(shader_node,node);
  mrb_value iv_children = _iv_children_(mrb,self);
  mrb_ary_push(mrb,iv_children,obj);
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),self);
  return obj;
}

static mrb_value mrb_BiShaderNode_remove_node(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiShaderNode* shader_node = DATA_PTR(self);
  BiNode* node = DATA_PTR(obj);
  bi_node_remove_node(shader_node,node);
  mrb_value iv_children = _iv_children_(mrb,self);
  mrb_funcall(mrb,iv_children,"delete",1,obj);
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),mrb_nil_value());
  return obj;
}

//
// define accessors
//

_GET_INT_(BiShaderNode,camera_x);
_GET_INT_(BiShaderNode,camera_y);
_SET_INT_(BiShaderNode,camera_x);
_SET_INT_(BiShaderNode,camera_y);

_GET_INT_F_(bi_node_get_z);
_SET_INT_F_(bi_node_set_z);

static mrb_value mrb_BiShaderNode_set_texture(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_value texture_obj;
  mrb_get_args(mrb, "io", &index, &texture_obj );
  BiShaderNode* shader_node = DATA_PTR(self);
  if(mrb_nil_p(texture_obj)){
    shader_node->textures[index] = NULL;
  }else{
    BiTexture* texture = DATA_PTR(texture_obj);
    shader_node->textures[index] = texture;
  }
  return self;
}

static mrb_value mrb_BiShaderNode_set_blend_factor(mrb_state *mrb, mrb_value self)
{
  mrb_int src,dst,alpha_src,alpha_dst;
  mrb_get_args(mrb, "iiii", &src, &dst, &alpha_src, &alpha_dst );
  BiShaderNode* target = DATA_PTR(self);
  target->blend_factor = bi_blend_factor(src,dst,alpha_src,alpha_dst);
  return self;
}

static mrb_value mrb_BiShaderNode_get_blend_factor(mrb_state *mrb, mrb_value self)
{
  BiShaderNode* target = DATA_PTR(self);
  mrb_value v[4];
  v[0] = mrb_fixnum_value(target->blend_factor.src);
  v[1] = mrb_fixnum_value(target->blend_factor.dst);
  v[2] = mrb_fixnum_value(target->blend_factor.alpha_src);
  v[3] = mrb_fixnum_value(target->blend_factor.alpha_dst);
  return mrb_ary_new_from_values(mrb,4,v);
}

//
// Shader
//
static mrb_value mrb_BiShaderNode_set_shader(mrb_state *mrb, mrb_value self)
{
  mrb_value shader_obj;
  mrb_get_args(mrb, "o", &shader_obj );
  BiShaderNode* shader_node = DATA_PTR(self);
  set_shader(mrb,self,"@shader",&shader_node->shader,shader_obj);
  return self;
}

static mrb_value mrb_BiShaderNode_set_shader_extra_data(mrb_state *mrb, mrb_value self)
{
  SET_SHADER_EXTRA_DATA(BiShaderNode);
  return self;
}
static mrb_value mrb_BiShaderNode_get_shader_extra_data(mrb_state *mrb, mrb_value self)
{
  mrb_int i;
  mrb_get_args(mrb, "i", &i );
  BiShaderNode* shader_node = DATA_PTR(self);
  return mrb_float_value(mrb,shader_node->shader_extra_data[i]);
}


//
void mrb_init_bi_shader_node(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *shader_node = mrb_define_class_under(mrb, bi, "ShaderNode", mrb->object_class);
  MRB_SET_INSTANCE_TT(shader_node, MRB_TT_DATA);

  mrb_define_method(mrb, shader_node, "initialize", mrb_shader_node_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader_node, "_add_node_", mrb_BiShaderNode_add_node, MRB_ARGS_REQ(1)); // node
  mrb_define_method(mrb, shader_node, "_remove_node_", mrb_BiShaderNode_remove_node, MRB_ARGS_REQ(1)); // node

  mrb_define_method(mrb, shader_node, "camera_x", mrb_BiShaderNode_get_camera_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader_node, "camera_x=",mrb_BiShaderNode_set_camera_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, shader_node, "camera_y", mrb_BiShaderNode_get_camera_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader_node, "camera_y=",mrb_BiShaderNode_set_camera_y, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, shader_node, "z_order", mrb_bi_node_get_z, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader_node, "z_order=",mrb_bi_node_set_z, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, shader_node, "shader=",mrb_BiShaderNode_set_shader, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, shader_node, "set_shader_extra_data",mrb_BiShaderNode_set_shader_extra_data, MRB_ARGS_REQ(2)); // index,value
  mrb_define_method(mrb, shader_node, "get_shader_extra_data",mrb_BiShaderNode_get_shader_extra_data, MRB_ARGS_REQ(1)); // index

  mrb_define_method(mrb, shader_node, "_set_texture_",mrb_BiShaderNode_set_texture, MRB_ARGS_REQ(2)); // num,tex

  mrb_define_method(mrb, shader_node, "set_blend_factor", mrb_BiShaderNode_set_blend_factor, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, shader_node, "get_blend_factor", mrb_BiShaderNode_get_blend_factor, MRB_ARGS_NONE());
}
