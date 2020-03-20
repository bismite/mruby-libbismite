#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/timer.h>

typedef struct {
  mrb_state *mrb;
  mrb_value node;
  mrb_value timer;
} BiTimerContainer;

//
// Timer callback
//

static bool _timer_callback_(double now,BiTimer* timer)
{
  BiTimerContainer *container = timer->userdata;
  mrb_state *mrb = container->mrb;
  mrb_value block = mrb_iv_get(mrb, container->timer, mrb_intern_cstr(mrb,"@_callback_") );
  mrb_value now_value = mrb_float_value(mrb,now);
  mrb_value argv[3] = { container->node, now_value, container->timer };
  return mrb_bool( mrb_yield_argv(mrb, block, 3, argv) );
}

//
// Bi::Timer class
//

static void mrb_timer_free(mrb_state *mrb, void *p)
{
  BiTimer *timer = p;
  BiTimerContainer *container = timer->userdata;
  mrb_free(mrb,container);
  mrb_free(mrb,p);
}

static struct mrb_data_type const mrb_timer_data_type = { "Timer", mrb_timer_free };

static mrb_value mrb_timer_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value node_obj;
    mrb_float interval;
    mrb_int repeat;
    mrb_value callback;
    BiNode* node;
    BiTimer* timer;
    BiTimerContainer* container;

    mrb_get_args(mrb, "ofi&", &node_obj, &interval, &repeat, &callback );

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_callback_"), callback);

    node = DATA_PTR(node_obj);
    timer = mrb_malloc(mrb,sizeof(BiTimer));
    container = mrb_malloc(mrb,sizeof(BiTimerContainer));
    container->mrb = mrb;
    container->timer = self;
    container->node = node_obj;
    bi_timer_init(timer,_timer_callback_,interval,repeat, container );
    mrb_data_init(self, timer, &mrb_timer_data_type);

    return self;
}

void mrb_init_bi_timer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *timer;
  timer = mrb_define_class_under(mrb, bi, "Timer", mrb->object_class);
  MRB_SET_INSTANCE_TT(timer, MRB_TT_DATA);

  mrb_define_method(mrb, timer, "initialize", mrb_timer_initialize, MRB_ARGS_REQ(4));
}
