#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/context.h>
#include <bi/layer.h>
#include "bi_core_inner_macro.h"

//
// Bi::Layer class
//

static struct mrb_data_type const mrb_layer_data_type = { "Layer", mrb_free };

static mrb_value mrb_layer_initialize(mrb_state *mrb, mrb_value self)
{
    BiLayer* layer;

    layer = mrb_malloc(mrb,sizeof(BiLayer));
    if (NULL == layer) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }

    bi_layer_init(layer);
    mrb_data_init(self, layer, &mrb_layer_data_type);

    return self;
}

//
// define accessors
//
_GET_(BiLayer,camera_x,bi_mrb_fixnum_value);
_SET_(BiLayer,camera_x,mrb_int,i);
_GET_(BiLayer,camera_y,bi_mrb_fixnum_value);
_SET_(BiLayer,camera_y,mrb_int,i);
_GET_(BiLayer,projection_centering,bi_mrb_bool_value);
_SET_(BiLayer,projection_centering,mrb_bool,b);
_GET_(BiLayer,blend_src,bi_mrb_fixnum_value);
_SET_(BiLayer,blend_src,mrb_int,i);
_GET_(BiLayer,blend_dst,bi_mrb_fixnum_value);
_SET_(BiLayer,blend_dst,mrb_int,i);
_GET_(BiLayer,z_order,bi_mrb_fixnum_value);
_SET_(BiLayer,z_order,mrb_int,i);

static mrb_value mrb_BiLayer_set_root(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiLayer* layer = DATA_PTR(self);
    BiNode* node = DATA_PTR(obj);

    layer->root = node;
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@root"), obj);

    return self;
}

static mrb_value mrb_BiLayer_get_root(mrb_state *mrb, mrb_value self)
{
    return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@root"));
}

static mrb_value mrb_BiLayer_set_shader(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiLayer* layer = DATA_PTR(self);
    struct RClass *bi = mrb_class_get(mrb,"Bi");
    struct RClass *shader_class = mrb_class_get_under(mrb,bi,"Shader");
    if( mrb_obj_is_kind_of(mrb, obj, shader_class) ) {
      BiShader* shader = DATA_PTR(obj);
      layer->shader = shader;
      mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@shader"), obj);
    }else{
      layer->shader = NULL;
      mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@shader"), mrb_nil_value() );
    }

    return self;
}

static mrb_value mrb_BiLayer_get_shader(mrb_state *mrb, mrb_value self)
{
    return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@shader"));
}

static mrb_value mrb_BiLayer_set_optional_shader_attributes(mrb_state *mrb, mrb_value self)
{
    mrb_int index;
    mrb_float value;
    mrb_get_args(mrb, "if", &index, &value );
    BiLayer* layer = DATA_PTR(self);
    if( 0 <= index && index < 4 ) {
      layer->optional_shader_attributes[index] = value;
    }
    return self;
}

static mrb_value mrb_BiLayer_set_texture(mrb_state *mrb, mrb_value self)
{
    mrb_int index;
    mrb_value texture_obj;
    mrb_get_args(mrb, "io", &index, &texture_obj );

    BiLayer* layer = DATA_PTR(self);
    BiTexture* texture = DATA_PTR(texture_obj);

    if( 0 <= index && index < 8 ) {
      layer->textures[index] = texture;
    }

    return self;
}

