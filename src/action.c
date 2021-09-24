#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/variable.h>
#include <bi/util.h>
#include <bi/ext/action.h>
#include <stdlib.h>
#include "bi_ext_inner_macro.h"

// callback on finish

typedef struct {
  mrb_state *mrb;
  mrb_value action;
} mrb_action_finish_callback_context;

void mrb_action_finish_callback(BiAction* action, void* context)
{
  mrb_action_finish_callback_context *c = context;
  mrb_state *mrb = c->mrb;
  mrb_value self = c->action;
  mrb_value node = mrb_obj_value(action->node->userdata);
  mrb_value callback = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@callback") );
  mrb_value argv[2] = { node, self };
  if( mrb_symbol_p(callback) ){
    mrb_funcall_argv(mrb,node,mrb_symbol(callback),2,argv);
  }else if( mrb_type(callback) == MRB_TT_PROC ) {
    mrb_yield_argv(mrb,callback,2,argv);
  }
}


//
// Bi::Node
//

static mrb_value mrb_add_action(mrb_state *mrb, mrb_value self)
{
    BiNode *node;
    BiAction *action;
    mrb_action_finish_callback_context *callback_context;
    mrb_value action_obj;
    mrb_get_args(mrb, "o", &action_obj);
    // TODO: check error
    node = DATA_PTR(self);
    action = DATA_PTR(action_obj);
    callback_context = action->on_finish_callback_context;
    bi_add_action(node,action);
    bi_action_start(node,action,bi_get_now());
    return self;
}

static mrb_value mrb_remove_action(mrb_state *mrb, mrb_value self)
{
    mrb_value action_obj;
    mrb_get_args(mrb, "o", &action_obj);
    // TODO: check error
    BiNode *node = DATA_PTR(self);
    BiAction *action = DATA_PTR(action_obj);
    bi_remove_action(node,action);
    return self;
}

//
// Bi::Action
//

void mrb_action_free(mrb_state *mrb,void* p){
  BiAction *action = p;
  mrb_free(mrb,action->timer);
  mrb_free(mrb,action->on_finish_callback_context);
  mrb_free(mrb,action);
}

static struct mrb_data_type const mrb_action_data_type = { "Action", mrb_action_free };

static BiAction* action_initialize(mrb_state *mrb, mrb_value *self,mrb_value callback,size_t size)
{
  BiAction* action = mrb_malloc(mrb,sizeof(BiAction));
  bi_action_init(action);
  action->timer = mrb_malloc(mrb,sizeof(BiTimer));
  action->action_data = mrb_malloc(mrb,size);
  DATA_PTR(*self) = action;
  DATA_TYPE(*self) = &mrb_action_data_type;

  mrb_action_finish_callback_context* c = mrb_malloc(mrb,sizeof(mrb_action_finish_callback_context));
  c->mrb = mrb;
  c->action = *self;
  action->on_finish_callback_context = c;
  action->on_finish = mrb_action_finish_callback;

  mrb_iv_set(mrb, *self, mrb_intern_cstr(mrb,"@callback"), callback );

  return action;
}

//
// Bi::Action::Base
//

static mrb_value mrb_action_base_initialize(mrb_state *mrb, mrb_value self)
{
  BiAction *action;
  mrb_value callback;
  mrb_get_args(mrb, "o", &callback);
  action = action_initialize(mrb,&self,callback,0);
  bi_action_base_init(action);
  return self;
}

//
// Bi::Action::MoveTo
//

static mrb_value mrb_action_move_to_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_int x,y;
  mrb_value callback;
  BiAction *action;
  mrb_get_args(mrb, "fiio", &duration, &x, &y, &callback);
  action = action_initialize(mrb,&self,callback,sizeof(BiActionMoveTo));
  bi_action_move_to_init(action,duration,x,y);
  return self;
}

//
// Bi::Action::RotateTo
//

static mrb_value mrb_action_rotate_to_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_float angle;
  mrb_value callback;
  BiAction *action;
  mrb_get_args(mrb, "ffo", &duration, &angle, &callback);
  action = action_initialize(mrb,&self,callback,sizeof(BiActionRotateTo));
  bi_action_rotate_to_init(action,duration,angle);
  return self;
}

