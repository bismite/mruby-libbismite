#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/ext/transition.h>


// Bi::Transition class
static struct mrb_data_type const mrb_transition_data_type = { "Transition", mrb_free };

static void transition_callback(BiTransition* transition)
{
  mrb_state *mrb = transition->context->userdata;
  mrb_value self = mrb_obj_value(transition->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@callback") );
  mrb_value argv[1] = {self};
  if( mrb_symbol_p(obj) ){
    mrb_funcall_argv(mrb,self,mrb_symbol(obj),1,argv);
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    mrb_yield_argv(mrb,obj,1,argv);
  }
}

static mrb_value mrb_transition_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value layer_group_obj, shader_obj, callback;
  mrb_int duration;
  mrb_bool invert;
  mrb_get_args(mrb, "oiobo", &layer_group_obj, &duration, &shader_obj, &invert, &callback);

  BiTransition *transition = mrb_malloc(mrb, sizeof(BiTransition));
  BiLayerGroup *lg = DATA_PTR(layer_group_obj);
  BiShader *shader = DATA_PTR(shader_obj);
  bi_transition_init(transition,lg,duration,transition_callback,shader,invert);
  mrb_data_init(self, transition, &mrb_transition_data_type);
  transition->userdata = mrb_ptr(self);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@callback"), callback );
  return self;
}

//
// Bi#transition_start
//
static mrb_value mrb_transition_start(mrb_state *mrb, mrb_value self)
{
  mrb_value transition_obj;
  mrb_get_args(mrb, "o", &transition_obj);
  BiTransition *transition = DATA_PTR(transition_obj);
  BiContext *context = DATA_PTR(self);
  bi_transition_start(context,transition);
  return self;
}

void mrb_init_transition(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *transition;
  transition = mrb_define_class_under(mrb, bi, "Transition", mrb->object_class);
  MRB_SET_INSTANCE_TT(transition, MRB_TT_DATA);

  mrb_define_method(mrb, transition, "initialize", mrb_transition_initialize, MRB_ARGS_REQ(5)); // LayerGroup,Duration,Shader,Invert,Callback

  // Bi.transition_start
  mrb_define_method(mrb, bi, "transition_start", mrb_transition_start, MRB_ARGS_REQ(1)); // transition
}
