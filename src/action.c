#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/variable.h>
#include <mruby/presym.h>
#include <bi/util.h>
#include <bi/ext/action.h>
#include <stdlib.h>
#include "_inner_macro.h"

// callback on finish

typedef struct {
  mrb_state *mrb;
  mrb_value action;
} mrb_action_finish_callback_context;

void mrb_action_finish_callback(BiAction* action, void* context)
{
  mrb_action_finish_callback_context *c = context;
  mrb_state *mrb = c->mrb;
  mrb_value timer_obj = c->action;
  mrb_value node_obj = mrb_obj_value(action->node->userdata);
  mrb_value callback = mrb_iv_get(mrb, timer_obj, mrb_intern_cstr(mrb,"@_callback") );
  mrb_value argv[2] = { node_obj, timer_obj };
  if( mrb_symbol_p(callback) ){
    mrb_funcall_argv(mrb,node_obj,mrb_symbol(callback),2,argv);
  }else if( mrb_type(callback) == MRB_TT_PROC ) {
    mrb_yield_argv(mrb,callback,2,argv);
  }
  mrb_value autoremove = mrb_iv_get(mrb, timer_obj, mrb_intern_cstr(mrb,"@_autoremove"));
  if( mrb_undef_p(autoremove) || mrb_false_p(autoremove) || mrb_nil_p(autoremove) ){
    // nop
  }else{
    mrb_funcall(mrb, node_obj, "remove_action", 1, timer_obj);
  }
}

//
// Bi::Action
//

void mrb_action_free(mrb_state *mrb,void* p){
  BiAction *action = p;
  mrb_free(mrb,action->on_finish_callback_context);
  mrb_free(mrb,action);
}

static const struct mrb_data_type mrb_action_data_type = { "Action", mrb_action_free };

static void action_initialize(mrb_state *mrb, mrb_value* self,BiAction* action,mrb_value* option,mrb_value callback)
{
  mrb_data_init(*self,action,&mrb_action_data_type);
  // callback
  mrb_action_finish_callback_context* c = mrb_malloc(mrb,sizeof(mrb_action_finish_callback_context));
  c->mrb = mrb;
  c->action = *self;
  action->on_finish_callback_context = c;
  action->on_finish = mrb_action_finish_callback;
  mrb_iv_set(mrb, *self, mrb_intern_cstr(mrb,"@_callback"), callback );
  // autoremove
  mrb_value autoremove = option[0];
  if ( mrb_undef_p(autoremove) || mrb_nil_p(autoremove) || mrb_false_p(autoremove) ) {
    autoremove = mrb_false_value();
  }
  mrb_iv_set(mrb, *self, mrb_intern_cstr(mrb,"@_autoremove"), autoremove );
  // repeat
  mrb_value repeat = option[1];
  if( mrb_fixnum_p(repeat) ){
    bi_action_set_repeat(action, mrb_integer(repeat) );
  }
}

//
// Bi::Action::Base
//

static mrb_value mrb_action_base_initialize(mrb_state *mrb, mrb_value self)
{
  BiAction *action;
  mrb_value callback;
  mrb_sym keywords[2] = {MRB_SYM(autoremove),MRB_SYM(repeat)};
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  mrb_get_args(mrb, ":&", &kw, &callback);
  action = bi_action_init(mrb_malloc(mrb,sizeof(BiAction)));
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

//
// Bi::Action::MoveTo, MoveBy
//

static mrb_value mrb_action_move_to_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_int x,y;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "fii:&", &duration, &x, &y, &kw, &callback);
  action = (BiAction*)bi_action_move_to_init(mrb_malloc(mrb,sizeof(BiActionMove)),duration,x,y);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

static mrb_value mrb_action_move_by_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_int x,y;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "fii:&", &duration, &x, &y, &kw, &callback);
  action = (BiAction*)bi_action_move_by_init(mrb_malloc(mrb,sizeof(BiActionMove)),duration,x,y);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

//
// Bi::Action::RotateTo, RotateBy
//