//
// Bi::Action::RotateBy
//

static mrb_value mrb_action_rotate_by_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_float angle;
  mrb_value callback;
  BiAction *action;
  mrb_get_args(mrb, "ffo", &duration, &angle, &callback);
  action = action_initialize(mrb,&self,callback,sizeof(BiActionRotateBy));
  bi_action_rotate_by_init(action,duration,angle);
  return self;
}

//
// Bi::Action::Repeat
//

static mrb_value mrb_action_repeat_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_value target_action_obj;
  mrb_value callback;
  BiAction *action;
  mrb_get_args(mrb, "oo", &target_action_obj, &callback);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_target_action_"), target_action_obj);
  action = action_initialize(mrb,&self,callback,sizeof(BiActionRepeat));
  bi_action_repeat_init( action, DATA_PTR(target_action_obj) );
  return self;
}

//
// Bi::Action::Sequence
//

static mrb_value mrb_action_sequence_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value actions_obj;
  mrb_value callback;
  BiAction *action;
  BiAction **actions;
  mrb_get_args(mrb, "Ao", &actions_obj, &callback);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_actions_"), actions_obj);
  action = action_initialize(mrb,&self,callback,sizeof(BiActionSequence));
  actions = malloc( sizeof(BiAction*) * RARRAY_LEN(actions_obj) );
  for(int i=0; i<RARRAY_LEN(actions_obj); i++ ) {
    actions[i] = DATA_PTR( RARRAY_PTR(actions_obj)[i] );
  }
  bi_action_sequence_init(action, RARRAY_LEN(actions_obj), actions );
  free(actions);
  return self;
}

//
//
//

void mrb_init_action(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *node;
  struct RClass *action;
  struct RClass *base;
  struct RClass *move_to;
  struct RClass *rotate_to;
  struct RClass *rotate_by;
  struct RClass *repeat;
  struct RClass *sequence;

  node = mrb_class_get_under(mrb, bi, "Node");
  mrb_define_method(mrb, node, "_add_action", mrb_add_action, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_remove_action", mrb_remove_action, MRB_ARGS_REQ(1));

  action = mrb_define_module_under(mrb,bi,"Action");

  base = mrb_define_class_under(mrb, action, "Base", mrb->object_class);
  MRB_SET_INSTANCE_TT(base, MRB_TT_DATA);
  mrb_define_method(mrb, base, "initialize", mrb_action_base_initialize, MRB_ARGS_REQ(1)); // callback

  move_to = mrb_define_class_under(mrb, action, "MoveTo", base);
  MRB_SET_INSTANCE_TT(move_to, MRB_TT_DATA);
  mrb_define_method(mrb, move_to, "initialize", mrb_action_move_to_initialize, MRB_ARGS_REQ(4)); // duration,x,y,callback

  rotate_to = mrb_define_class_under(mrb, action, "RotateTo", base);
  MRB_SET_INSTANCE_TT(rotate_to, MRB_TT_DATA);
  mrb_define_method(mrb, rotate_to, "initialize", mrb_action_rotate_to_initialize, MRB_ARGS_REQ(3)); // duration,angle,callback

  rotate_by = mrb_define_class_under(mrb, action, "RotateBy", base);
  MRB_SET_INSTANCE_TT(rotate_by, MRB_TT_DATA);
  mrb_define_method(mrb, rotate_by, "initialize", mrb_action_rotate_by_initialize, MRB_ARGS_REQ(3)); // duration,angle,callback

  repeat = mrb_define_class_under(mrb, action, "Repeat", base);
  MRB_SET_INSTANCE_TT(repeat, MRB_TT_DATA);
  mrb_define_method(mrb, repeat, "initialize", mrb_action_repeat_initialize, MRB_ARGS_REQ(2)); // action, callback

  sequence = mrb_define_class_under(mrb, action, "Sequence", base);
  MRB_SET_INSTANCE_TT(sequence, MRB_TT_DATA);
  mrb_define_method(mrb, sequence, "initialize", mrb_action_sequence_initialize, MRB_ARGS_REQ(2)); // actions, callback
}
