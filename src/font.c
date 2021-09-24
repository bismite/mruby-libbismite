#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <bi/util.h>
#include <bi/ext/font.h>
#include <stdlib.h>
#include "bi_ext_inner_macro.h"

// Bi::Font class
static struct mrb_data_type const mrb_font_data_type = { "Font", mrb_free };

static mrb_value mrb_bi_font_read(mrb_state *mrb, mrb_value self)
{
    mrb_value texture_obj, layout_file;
    mrb_get_args(mrb, "oS", &texture_obj, &layout_file);

    BiFontAtlas *font = mrb_malloc(mrb,sizeof(BiFontAtlas));
    BiTexture* texture = DATA_PTR(texture_obj);

    bi_load_font_layout_from_file( mrb_string_value_cstr(mrb,&layout_file), font );
    font->texture = texture;

    struct RClass *bi = mrb_class_get(mrb, "Bi");
    struct RClass *klass = mrb_class_get_under(mrb,bi,"Font");

    struct RData *data = mrb_data_object_alloc(mrb,klass,font,&mrb_font_data_type);
    mrb_value val = mrb_obj_value(data);

    mrb_iv_set(mrb, val, mrb_intern_cstr(mrb,"@texture"), texture_obj);

    return val;
}

static mrb_value mrb_font_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value texture_obj, layout_data;
    mrb_get_args(mrb, "oS", &texture_obj, &layout_data);

    BiFontAtlas *font = DATA_PTR(self);
    if (font == NULL) {
      font = mrb_malloc(mrb, sizeof(BiFontAtlas));
      if (NULL == font) {
        mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
      }
    }

    // TODO: error check
    BiTexture* texture = DATA_PTR(texture_obj);

    bi_load_font_layout( RSTRING_PTR(layout_data), RSTRING_LEN(layout_data), font );
    font->texture = texture;

    DATA_PTR(self) = font;
    DATA_TYPE(self) = &mrb_font_data_type;

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@texture"), texture_obj);

    return self;
}

_GET_(BiFontAtlas,font_size,bi_mrb_fixnum_value);


void mrb_init_font(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *font;
  font = mrb_define_class_under(mrb, bi, "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(font, MRB_TT_DATA);

  mrb_define_class_method(mrb, font, "read", mrb_bi_font_read, MRB_ARGS_REQ(2) ); // texture, layout_filename

  mrb_define_method(mrb, font, "initialize", mrb_font_initialize, MRB_ARGS_REQ(2)); // texture, layout_data
  mrb_define_method(mrb, font, "size", mrb_BiFontAtlas_get_font_size, MRB_ARGS_NONE());
}