static mrb_value mrb_action_rotate_to_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_float angle;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "ff:&", &duration, &angle, &kw, &callback);
  action = (BiAction*)bi_action_rotate_to_init(mrb_malloc(mrb,sizeof(BiActionRotate)),duration,angle);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

static mrb_value mrb_action_rotate_by_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_float angle;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "ff:&", &duration, &angle, &kw, &callback);
  action = (BiAction*)  bi_action_rotate_by_init(mrb_malloc(mrb,sizeof(BiActionRotate)),duration,angle);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

//
// Bi::Action::ScaleTo, ScaleBy
//

static mrb_value mrb_action_scale_to_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_float scale_x,scale_y;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "fff:&", &duration, &scale_x, &scale_y, &kw, &callback);
  action = (BiAction*)bi_action_scale_to_init(mrb_malloc(mrb,sizeof(BiActionScale)),duration,scale_x,scale_y);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

static mrb_value mrb_action_scale_by_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_float scale_x,scale_y;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "fff:&", &duration, &scale_x, &scale_y, &kw, &callback);
  action = (BiAction*)bi_action_scale_by_init(mrb_malloc(mrb,sizeof(BiActionScale)),duration,scale_x,scale_y);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}


//
// Bi::Action::FadeIn, FadeOut
//

static mrb_value mrb_action_fade_in_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "f:&", &duration, &kw, &callback);
  action = (BiAction*)bi_action_fade_in_init(mrb_malloc(mrb,sizeof(BiActionFade)),duration);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

static mrb_value mrb_action_fade_out_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_float duration;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  mrb_get_args(mrb, "f:&", &duration, &kw, &callback);
  action = (BiAction*)bi_action_fade_out_init(mrb_malloc(mrb,sizeof(BiActionFade)),duration);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

//
// Bi::Action::Sequence
//

static mrb_value mrb_action_sequence_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value actions_obj;
  mrb_value callback;
  mrb_sym keywords[2] = { MRB_SYM(autoremove), MRB_SYM(repeat) };
  mrb_value option[2];
  const mrb_kwargs kw = {2, 0, keywords, option, NULL};
  BiAction *action;
  BiAction **actions;
  mrb_get_args(mrb, "A:&", &actions_obj, &kw, &callback);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_actions_"), actions_obj);
  actions = malloc( sizeof(BiAction*) * RARRAY_LEN(actions_obj) );
  for(int i=0; i<RARRAY_LEN(actions_obj); i++ ) {
    actions[i] = DATA_PTR( RARRAY_PTR(actions_obj)[i] );
  }
  action = (BiAction*)bi_action_sequence_init(mrb_malloc(mrb,sizeof(BiActionSequence)),
                                              RARRAY_LEN(actions_obj),
                                              actions );
  free(actions);
  action_initialize(mrb,&self,action,option,callback);
  return self;
}

//
// Bi::Node
//

static mrb_value mrb_add_action(mrb_state *mrb, mrb_value self)
{
  BiNode *node;
  BiAction *action;
  mrb_value action_obj;
  mrb_get_args(mrb, "o", &action_obj);
  node = DATA_PTR(self);
  action = DATA_PTR(action_obj);
  //
  mrb_value actions = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_actions") );
  if( mrb_nil_p(actions) ){
    actions = mrb_ary_new(mrb);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_actions"), actions);
  }
  mrb_ary_push(mrb,actions,action_obj);
  //
  bi_add_action(node,action);
  bi_action_start(action);
  return action_obj;
}

static mrb_value mrb_remove_action(mrb_state *mrb, mrb_value self)
{
  mrb_value action_obj;
  mrb_get_args(mrb, "o", &action_obj);
  BiNode *node = DATA_PTR(self);
  BiAction *action = DATA_PTR(action_obj);
  //
  mrb_value actions = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_actions") );
  if( mrb_nil_p(actions) ){
    actions = mrb_ary_new(mrb);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_actions"), actions);
  }
  mrb_funcall(mrb, actions, "delete", 1, action_obj);
  //
  bi_remove_action(node,action);
  return action_obj;
}

