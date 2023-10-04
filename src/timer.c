#include <stdlib.h>
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/context.h>
#include <bi/timer.h>

//
// Bi::Timer class
//
static struct mrb_data_type const mrb_timer_data_type = { "Timer", mrb_free };

//
// Timer callback
//
static void _timer_callback_(BiContext* context,BiTimer* timer,double dt)
{
  mrb_state *mrb = context->userdata;
  mrb_value timer_obj = mrb_obj_value(timer->userdata);
  mrb_value callback = mrb_iv_get(mrb, timer_obj, mrb_intern_cstr(mrb,"@callback") );
  mrb_value argv[2] = { timer_obj, mrb_float_value(mrb,dt) };
  mrb_yield_argv(mrb, callback, 2, argv);
  if(timer->count == 0){
    mrb_value node_obj = mrb_iv_get(mrb, timer_obj, mrb_intern_cstr(mrb,"@node") );
    mrb_funcall(mrb, node_obj, "remove_timer", 1, timer_obj);
  }
}

static mrb_value mrb_timer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int interval,count;
  mrb_value callback;
  BiTimer* timer;
  mrb_get_args(mrb, "ii&!", &interval, &count, &callback );
  timer = bi_timer_init(mrb_malloc(mrb,sizeof(BiTimer)),_timer_callback_,interval,count, NULL );
  mrb_data_init(self, timer, &mrb_timer_data_type);
  timer->userdata = mrb_ptr(self);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@callback"), callback);
  return self;
}

//
// Module
//
static mrb_value mrb_bi_add_timer(mrb_state *mrb, mrb_value self)
{
  mrb_value timer_obj;
  mrb_get_args(mrb, "o", &timer_obj );
  BiNodeBase* node = DATA_PTR(self);
  BiTimers *timers = &node->timers;
  BiTimer* timer = DATA_PTR(timer_obj);
  bi_timers_add(timers,timer);
  //
  mrb_value timers_obj = mrb_iv_get(mrb,self,mrb_intern_cstr(mrb,"@_timers"));
  if(mrb_nil_p(timers_obj)){
    timers_obj = mrb_ary_new(mrb);
    mrb_iv_set(mrb,self,mrb_intern_cstr(mrb,"@_timers"),timers_obj);
  }
  mrb_ary_push(mrb,timers_obj,timer_obj);
  //
  mrb_iv_set(mrb,timer_obj,mrb_intern_cstr(mrb,"@node"),self);
  return timer_obj;
}

static mrb_value mrb_bi_remove_timer(mrb_state *mrb, mrb_value self)
{
  mrb_value timer_obj;
  mrb_get_args(mrb, "o", &timer_obj );
  BiNodeBase* node = DATA_PTR(self);
  BiTimers *timers = &node->timers;
  BiTimer* timer = DATA_PTR(timer_obj);
  bi_timers_remove(timers,timer);
  //
  mrb_value timers_obj = mrb_iv_get(mrb,self,mrb_intern_cstr(mrb,"@_timers"));
  if(!mrb_nil_p(timers_obj)){
    mrb_funcall(mrb, timers_obj, "delete", 1, timer_obj);
  }
  //
  mrb_iv_set(mrb,timer_obj,mrb_intern_cstr(mrb,"@node"),mrb_nil_value());
  return timer_obj;
}

//
//
//
void mrb_init_bi_timer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *timer = mrb_define_class_under(mrb, bi, "Timer", mrb->object_class);
  MRB_SET_INSTANCE_TT(timer, MRB_TT_DATA);
  mrb_define_method(mrb, timer, "initialize", mrb_timer_initialize, MRB_ARGS_REQ(3)); // interval, count, callback

  struct RClass *runner = mrb_define_module_under(mrb, bi, "TimerRunner");
  mrb_define_module_function(mrb, runner, "add_timer", mrb_bi_add_timer, MRB_ARGS_REQ(1)); // Timer
  mrb_define_module_function(mrb, runner, "remove_timer", mrb_bi_remove_timer, MRB_ARGS_REQ(1)); // Timer
}
