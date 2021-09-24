#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/timer.h>

//
// Timer callback
//

static bool _timer_callback_(BiContext *context,BiTimer* timer)
{
  mrb_value self = mrb_obj_value(timer->userdata);
  mrb_state *mrb = context->userdata;
  mrb_value block = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@callback") );
  mrb_value now_value = mrb_fixnum_value(context->frame_start_at);
  mrb_value argv[2] = { self, now_value };
  return mrb_bool( mrb_yield_argv(mrb, block, 2, argv) );
}

//
// Bi::Timer class
//

static struct mrb_data_type const mrb_timer_data_type = { "Timer", mrb_free };

static mrb_value mrb_timer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value owner;
  mrb_int interval;
  mrb_int repeat;
  mrb_value callback;
  BiTimer* timer;

  mrb_get_args(mrb, "oii&", &owner, &interval, &repeat, &callback );

  timer = mrb_malloc(mrb,sizeof(BiTimer));
  mrb_data_init(self, timer, &mrb_timer_data_type);
  bi_timer_init(timer,_timer_callback_,interval,repeat, mrb_ptr(self) );

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@callback"), callback);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@owner"), owner);
  return self;
}

void mrb_init_bi_timer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *timer;
  timer = mrb_define_class_under(mrb, bi, "Timer", mrb->object_class);
  MRB_SET_INSTANCE_TT(timer, MRB_TT_DATA);

  mrb_define_method(mrb, timer, "initialize", mrb_timer_initialize, MRB_ARGS_REQ(4));
}
