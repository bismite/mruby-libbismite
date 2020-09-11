#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <bi/context.h>

//
// Bi::Profile
//

static struct mrb_data_type const mrb_profile_data_type = { "Profile", mrb_free };

static mrb_value mrb_bi_profile_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value context;
  mrb_get_args(mrb, "o", &context );
  BiContext* c = DATA_PTR(context);
  mrb_data_init(self, &(c->profile), &mrb_profile_data_type);
  return self;
}

static mrb_value mrb_bi_profile_fps(mrb_state *mrb, mrb_value self)
{
    BiProfile* p = DATA_PTR(self);
    return mrb_float_value(mrb, p->stats.fps);
}

static mrb_value mrb_bi_profile_time_spent_on_rendering(mrb_state *mrb, mrb_value self)
{
    BiProfile* p = DATA_PTR(self);
    return mrb_fixnum_value(p->stats.time_spent_on_rendering);
}
static mrb_value mrb_bi_profile_time_spent_on_callback(mrb_state *mrb, mrb_value self)
{
    BiProfile* p = DATA_PTR(self);
    return mrb_fixnum_value(p->stats.time_spent_on_callback);
}
static mrb_value mrb_bi_profile_matrix_updated(mrb_state *mrb, mrb_value self)
{
    BiProfile* p = DATA_PTR(self);
    return mrb_float_value(mrb,p->stats.matrix_updated);
}


static mrb_value mrb_bi_profile_rendering_nodes_queue_size(mrb_state *mrb, mrb_value self)
{
    BiProfile* p = DATA_PTR(self);
    return mrb_fixnum_value(p->rendering_nodes_queue_size);
}
static mrb_value mrb_bi_profile_callback_planned_nodes_size(mrb_state *mrb, mrb_value self)
{
    BiProfile* p = DATA_PTR(self);
    return mrb_fixnum_value(p->callback_planned_nodes_size);
}

void mrb_init_bi_profile(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *profile;
  profile = mrb_define_class_under(mrb, bi, "Profile", mrb->object_class);
  MRB_SET_INSTANCE_TT(profile, MRB_TT_DATA);

  mrb_define_method(mrb, profile, "initialize", mrb_bi_profile_initialize, MRB_ARGS_REQ(1)); // context
  mrb_define_method(mrb, profile, "fps", mrb_bi_profile_fps, MRB_ARGS_NONE());
  mrb_define_method(mrb, profile, "time_spent_on_rendering", mrb_bi_profile_time_spent_on_rendering, MRB_ARGS_NONE());
  mrb_define_method(mrb, profile, "time_spent_on_callback", mrb_bi_profile_time_spent_on_callback, MRB_ARGS_NONE());
  mrb_define_method(mrb, profile, "matrix_updated", mrb_bi_profile_matrix_updated, MRB_ARGS_NONE());

  mrb_define_method(mrb, profile, "rendering_nodes_queue_size", mrb_bi_profile_rendering_nodes_queue_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, profile, "callback_planned_nodes_size", mrb_bi_profile_callback_planned_nodes_size, MRB_ARGS_NONE());
}
