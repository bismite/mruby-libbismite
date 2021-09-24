#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <bi/shader.h>

//
// Bi::Shader class
//

static struct mrb_data_type const mrb_shader_data_type = { "Shader", mrb_free };

static mrb_value mrb_shader_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value vertex_shader, fragment_shader;
  mrb_get_args(mrb, "SS",&vertex_shader,&fragment_shader );
  BiShader* shader = mrb_malloc(mrb,sizeof(BiShader));
  bi_shader_init(shader, mrb_string_value_cstr(mrb,&vertex_shader), mrb_string_value_cstr(mrb,&fragment_shader) );
  mrb_data_init(self, shader, &mrb_shader_data_type);
  return self;
}

void mrb_init_bi_shader(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *shader = mrb_define_class_under(mrb, bi, "Shader", mrb->object_class);
  MRB_SET_INSTANCE_TT(shader, MRB_TT_DATA);

  mrb_define_method(mrb, shader, "initialize", mrb_shader_initialize, MRB_ARGS_REQ(2)); // VertexShader,FragmentShader
}
