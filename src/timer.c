#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/timer.h>

//
// Timer callback
//
static void _timer_callback_(BiContext *context,BiTimer* timer,double dt)
{
  mrb_state *mrb = context->userdata;
  mrb_value timer_obj = mrb_obj_value(timer->userdata);
  mrb_value node_obj = mrb_obj_value(timer->node->userdata);
  mrb_value callback = mrb_iv_get(mrb, timer_obj, mrb_intern_cstr(mrb,"@callback") );
  mrb_value argv[2] = { node_obj, mrb_float_value(mrb,dt) };
  mrb_yield_argv(mrb, callback, 2, argv);
  if(timer->count == 0){
    mrb_funcall(mrb, node_obj, "remove_timer", 1, timer_obj);
  }
}

//
// Bi::Timer class
//

static struct mrb_data_type const mrb_timer_data_type = { "Timer", mrb_free };

static mrb_value mrb_timer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int interval,count;
  mrb_value callback;
  BiTimer* timer;
  mrb_get_args(mrb, "iio", &interval, &count, &callback );
  timer = mrb_malloc(mrb,sizeof(BiTimer));
  mrb_data_init(self, timer, &mrb_timer_data_type);
  bi_timer_init(timer,_timer_callback_,interval,count, mrb_ptr(self) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@callback"), callback);
  return self;
}

void mrb_init_bi_timer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *timer = mrb_define_class_under(mrb, bi, "Timer", mrb->object_class);
  MRB_SET_INSTANCE_TT(timer, MRB_TT_DATA);
  mrb_define_method(mrb, timer, "initialize", mrb_timer_initialize, MRB_ARGS_REQ(3));
}
