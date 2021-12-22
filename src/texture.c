#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <bi/texture.h>

// Bi::Texture class
static struct mrb_data_type const mrb_texture_data_type = { "Texture", mrb_free };

//
// extern methods
//

mrb_value create_bi_texture(mrb_state *mrb, BiTexture* texture)
{
  struct RClass *klass = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Texture");
  struct RData *data = mrb_data_object_alloc(mrb,klass,texture,&mrb_texture_data_type);
  return mrb_obj_value(data);
}

mrb_value create_bi_texture_from_memory(mrb_state *mrb, void* buffer, int size, bool antialias)
{
  BiTexture *texture = mrb_malloc(mrb,sizeof(BiTexture));
  bi_texture_init_with_file(texture,buffer,size,antialias);
  return create_bi_texture(mrb,texture);
}

//
// instance methods
//

static mrb_value mrb_texture_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value image_name;
  mrb_bool straight_alpha = false;
  mrb_get_args(mrb, "S|b", &image_name, &straight_alpha );
  BiTexture *texture = mrb_malloc(mrb,sizeof(BiTexture));
  bi_texture_init_with_filename(texture,mrb_string_value_cstr(mrb,&image_name), straight_alpha);
  mrb_data_init(self, texture, &mrb_texture_data_type);
  return self;
}

static mrb_value mrb_texture_w(mrb_state *mrb, mrb_value self)
{
  BiTexture *texture = DATA_PTR(self);
  return mrb_fixnum_value(texture->w);
}

static mrb_value mrb_texture_h(mrb_state *mrb, mrb_value self)
{
  BiTexture *texture = DATA_PTR(self);
  return mrb_fixnum_value(texture->h);
}

void mrb_init_bi_texture(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *texture;
  texture = mrb_define_class_under(mrb, bi, "Texture", mrb->object_class);
  MRB_SET_INSTANCE_TT(texture, MRB_TT_DATA);

  mrb_define_method(mrb, texture, "initialize", mrb_texture_initialize, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(1)); // filename,straight-alpha
  mrb_define_method(mrb, texture, "w", mrb_texture_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, texture, "h", mrb_texture_h, MRB_ARGS_NONE());
}
