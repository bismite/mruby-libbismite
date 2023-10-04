#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/ext/transition.h>

//
// Bi::TransitionLayer class
//
static void mrb_transition_layer_free(mrb_state *mrb, void *p)
{
  BiTransitionLayer* transition_layer = p;
  if (NULL != transition_layer) {
    bi_node_base_deinit((BiNodeBase*)&transition_layer->node);
    bi_node_base_deinit(p);
    mrb_free(mrb,transition_layer);
  }
}
static struct mrb_data_type const mrb_transition_layer_data_type = { "TransitionLayer", mrb_transition_layer_free };

static void transition_layer_callback(BiContext* context,BiTimer* timer, double dt)
{
  mrb_state *mrb = context->userdata;
  BiLayer *transition_layer = timer->userdata;
  mrb_value self = mrb_obj_value(transition_layer->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@callback") );
  mrb_value argv[1] = {mrb_float_value(mrb,dt)};
  if( mrb_symbol_p(obj) ){
    mrb_funcall_argv(mrb,self,mrb_symbol(obj),1,argv);
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    mrb_yield_argv(mrb,obj,1,argv);
  }
}

static mrb_value mrb_transition_layer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value shader_obj, layer_group_obj_0, layer_group_obj_1, callback;
  mrb_int w,h;
  mrb_get_args(mrb, "iioooo", &w, &h, &shader_obj, &layer_group_obj_0, &layer_group_obj_1, &callback);

  BiLayerGroup *lg0 = DATA_PTR(layer_group_obj_0);
  BiLayerGroup *lg1 = DATA_PTR(layer_group_obj_1);
  BiShader *shader = DATA_PTR(shader_obj);
  BiTransitionLayer *transition_layer = mrb_malloc(mrb, sizeof(BiTransitionLayer));
  bi_transition_layer_init(transition_layer,w,h,shader,lg0,lg1,transition_layer_callback);
  transition_layer->layer.userdata = mrb_ptr(self);
  mrb_data_init(self, transition_layer, &mrb_transition_layer_data_type);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@callback"), callback );
  return self;
}


void mrb_init_transition_layer(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *transition_layer;
  transition_layer = mrb_define_class_under(mrb, bi, "TransitionLayer", mrb->object_class);
  MRB_SET_INSTANCE_TT(transition_layer, MRB_TT_DATA);

  mrb_define_method(mrb, transition_layer, "initialize", mrb_transition_layer_initialize, MRB_ARGS_REQ(6)); // w,h,shader,LayerGroup,LayerGroup,Callback
}
