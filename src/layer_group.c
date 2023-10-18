#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/presym.h>
#include <bi/layer.h>
#include "_blend_factor_macro.h"
#include "_init_layer_group.h"

//
// Bi::LayerGroup class
//
static void mrb_bi_layer_group_free(mrb_state *mrb, void *p)
{
  if (NULL != p) {
    bi_node_base_deinit(p);
    mrb_free(mrb,p);
  }
}
static struct mrb_data_type const mrb_layer_group_data_type = { "LayerGroup", mrb_bi_layer_group_free };

static mrb_value mrb_layer_group_initialize(mrb_state *mrb, mrb_value self)
{
  BiLayerGroup *layer_group = mrb_malloc(mrb,sizeof(BiLayerGroup));
  mrb_data_init(self, layer_group, &mrb_layer_group_data_type);
  return _init_layer_group_(mrb,self,layer_group);
}

static mrb_value mrb_layer_group_set_blend_factor(mrb_state *mrb, mrb_value self)
{
  SET_BLEND_FACTOR(BiLayerGroup,blend_factor);
}

static mrb_value mrb_layer_group_get_blend_factor(mrb_state *mrb, mrb_value self)
{
  GET_BLEND_FACTOR(BiLayerGroup,blend_factor);
}

static mrb_value mrb_layer_group_add(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiLayerGroup* lg = DATA_PTR(self);
  struct RClass* layer_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Layer");
  struct RClass* layer_group_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"LayerGroup");
  if( mrb_obj_is_kind_of(mrb,obj,layer_class) ) {
    bi_layer_group_add_layer(lg, DATA_PTR(obj) );
  }else if( mrb_obj_is_kind_of(mrb,obj,layer_group_class) ) {
    bi_layer_group_add_layer_group(lg, DATA_PTR(obj) );
  }else{
    return mrb_nil_value();
  }
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),self);
  mrb_value children = mrb_iv_get(mrb, self, MRB_IVSYM(_children) );
  mrb_ary_push(mrb,children,obj);
  return obj;
}

static mrb_value mrb_layer_group_remove(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiLayerGroup* lg = DATA_PTR(self);
  struct RClass* layer_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Layer");
  struct RClass* layer_group_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"LayerGroup");
  if( mrb_obj_is_kind_of(mrb,obj,layer_class) ) {
    bi_layer_group_remove_layer(lg, DATA_PTR(obj) );
  }else if( mrb_obj_is_kind_of(mrb,obj,layer_group_class) ) {
    bi_layer_group_remove_layer_group(lg, DATA_PTR(obj) );
  }else{
    return mrb_nil_value();
  }
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),mrb_nil_value());
  mrb_value children = mrb_iv_get(mrb, self, MRB_IVSYM(_children) );
  mrb_funcall(mrb,children,"delete",1,obj);
  return obj;
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
  mrb_define_method(mrb, layer_group, "add", mrb_layer_group_add, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer_group, "remove",mrb_layer_group_remove, MRB_ARGS_REQ(1));
}
