#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <stdbool.h>
#include <bi/node_base.h>

static void add_timer(mrb_state* mrb,mrb_value self,mrb_value obj)
{
  BiRawNode* node = DATA_PTR(self);
  BiTimer* timer = DATA_PTR(obj);
  bi_raw_node_add_timer(node,timer);
  mrb_value timers = mrb_iv_get(mrb,self,mrb_intern_cstr(mrb,"@_timers"));
  if(mrb_nil_p(timers)){
    timers = mrb_ary_new(mrb);
    mrb_iv_set(mrb,self,mrb_intern_cstr(mrb,"@_timers"),timers);
  }
  mrb_ary_push(mrb,timers,obj);
}

static mrb_value mrb_bi_add_timer(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  add_timer(mrb,self,obj);
  return obj;
}

static mrb_value mrb_bi_create_timer(mrb_state *mrb, mrb_value self)
{
  mrb_value interval,count,block;
  mrb_get_args(mrb, "oo&!", &interval,&count,&block );
  struct RClass *bi = mrb_class_get(mrb, "Bi");
  struct RClass *timer = mrb_class_get_under(mrb, bi, "Timer");
  mrb_value argv[3] = {interval,count,block};
  mrb_value timer_obj = mrb_obj_new(mrb,timer,3,argv);
  add_timer(mrb,self,timer_obj);
  return timer_obj;
}

static mrb_value mrb_bi_remove_timer(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiRawNode* node = DATA_PTR(self);
  BiTimer* timer = DATA_PTR(obj);
  bi_raw_node_remove_timer(node,timer);
  mrb_value timers = mrb_iv_get(mrb,self,mrb_intern_cstr(mrb,"@_timers"));
  if(!mrb_nil_p(timers))
    mrb_funcall(mrb, timers, "delete", 1, obj);
  return obj;
}

void mrb_init_bi_timer_runner(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *t = mrb_define_module_under(mrb, bi, "TimerRunner");
  mrb_define_module_function(mrb, t, "add_timer", mrb_bi_add_timer, MRB_ARGS_REQ(1)); // Timer
  mrb_define_module_function(mrb, t, "create_timer", mrb_bi_create_timer, MRB_ARGS_REQ(3)); // interval,count,callback
  mrb_define_module_function(mrb, t, "remove_timer", mrb_bi_remove_timer, MRB_ARGS_REQ(1)); // Timer
}