//
void mrb_init_bi_layer(mrb_state *mrb,struct RClass *bi)
{
  struct RClass *layer;
  layer = mrb_define_class_under(mrb, bi, "Layer", mrb->object_class);
  MRB_SET_INSTANCE_TT(layer, MRB_TT_DATA);

  mrb_define_method(mrb, layer, "initialize", mrb_layer_initialize, MRB_ARGS_NONE());

  mrb_define_method(mrb, layer, "camera_x", mrb_BiLayer_get_camera_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "camera_x=",mrb_BiLayer_set_camera_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "camera_y", mrb_BiLayer_get_camera_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "camera_y=",mrb_BiLayer_set_camera_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "projection_centering", mrb_BiLayer_get_projection_centering, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "projection_centering=", mrb_BiLayer_set_projection_centering, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "blend_src", mrb_BiLayer_get_blend_src, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "blend_src=",mrb_BiLayer_set_blend_src, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "blend_dst", mrb_BiLayer_get_blend_dst, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "blend_dst=",mrb_BiLayer_set_blend_dst, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "z_order", mrb_BiLayer_get_z_order, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "z_order=",mrb_BiLayer_set_z_order, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "root", mrb_BiLayer_get_root, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "root=",mrb_BiLayer_set_root, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "shader", mrb_BiLayer_get_shader, MRB_ARGS_NONE());
  mrb_define_method(mrb, layer, "shader=",mrb_BiLayer_set_shader, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, layer, "set_optional_shader_attributes",mrb_BiLayer_set_optional_shader_attributes, MRB_ARGS_REQ(2)); // index,value

  mrb_define_method(mrb, layer, "set_texture",mrb_BiLayer_set_texture, MRB_ARGS_REQ(2));

  // blend functions
  // source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml
  mrb_define_const(mrb, layer, "GL_ZERO", mrb_fixnum_value(GL_ZERO));
  mrb_define_const(mrb, layer, "GL_ONE", mrb_fixnum_value(GL_ONE));
  mrb_define_const(mrb, layer, "GL_SRC_COLOR", mrb_fixnum_value(GL_SRC_COLOR));
  mrb_define_const(mrb, layer, "GL_ONE_MINUS_SRC_COLOR", mrb_fixnum_value(GL_ONE_MINUS_SRC_COLOR));
  mrb_define_const(mrb, layer, "GL_DST_COLOR", mrb_fixnum_value(GL_DST_COLOR));
  mrb_define_const(mrb, layer, "GL_ONE_MINUS_DST_COLOR", mrb_fixnum_value(GL_ONE_MINUS_DST_COLOR));
  mrb_define_const(mrb, layer, "GL_SRC_ALPHA", mrb_fixnum_value(GL_SRC_ALPHA));
  mrb_define_const(mrb, layer, "GL_ONE_MINUS_SRC_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_SRC_ALPHA));
  mrb_define_const(mrb, layer, "GL_DST_ALPHA", mrb_fixnum_value(GL_DST_ALPHA));
  mrb_define_const(mrb, layer, "GL_ONE_MINUS_DST_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_DST_ALPHA));
  mrb_define_const(mrb, layer, "GL_CONSTANT_COLOR", mrb_fixnum_value(GL_CONSTANT_COLOR));
  mrb_define_const(mrb, layer, "GL_ONE_MINUS_CONSTANT_COLOR", mrb_fixnum_value(GL_ONE_MINUS_CONSTANT_COLOR));
  mrb_define_const(mrb, layer, "GL_CONSTANT_ALPHA", mrb_fixnum_value(GL_CONSTANT_ALPHA));
  mrb_define_const(mrb, layer, "GL_ONE_MINUS_CONSTANT_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_CONSTANT_ALPHA));
  mrb_define_const(mrb, layer, "GL_SRC_ALPHA_SATURATE", mrb_fixnum_value(GL_SRC_ALPHA_SATURATE));
  // not supported in OpenGL ES
  // mrb_define_const(mrb, layer, "GL_SRC1_COLOR", mrb_fixnum_value(GL_SRC1_COLOR));
  // mrb_define_const(mrb, layer, "GL_ONE_MINUS_SRC1_COLOR", mrb_fixnum_value(GL_ONE_MINUS_SRC1_COLOR));
  // mrb_define_const(mrb, layer, "GL_SRC1_ALPHA", mrb_fixnum_value(GL_SRC1_ALPHA));
  // mrb_define_const(mrb, layer, "GL_ONE_MINUS_SRC1_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_SRC1_ALPHA));
}
