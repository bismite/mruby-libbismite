#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/texture.h>
#include "bi_core_inner_macro.h"

// Bi::Texture class
static struct mrb_data_type const mrb_texture_data_type = { "Texture", mrb_free };

static mrb_value mrb_texture_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_int x,y,w,h;
    mrb_get_args(mrb, "oiiii", &obj, &x,&y,&w,&h );

    BiTexture* texture = mrb_malloc(mrb,sizeof(BiTexture));

    // XXX: check type
    texture->texture_image = DATA_PTR(obj);
    bi_set_texture_boundary(texture,x,y,w,h);

    mrb_data_init(self, texture, &mrb_texture_data_type);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@texture_image"), obj);

    return self;
}

static mrb_value mrb_texture_set_texture_image(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiTexture* texture = DATA_PTR(self);
    BiTextureImage* img = DATA_PTR(obj);

    texture->texture_image = img;

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@texture_image"), obj);

    return self;
}

//
// define accessors
//
_GET_(BiTexture,w,bi_mrb_fixnum_value);
_SET_(BiTexture,w,mrb_int,i);
_GET_(BiTexture,h,bi_mrb_fixnum_value);
_SET_(BiTexture,h,mrb_int,i);

void mrb_init_bi_texture(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *texture;
  texture = mrb_define_class_under(mrb, bi, "Texture", mrb->object_class);
  MRB_SET_INSTANCE_TT(texture, MRB_TT_DATA);

  mrb_define_method(mrb, texture, "initialize", mrb_texture_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, texture, "set_texture_image", mrb_texture_set_texture_image, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, texture, "w", mrb_BiTexture_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, texture, "w=",mrb_BiTexture_set_w, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, texture, "h", mrb_BiTexture_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, texture, "h=",mrb_BiTexture_set_h, MRB_ARGS_REQ(1));
}
