#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/context.h>
#include <bi/node.h>
#include <bi/util.h>
#include <stdlib.h>
#include "_inner_macro.h"
#include "_node_base.h"

// Color class (static)
static struct mrb_data_type const mrb_bi_color_data_type = { "Color", NULL };

// Bi::Node class
static void bi_node_free(mrb_state *mrb, void *p)
{
  if (NULL != p) {
    bi_node_base_deinit(p);
    mrb_free(mrb, p);
  }
}
static struct mrb_data_type const mrb_node_data_type = { "Node", bi_node_free };

//
// callback function
//

static bool on_click(BiContext* context, BiNode* node, int x, int y, int button, bool pressed)
{
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_click_callback_") );
  mrb_value _x = mrb_fixnum_value(x);
  mrb_value _y = mrb_fixnum_value(y);
  mrb_value _button = mrb_fixnum_value(button);
  mrb_value _pressed = mrb_bool_value(pressed);
  mrb_value argv[5] = { self, _x, _y, _button, _pressed };

  if( mrb_symbol_p(obj) ){
    return mrb_bool( mrb_funcall_argv(mrb,self,mrb_symbol(obj),5,argv) );
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    return mrb_bool( mrb_yield_argv(mrb, obj, 5, argv) );
  }
  return false;
}

static bool on_move_cursor(BiContext* context, BiNode* node, int x, int y)
{
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_move_cursor_callback_") );
  mrb_value _x = mrb_fixnum_value(x);
  mrb_value _y = mrb_fixnum_value(y);
  mrb_value argv[3] = { self, _x, _y };

  if( mrb_symbol_p(obj) ){
    return mrb_bool( mrb_funcall_argv(mrb,self,mrb_symbol(obj),3,argv) );
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    return mrb_bool( mrb_yield_argv(mrb, obj, 3, argv) );
  }
  return false;
}

static bool on_keyinput(BiContext* context, BiNode* node, uint16_t scancode, uint32_t keycode, uint16_t mod, bool pressed)
{
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_keyinput_callback_") );
  mrb_value _scancode = mrb_fixnum_value(scancode);
  mrb_value _keycode = mrb_fixnum_value(keycode);
  mrb_value _mod = mrb_fixnum_value(mod);
  mrb_value _pressed = mrb_bool_value(pressed);
  mrb_value argv[5] = { self, _scancode, _keycode, _mod, _pressed };

  if( mrb_symbol_p(obj) ){
    return mrb_bool( mrb_funcall_argv(mrb,self,mrb_symbol(obj),5,argv) );
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    return mrb_bool( mrb_yield_argv(mrb, obj, 5, argv) );
  }
  return false;
}

static bool on_move_finger(BiContext* context, BiNode* node, float x, float y, int64_t finger)
{
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_move_finger_callback_") );
  mrb_value _x = mrb_float_value(mrb,x);
  mrb_value _y = mrb_float_value(mrb,y);
  mrb_value _finger = mrb_fixnum_value(finger);
  mrb_value argv[4] = { self, _x, _y, _finger };

  if( mrb_symbol_p(obj) ){
    return mrb_bool( mrb_funcall_argv(mrb,self,mrb_symbol(obj),4,argv) );
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    return mrb_bool( mrb_yield_argv(mrb, obj, 4, argv) );
  }
  return false;
}

static bool on_touch(BiContext* context, BiNode* node, float x, float y, int64_t finger, bool pressed)
{
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_touch_callback_") );
  mrb_value _x = mrb_float_value(mrb,x);
  mrb_value _y = mrb_float_value(mrb,y);
  mrb_value _finger = mrb_fixnum_value(finger);
  mrb_value _pressed = mrb_bool_value(pressed);
  mrb_value argv[5] = { self, _x, _y, _finger, _pressed };

  if( mrb_symbol_p(obj) ){
    return mrb_bool( mrb_funcall_argv(mrb,self,mrb_symbol(obj),5,argv) );
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    return mrb_bool( mrb_yield_argv(mrb, obj, 5, argv) );
  }
  return false;
}

static bool on_textinput(BiContext* context, BiNode* node, char* text)
{
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_textinput_callback_") );
  mrb_value _text = mrb_str_new_cstr(mrb,text);
  mrb_value argv[2] = { self, _text };

  if( mrb_symbol_p(obj) ){
    return mrb_bool( mrb_funcall_argv(mrb,self,mrb_symbol(obj),2,argv) );
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    return mrb_bool( mrb_yield_argv(mrb, obj, 2, argv) );
  }
  return false;
}

