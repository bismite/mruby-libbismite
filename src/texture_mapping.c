#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/texture.h>
#include "bi_core_inner_macro.h"

// Bi::Texture class
static struct mrb_data_type const mrb_texture_mapping_data_type = { "TextureMapping", mrb_free };

static mrb_value mrb_texture_mapping_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_int x,y,w,h;
    mrb_get_args(mrb, "oiiii", &obj, &x,&y,&w,&h );

    BiTextureMapping* mapping = mrb_malloc(mrb,sizeof(BiTextureMapping));
    bi_texture_mapping_init(mapping);

    // XXX: check type
    mapping->texture = DATA_PTR(obj);
    bi_texture_mapping_set_bound(mapping,x,y,w,h);

    mrb_data_init(self, mapping, &mrb_texture_mapping_data_type);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@texture"), obj);

    return self;
}

static mrb_value mrb_texture_set_texture(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiTextureMapping* mapping = DATA_PTR(self);
    BiTexture* texture = DATA_PTR(obj);

    mapping->texture = texture;

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@texture"), obj);

    return self;
}

//
// define accessors
//
_GET_(BiTextureMapping,w,bi_mrb_fixnum_value);
_SET_(BiTextureMapping,w,mrb_int,i);
_GET_(BiTextureMapping,h,bi_mrb_fixnum_value);
_SET_(BiTextureMapping,h,mrb_int,i);

void mrb_init_bi_texture_mapping(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *mapping;
  mapping = mrb_define_class_under(mrb, bi, "TextureMapping", mrb->object_class);
  MRB_SET_INSTANCE_TT(mapping, MRB_TT_DATA);

  mrb_define_method(mrb, mapping, "initialize", mrb_texture_mapping_initialize, MRB_ARGS_REQ(5)); // texture,x,y,w,h
  mrb_define_method(mrb, mapping, "set_texture", mrb_texture_set_texture, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, mapping, "w", mrb_BiTextureMapping_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, mapping, "w=",mrb_BiTextureMapping_set_w, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mapping, "h", mrb_BiTextureMapping_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, mapping, "h=",mrb_BiTextureMapping_set_h, MRB_ARGS_REQ(1));
}
