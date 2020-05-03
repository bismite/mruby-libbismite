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
    BiTexture *new_texture = mrb_malloc(mrb,sizeof(BiTexture));
    *new_texture = *texture;

    struct RClass *bi = mrb_class_get(mrb, "Bi");
    struct RClass *klass = mrb_class_get_under(mrb,bi,"Texture");

    struct RData *data = mrb_data_object_alloc(mrb,klass,new_texture,&mrb_texture_data_type);
    mrb_value tmp = mrb_obj_value(data);

    return tmp;
}

mrb_value create_bi_texture_from_buffer(mrb_state *mrb, void* buffer, int size, bool antialias)
{
    BiTexture texture;

    if( ! bi_create_texture( buffer, size, &texture, antialias ) ) {
      // XXX: raise!
      mrb_raise(mrb, E_RUNTIME_ERROR, "texture load error.");
    }

    return create_bi_texture(mrb,&texture);
}

//
// instance methods
//

static mrb_value mrb_texture_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value image_name;
    mrb_bool antialias;
    mrb_get_args(mrb, "Sb", &image_name, &antialias );

    BiTexture *texture = mrb_malloc(mrb,sizeof(BiTexture));

    if( bi_load_texture( mrb_string_value_cstr(mrb,&image_name), texture, antialias ) ) {
      mrb_data_init(self, texture, &mrb_texture_data_type);
    }else{
      // XXX: raise!
      mrb_raise(mrb, E_RUNTIME_ERROR, "texture load error.");
    }

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

  mrb_define_method(mrb, texture, "initialize", mrb_texture_initialize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, texture, "w", mrb_texture_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, texture, "h", mrb_texture_h, MRB_ARGS_NONE());
}