//
// BiNode
//

static mrb_value color_obj(mrb_state *mrb,BiColor* color)
{
  struct RClass *color_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Color");
  struct RData *color_data = mrb_data_object_alloc(mrb,color_class,color,&mrb_bi_color_data_type);
  return mrb_obj_value(color_data);
}

static mrb_value mrb_node_initialize(mrb_state *mrb, mrb_value self)
{
  BiNode* node = mrb_malloc(mrb, sizeof(BiNode));
  bi_node_init(node);
  mrb_data_init(self, node, &mrb_node_data_type);
  node->userdata = mrb_ptr(self);
  // Color
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@color_tint"), color_obj(mrb,&node->color_tint) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@color_modulate"), color_obj(mrb,&node->color_modulate) );
  return self;
}

//
// scene graph
//

static BiNode* bi_node_from_obj(mrb_state *mrb,mrb_value obj)
{
  struct RClass* node_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Node");
  if( ! mrb_obj_is_kind_of(mrb,obj,node_class) ) {
    return NULL;
  }
  return DATA_PTR(obj);
}

static mrb_value mrb_node_add_child(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiNode* child = bi_node_from_obj(mrb,obj);
  if(!child) {
    return mrb_nil_value();
  }
  BiNode* node = DATA_PTR(self);
  bi_node_add_node(node,child);
  mrb_value iv_children = _iv_children_(mrb,self);
  mrb_ary_push(mrb,iv_children,obj);
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),self);
  return self;
}

static mrb_value mrb_node_remove_child(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj );
  BiNode* child = bi_node_from_obj(mrb,obj);
  if(!child) {
    return mrb_nil_value();
  }
  BiNode* node = DATA_PTR(self);
  mrb_value iv_children = _iv_children_(mrb,self);
  mrb_iv_set(mrb,obj,MRB_IVSYM(parent),mrb_nil_value());
  mrb_funcall(mrb,iv_children,"delete",1,obj);
  bi_node_remove_node(node,child);
  return self;
}

//
// geometry
//

_GET_(BiNode,x,bi_mrb_fixnum_value);
_GET_(BiNode,y,bi_mrb_fixnum_value);
_GET_(BiNode,z,bi_mrb_fixnum_value);

_SET_FUNC_(BiNode,x,mrb_int,i,bi_node_set_x);
_SET_FUNC_(BiNode,y,mrb_int,i,bi_node_set_y);
_SET_FUNC_(BiNode,z,mrb_int,i,bi_node_set_z);

static mrb_value mrb_node_set_position(mrb_state *mrb, mrb_value self)
{
  mrb_int x,y;
  mrb_get_args(mrb, "ii", &x, &y );
  BiNode* node = DATA_PTR(self);
  bi_node_set_position(node,x,y);
  return self;
}

static mrb_value mrb_node_angle(mrb_state *mrb, mrb_value self)
{
  BiNode* node = DATA_PTR(self);
  return mrb_float_value(mrb,node->angle);
}

_SET_FUNC_(BiNode,angle,mrb_float,f,bi_node_set_angle);

// Size
_GET_(BiNode,w,bi_mrb_fixnum_value);
_GET_(BiNode,h,bi_mrb_fixnum_value);
static mrb_value mrb_node_set_size(mrb_state *mrb, mrb_value self)
{
  mrb_int w,h;
  mrb_get_args(mrb, "ii", &w, &h );
  BiNode* node = DATA_PTR(self);
  bi_node_set_size(node,w,h);
  return self;
}

// Scale
_GET_(BiNode,scale_x,mrb_float_value);
_GET_(BiNode,scale_y,mrb_float_value);
_SET_FUNC_(BiNode,scale_x,mrb_float,f,bi_node_set_scale_x);
_SET_FUNC_(BiNode,scale_y,mrb_float,f,bi_node_set_scale_y);
static mrb_value mrb_node_set_scale(mrb_state *mrb, mrb_value self)
{
  mrb_float x,y;
  mrb_get_args(mrb, "ff", &x, &y );
  BiNode* node = DATA_PTR(self);
  bi_node_set_scale(node,x,y);
  return self;
}

