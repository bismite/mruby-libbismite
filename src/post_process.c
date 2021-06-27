#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/layer.h>

//
// Bi::PostProcess class
//
static struct mrb_data_type const mrb_post_process_data_type = { "PostProcess", mrb_free };

static mrb_value mrb_post_process_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiPostProcess *post_process = mrb_malloc(mrb,sizeof(BiPostProcess));
  if (NULL == post_process) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  bi_post_process_init(post_process);
  mrb_data_init(self, post_process, &mrb_post_process_data_type);
  // shader
  BiShader* _obj = DATA_PTR(obj);
  post_process->shader = _obj;
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@shader"), obj);
  return self;
}

static mrb_value mrb_post_process_set_shader(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiPostProcess* _self = DATA_PTR(self);
  BiShader* _obj = DATA_PTR(obj);
  _self->shader = _obj;
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@shader"), obj);
  return self;
}

//
void mrb_init_bi_post_process(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *post_process = mrb_define_class_under(mrb, bi, "PostProcess", mrb->object_class);
  MRB_SET_INSTANCE_TT(post_process, MRB_TT_DATA);

  mrb_define_method(mrb, post_process, "initialize", mrb_post_process_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, post_process, "shader=", mrb_post_process_set_shader, MRB_ARGS_REQ(1));
}
