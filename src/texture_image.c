#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <bi/texture.h>

// Bi::TextureImage class
static struct mrb_data_type const mrb_texture_image_data_type = { "TextureImage", mrb_free };

//
// extern methods
//

mrb_value create_bi_texture_image(mrb_state *mrb, BiTextureImage* texture_image)
{
    BiTextureImage *img = mrb_malloc(mrb,sizeof(BiTextureImage));
    *img = *texture_image;

    struct RClass *bi = mrb_class_get(mrb, "Bi");
    struct RClass *klass = mrb_class_get_under(mrb,bi,"TextureImage");

    struct RData *data = mrb_data_object_alloc(mrb,klass,img,&mrb_texture_image_data_type);
    mrb_value tmp = mrb_obj_value(data);

    return tmp;
}

mrb_value create_bi_texture_image_from_buffer(mrb_state *mrb, void* buffer, int size, bool antialias)
{
    BiTextureImage img;

    if( ! bi_create_texture( buffer, size, &img, antialias ) ) {
      // XXX: raise!
      mrb_raise(mrb, E_RUNTIME_ERROR, "texture load error.");
    }

    return create_bi_texture_image(mrb,&img);
}

//
// instance methods
//

static mrb_value mrb_texture_image_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value image_name;
    mrb_bool antialias;
    mrb_get_args(mrb, "Sb", &image_name, &antialias );

    BiTextureImage *img = mrb_malloc(mrb,sizeof(BiTextureImage));

    if( bi_load_texture( mrb_string_value_cstr(mrb,&image_name), img, antialias ) ) {
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
