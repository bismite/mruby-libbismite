#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <bi/texture.h>

// Bi::TextureImage class
static struct mrb_data_type const mrb_texture_image_data_type = { "TextureImage", mrb_free };

static mrb_value mrb_texture_image_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value image_name;
    mrb_bool antialiase;
    mrb_get_args(mrb, "Sb", &image_name, &antialiase );

    BiTextureImage *img = mrb_malloc(mrb,sizeof(BiTextureImage));

    if( bi_load_texture( mrb_string_value_cstr(mrb,&image_name), img, antialiase ) ) {
      mrb_data_init(self, img, &mrb_texture_image_data_type);
    }else{
      // XXX: raise!
      mrb_raise(mrb, E_RUNTIME_ERROR, "texture load error.");
    }

    return self;
}

static mrb_value mrb_texture_image_w(mrb_state *mrb, mrb_value self)
{
    BiTextureImage *img = DATA_PTR(self);
    return mrb_fixnum_value(img->w);
}

static mrb_value mrb_texture_image_h(mrb_state *mrb, mrb_value self)
{
    BiTextureImage *img = DATA_PTR(self);
    return mrb_fixnum_value(img->h);
}

void mrb_init_texture_image(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *img;
  img = mrb_define_class_under(mrb, bi, "TextureImage", mrb->object_class);
  MRB_SET_INSTANCE_TT(img, MRB_TT_DATA);

  mrb_define_method(mrb, img, "initialize", mrb_texture_image_initialize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, img, "w", mrb_texture_image_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, img, "h", mrb_texture_image_h, MRB_ARGS_NONE());
}