static mrb_value mrb_remove_all_actions(mrb_state *mrb, mrb_value self)
{
  BiNode *node = DATA_PTR(self);
  //
  mrb_value actions = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_actions") );
  if( mrb_nil_p(actions) ){
    return mrb_nil_value();
  }
  //
  struct RArray *a = mrb_ary_ptr(self);
  mrb_int len = ARY_LEN(a);
  for(int i=0;i<len;i++){
    mrb_value action_obj = mrb_ary_entry(actions,i);
    if( mrb_data_p(action_obj) && DATA_TYPE(action_obj) == &mrb_action_data_type ){
      bi_remove_action(node,DATA_PTR(action_obj));
    }
  }
  mrb_ary_clear(mrb,actions);
  //
  return self;
}

//
// init
//
void mrb_init_action(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *node;
  struct RClass *action;
  struct RClass *base;
  struct RClass *move_to, *move_by;
  struct RClass *rotate_to, *rotate_by;
  struct RClass *scale_to, *scale_by;
  struct RClass *fade_in, *fade_out;
  struct RClass *sequence;

  node = mrb_class_get_under(mrb, bi, "Node");
  mrb_define_method(mrb, node, "add_action", mrb_add_action, MRB_ARGS_REQ(1)); // action
  mrb_define_method(mrb, node, "remove_action", mrb_remove_action, MRB_ARGS_REQ(1)); // action
  mrb_define_method(mrb, node, "remove_all_actions", mrb_remove_all_actions, MRB_ARGS_NONE());

  action = mrb_define_module_under(mrb,bi,"Action");

  base = mrb_define_class_under(mrb, action, "Base", mrb->object_class);
  MRB_SET_INSTANCE_TT(base, MRB_TT_DATA);
  mrb_define_method(mrb, base, "initialize", mrb_action_base_initialize,
    MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // :autoremove,&block

  move_to = mrb_define_class_under(mrb, action, "MoveTo", base);
  mrb_define_method(mrb, move_to, "initialize", mrb_action_move_to_initialize,
    MRB_ARGS_REQ(3)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,x,y,:autoremove,&block

  move_by = mrb_define_class_under(mrb, action, "MoveBy", base);
  mrb_define_method(mrb, move_by, "initialize", mrb_action_move_by_initialize,
    MRB_ARGS_REQ(3)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,x,y,:autoremove,&block

  rotate_to = mrb_define_class_under(mrb, action, "RotateTo", base);
  mrb_define_method(mrb, rotate_to, "initialize", mrb_action_rotate_to_initialize,
    MRB_ARGS_REQ(2)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,angle,:autoremove,&block

  rotate_by = mrb_define_class_under(mrb, action, "RotateBy", base);
  mrb_define_method(mrb, rotate_by, "initialize", mrb_action_rotate_by_initialize,
    MRB_ARGS_REQ(2)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,angle,:autoremove,&block

  scale_to = mrb_define_class_under(mrb, action, "ScaleTo", base);
  mrb_define_method(mrb, scale_to, "initialize", mrb_action_scale_to_initialize,
    MRB_ARGS_REQ(3)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,scale_x,scale_y,:autoremove,&block

  scale_by = mrb_define_class_under(mrb, action, "ScaleBy", base);
  mrb_define_method(mrb, scale_by, "initialize", mrb_action_scale_by_initialize,
    MRB_ARGS_REQ(3)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,scale_x,scale_y,:autoremove,&block

  fade_in = mrb_define_class_under(mrb, action, "FadeIn", base);
  mrb_define_method(mrb, fade_in, "initialize", mrb_action_fade_in_initialize,
    MRB_ARGS_REQ(1)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,:autoremove,&block

  fade_out = mrb_define_class_under(mrb, action, "FadeOut", base);
  mrb_define_method(mrb, fade_out, "initialize", mrb_action_fade_out_initialize,
    MRB_ARGS_REQ(1)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // duration,:autoremove,&block

  sequence = mrb_define_class_under(mrb, action, "Sequence", base);
  mrb_define_method(mrb, sequence, "initialize", mrb_action_sequence_initialize,
    MRB_ARGS_REQ(1)|MRB_ARGS_KEY(1,1)|MRB_ARGS_BLOCK()); // actions, :autoremove, &block
}