// Anchor
static mrb_value mrb_node_set_anchor(mrb_state *mrb, mrb_value self)
{
  mrb_float x,y;
  mrb_get_args(mrb, "ff", &x, &y );
  BiNode* node = DATA_PTR(self);
  bi_node_set_anchor(node,x,y);
  return self;
}
_GET_(BiNode,anchor_x,mrb_float_value);
_GET_(BiNode,anchor_y,mrb_float_value);
_SET_FUNC_(BiNode,anchor_x,mrb_float,f,bi_node_set_anchor_x);
_SET_FUNC_(BiNode,anchor_y,mrb_float,f,bi_node_set_anchor_y);


static mrb_value mrb_node_is_include(mrb_state *mrb, mrb_value self)
{
  mrb_int x,y;
  mrb_get_args(mrb, "ii", &x, &y );
  BiNode* node = (BiNode*)DATA_PTR(self);
  return mrb_bool_value(bi_node_inside(node,x,y));
}

static mrb_value mrb_node_transform_local(mrb_state *mrb, mrb_value self)
{
  mrb_int x,y;
  int lx,ly;
  BiNode* node;
  mrb_value v[2];

  mrb_get_args(mrb, "ii", &x, &y );
  node = (BiNode*)DATA_PTR(self);
  bi_node_transform_local(node, x, y, &lx, &ly);

  v[0] = mrb_fixnum_value(lx);
  v[1] = mrb_fixnum_value(ly);
  return mrb_ary_new_from_values(mrb,2,v);
}

//
// Color
//
static mrb_value mrb_node_set_color_modulate(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiColor* color = DATA_PTR(color_obj);
  BiNode* node = DATA_PTR(self);
  node->color_modulate = *color;
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@color_modulate") );
}

static mrb_value mrb_node_set_color_tint(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiColor* color = DATA_PTR(color_obj);
  BiNode* node = DATA_PTR(self);
  node->color_tint = *color;
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@color_tint") );
}

// Visibility
static mrb_value mrb_node_set_visible(mrb_state *mrb, mrb_value self)
{
  mrb_bool visible;
  mrb_get_args(mrb, "b", &visible );
  BiNode* node = DATA_PTR(self);
  node->visible = visible;
  return self;
}
static mrb_value mrb_node_get_visible(mrb_state *mrb, mrb_value self)
{
  BiNode* node = DATA_PTR(self);
  return mrb_bool_value(node->visible);
}

// Texture
static mrb_value mrb_node_set_texture(mrb_state *mrb, mrb_value self)
{
  mrb_int x,y,w,h;
  mrb_int cx,cy,ow,oh;
  mrb_value tex_obj;
  mrb_int num = mrb_get_args(mrb, "oiiii|iiii", &tex_obj,&x,&y,&w,&h, &cx,&cy,&ow,&oh );
  BiNode* node = DATA_PTR(self);
  BiTexture* tex = DATA_PTR(tex_obj);
  if(num==5){
    bi_node_set_texture(node,tex,x,y,w,h);
  }else{
    bi_node_set_cropped_texture(node,tex, x,y,w,h, cx,cy,ow,oh);
  }
  mrb_iv_set(mrb,self,MRB_IVSYM(texture),tex_obj);
  return self;
}
static mrb_value mrb_node_unset_texture(mrb_state *mrb, mrb_value self)
{
  BiNode* node = DATA_PTR(self);
  bi_node_unset_texture(node);
  return self;
}
_GET_(BiNode,texture_flip_vertical,bi_mrb_bool_value);
_GET_(BiNode,texture_flip_horizontal,bi_mrb_bool_value);
_SET_(BiNode,texture_flip_vertical,mrb_bool,b);
_SET_(BiNode,texture_flip_horizontal,mrb_bool,b);

//
// callback
//

#define SET_CALLBACK(CALLBACK_NAME) \
  mrb_value obj; \
  mrb_get_args(mrb, "o", &obj ); \
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@_" #CALLBACK_NAME "_callback_" ), obj); \
  bi_node_set_ ## CALLBACK_NAME (DATA_PTR(self),  CALLBACK_NAME ); \
  return self;

static mrb_value mrb_node_on_click(mrb_state *mrb, mrb_value self) {
  SET_CALLBACK(on_click);
}

