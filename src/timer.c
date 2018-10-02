#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/timer.h>

//
// Timer callback
//

static bool _timer_callback_(BiContext* context,BiNode* node,double now,BiTimer* timer)
{
  mrb_state *mrb = context->userdata;

  mrb_value target_node = mrb_obj_value(node->userdata);
  mrb_value timer_obj = mrb_obj_value(timer->userdata);
  mrb_value block = mrb_iv_get(mrb, timer_obj, mrb_intern_cstr(mrb,"@_callback_") );

  mrb_value now_value = mrb_float_value(mrb,now);
  mrb_value argv[3] = { target_node, now_value, timer_obj };
  return mrb_bool( mrb_yield_argv(mrb, block, 3, argv) );
}

//
// Bi::Timer class
//

static struct mrb_data_type const mrb_timer_data_type = { "Timer", mrb_free };

static mrb_value mrb_timer_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value node_obj;
    mrb_float interval;
    mrb_int repeat;
    mrb_value callback;
    BiNode* node;
    BiTimer* timer;

    mrb_get_args(mrb, "ofi&", &node_obj, &interval, &repeat, &callback );

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_callback_"), callback);

    node = DATA_PTR(node_obj);
    node->userdata = mrb_ptr(node_obj);

    timer = mrb_malloc(mrb,sizeof(BiTimer));
    bi_timer_init(timer,node,_timer_callback_,interval,repeat,mrb_ptr(self));

    mrb_data_init(self, timer, &mrb_timer_data_type);

    return self;
}

void mrb_init_timer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *timer;
  timer = mrb_define_class_under(mrb, bi, "Timer", mrb->object_class);
  MRB_SET_INSTANCE_TT(timer, MRB_TT_DATA);

  mrb_define_method(mrb, timer, "initialize", mrb_timer_initialize, MRB_ARGS_REQ(4));
}
