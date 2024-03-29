#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/layer.h>
#include "_blend_factor_macro.h"

//
// Bi::LayerGroup class
//
static struct mrb_data_type const mrb_layer_group_data_type = { "LayerGroup", mrb_free };

static mrb_value mrb_layer_group_initialize(mrb_state *mrb, mrb_value self)
{
  BiLayerGroup *layer_group = mrb_malloc(mrb,sizeof(BiLayerGroup));
  if (NULL == layer_group) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  bi_layer_group_init(layer_group);
  mrb_data_init(self, layer_group, &mrb_layer_group_data_type);

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@layers"),mrb_ary_new(mrb));
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@post_processes"),mrb_ary_new(mrb));
  layer_group->userdata = mrb_ptr(self);
  return self;
}

static mrb_value mrb_layer_group_set_blend_factor(mrb_state *mrb, mrb_value self)
{
  SET_BLEND_FACTOR(BiLayerGroup,blend_factor);
}

static mrb_value mrb_layer_group_get_blend_factor(mrb_state *mrb, mrb_value self)
{
  GET_BLEND_FACTOR(BiLayerGroup,blend_factor);
}

// add macro
#define _LAYER_GROUP_ADD_FUNCTION_(OBJ_STRUCT,FUNC,ATTR) \
  mrb_value obj; \
  mrb_get_args(mrb, "o", &obj ); \
  BiLayerGroup* _self = DATA_PTR(self); \
  OBJ_STRUCT* _obj = DATA_PTR(obj); \
  FUNC(_self,_obj); \
  mrb_value storage = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,ATTR)); \
  mrb_ary_push(mrb,storage,obj); \

// remove macro
#define _LAYER_GROUP_REMOVE_FUNCTION_(OBJ_STRUCT,FUNC,ATTR) \
  mrb_value obj; \
  mrb_get_args(mrb, "o", &obj ); \
  BiLayerGroup* _self = DATA_PTR(self); \
  OBJ_STRUCT* _obj = DATA_PTR(obj); \
  FUNC(_self,_obj); \
  mrb_value storage = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,ATTR)); \
  mrb_funcall(mrb,storage,"delete",1,obj); \


static mrb_value mrb_layer_group_add_layer(mrb_state *mrb, mrb_value self)
{
  _LAYER_GROUP_ADD_FUNCTION_(BiLayer,bi_layer_group_add_layer,"@layers");
  return self;
}

static mrb_value mrb_layer_group_remove_layer(mrb_state *mrb, mrb_value self)
{
  _LAYER_GROUP_REMOVE_FUNCTION_(BiLayer,bi_layer_group_remove_layer,"@layers");
  return self;
}

static mrb_value mrb_layer_group_add_layer_group(mrb_state *mrb, mrb_value self)
{
  _LAYER_GROUP_ADD_FUNCTION_(BiLayerGroup,bi_layer_group_add_layer_group,"@layers");
  return self;
}

static mrb_value mrb_layer_group_remove_layer_group(mrb_state *mrb, mrb_value self)
{
  _LAYER_GROUP_REMOVE_FUNCTION_(BiLayerGroup,bi_layer_group_remove_layer_group,"@layers");
  return self;
}

//
void mrb_init_bi_layer_group(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *layer_group;
  layer_group = mrb_define_class_under(mrb, bi, "LayerGroup", mrb->object_class);
  MRB_SET_INSTANCE_TT(layer_group, MRB_TT_DATA);

  mrb_define_method(mrb, layer_group, "initialize", mrb_layer_group_initialize, MRB_ARGS_NONE());

  mrb_define_method(mrb, layer_group, "set_blend_factor", mrb_layer_group_set_blend_factor, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, layer_group, "get_blend_factor", mrb_layer_group_get_blend_factor, MRB_ARGS_NONE());

  mrb_define_method(mrb, layer_group, "add_layer", mrb_layer_group_add_layer, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer_group, "remove_layer",mrb_layer_group_remove_layer, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer_group, "add_layer_group", mrb_layer_group_add_layer_group, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer_group, "remove_layer_group",mrb_layer_group_remove_layer_group, MRB_ARGS_REQ(1));
}