static mrb_value mrb_node_on_move_cursor(mrb_state *mrb, mrb_value self) {
  SET_CALLBACK(on_move_cursor);
}

static mrb_value mrb_node_on_key_input(mrb_state *mrb, mrb_value self) {
  SET_CALLBACK(on_keyinput);
}

static mrb_value mrb_node_on_move_finger(mrb_state *mrb, mrb_value self) {
  SET_CALLBACK(on_move_finger);
}

static mrb_value mrb_node_on_touch(mrb_state *mrb, mrb_value self)
{
  SET_CALLBACK(on_touch);
}

static mrb_value mrb_node_on_text_input(mrb_state *mrb, mrb_value self) {
  SET_CALLBACK(on_textinput);
}

// gem

void mrb_init_bi_node(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *node;
  node = mrb_define_class_under(mrb, bi, "Node", mrb->object_class);
  MRB_SET_INSTANCE_TT(node, MRB_TT_DATA);

  mrb_define_method(mrb, node, "initialize", mrb_node_initialize, MRB_ARGS_NONE());

  // scene graph
  mrb_define_method(mrb, node, "add", mrb_node_add_child, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "remove", mrb_node_remove_child, MRB_ARGS_REQ(1));

  // geometry
  mrb_define_method(mrb, node, "x", mrb_BiNode_get_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "y", mrb_BiNode_get_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "z", mrb_BiNode_get_z, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "x=", mrb_BiNode_set_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "y=", mrb_BiNode_set_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "z=", mrb_BiNode_set_z, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "set_position", mrb_node_set_position, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, node, "include?", mrb_node_is_include, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, node, "transform_local", mrb_node_transform_local, MRB_ARGS_REQ(2));
  // size
  mrb_define_method(mrb, node, "w", mrb_BiNode_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "h", mrb_BiNode_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "set_size", mrb_node_set_size, MRB_ARGS_REQ(2));
  // scale
  mrb_define_method(mrb, node, "scale_x", mrb_BiNode_get_scale_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "scale_y", mrb_BiNode_get_scale_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "scale_x=", mrb_BiNode_set_scale_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "scale_y=", mrb_BiNode_set_scale_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "set_scale", mrb_node_set_scale, MRB_ARGS_REQ(2));
  // anchor
  mrb_define_method(mrb, node, "anchor_x", mrb_BiNode_get_anchor_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "anchor_y", mrb_BiNode_get_anchor_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "anchor_x=",mrb_BiNode_set_anchor_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "anchor_y=",mrb_BiNode_set_anchor_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "set_anchor",mrb_node_set_anchor, MRB_ARGS_REQ(2));
  // angle
  mrb_define_method(mrb, node, "angle", mrb_node_angle, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "angle=", mrb_BiNode_set_angle, MRB_ARGS_REQ(1));

  // color
  mrb_define_method(mrb, node, "color_tint=", mrb_node_set_color_tint, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "color_modulate=", mrb_node_set_color_modulate, MRB_ARGS_REQ(1));

  // Visibility
  mrb_define_method(mrb, node, "visible=", mrb_node_set_visible, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "visible", mrb_node_get_visible, MRB_ARGS_NONE());

  // Texture
  mrb_define_method(mrb, node, "set_texture", mrb_node_set_texture, MRB_ARGS_REQ(5)|MRB_ARGS_OPT(4)); // tex,x,y,w,h, xywh
  mrb_define_method(mrb, node, "unset_texture", mrb_node_unset_texture, MRB_ARGS_NONE()); // tex,x,y,w,h, xywh
  mrb_define_method(mrb, node, "flip_vertical", mrb_BiNode_get_texture_flip_vertical, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "flip_vertical=", mrb_BiNode_set_texture_flip_vertical, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "flip_horizontal", mrb_BiNode_get_texture_flip_horizontal, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "flip_horizontal=", mrb_BiNode_set_texture_flip_horizontal, MRB_ARGS_REQ(1));

  // callback
  mrb_define_method(mrb, node, "_on_click_", mrb_node_on_click, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_move_cursor_", mrb_node_on_move_cursor, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_key_input_", mrb_node_on_key_input, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_move_finger_", mrb_node_on_move_finger, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_touch_", mrb_node_on_touch, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_text_input_", mrb_node_on_text_input, MRB_ARGS_REQ(1));
}
