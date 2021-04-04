#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/presym.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/context.h>
#include <bi/node.h>
#include <bi/util.h>
#include <stdlib.h>
#include "bi_core_inner_macro.h"

// Bi::Node class
static void node_free(mrb_state *mrb, void *p)
{
  BiNode* node = p;
  if (NULL != node) {
    if(node->children != NULL) free(node->children);
    free(node->timers.timers);
    mrb_free(mrb, node);
  }
}
static struct mrb_data_type const mrb_node_data_type = { "Node", node_free };

//
// callback function
//

static void _update_callback_(BiContext* context, void *userdata)
{
  BiNode *node = userdata;
  mrb_state *mrb = context->userdata;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_update_callback_") );
  mrb_value argv[1] = {self};

  if( mrb_symbol_p(obj) ){
    mrb_funcall_argv(mrb,self,mrb_symbol(obj),1,argv);
  }else if( mrb_type(obj) == MRB_TT_PROC ) {
    mrb_yield_argv(mrb,obj,1,argv);
  }
}

static bool _on_click_callback_(BiNode* node, void *callback_context, int x, int y, int button, bool pressed)
{
  mrb_state *mrb = callback_context;
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

static bool _on_move_cursor_callback_(BiNode* node, void *callback_context, int x, int y)
{
  mrb_state *mrb = callback_context;
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

static bool _on_key_input_callback_(BiNode* node, void *callback_context, uint16_t scancode, uint32_t keycode, uint16_t mod, bool pressed)
{
  mrb_state *mrb = callback_context;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_key_input_callback_") );
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

static bool _on_move_finger_callback_(BiNode* node, void *callback_context, float x, float y, int64_t finger)
{
  mrb_state *mrb = callback_context;
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

static bool _on_touch_callback_(BiNode* node, void *callback_context, float x, float y, int64_t finger, bool pressed)
{
  mrb_state *mrb = callback_context;
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

static bool _on_text_input_callback_(BiNode* node, void *callback_context, char* text)
{
  mrb_state *mrb = callback_context;
  mrb_value self = mrb_obj_value(node->userdata);
  mrb_value obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@_on_text_input_callback_") );
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
// node functions
//

static mrb_value mrb_node_initialize(mrb_state *mrb, mrb_value self)
{
    BiNode* node;

    node = mrb_malloc(mrb, sizeof(BiNode));
    if (NULL == node) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }

    bi_node_init(node);
    mrb_data_init(self, node, &mrb_node_data_type);
    node->userdata = mrb_ptr(self);

    return self;
}

//
// scene graph
//

static mrb_value _iv_children_(mrb_state *mrb, mrb_value self)
{
  mrb_sym iv_name = MRB_IVSYM(_children_);
  mrb_value v = mrb_iv_get(mrb,self,iv_name);
  if( mrb_nil_p(v) ) {
    v = mrb_ary_new(mrb);
    mrb_iv_set(mrb,self,iv_name,v);
  }
  return v;
}

static mrb_value mrb_node_add_child(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    struct RClass* node_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Node");
    if( ! mrb_obj_is_kind_of(mrb,obj,node_class) ) {
      return mrb_nil_value();
    }
    BiNode* child = DATA_PTR(obj);
    if(!child) {
      return mrb_nil_value();
    }
    BiNode* node = DATA_PTR(self);
    bi_add_node(node,child);
    mrb_value iv_children = _iv_children_(mrb,self);
    mrb_ary_push(mrb,iv_children,obj);
    mrb_iv_set(mrb,obj,MRB_IVSYM(parent),self);
    return self;
}

static mrb_value mrb_node_remove_child(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    struct RClass* node_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Node");
    if( ! mrb_obj_is_kind_of(mrb,obj,node_class) ) {
      return mrb_nil_value();
    }
    BiNode* child = DATA_PTR(obj);
    if(!child) {
      return mrb_nil_value();
    }
    BiNode* node = DATA_PTR(self);
    mrb_value iv_children = _iv_children_(mrb,self);
    mrb_iv_set(mrb,obj,MRB_IVSYM(parent),mrb_nil_value());
    mrb_funcall(mrb,iv_children,"delete",1,obj);
    child = bi_remove_node(node,child);
    if(child!=NULL) {
      child->parent = NULL;
    }
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
    return mrb_float_value(mrb,node->angle * 180.0 / 3.14159);
}

_SET_FUNC_(BiNode,angle,mrb_float,f,bi_node_set_degree);

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

_GET_(BiNode,scale_x,mrb_float_value);
_GET_(BiNode,scale_y,mrb_float_value);
_SET_FUNC_(BiNode,scale_x,mrb_float,f,bi_node_set_scale_x);
_SET_FUNC_(BiNode,scale_y,mrb_float,f,bi_node_set_scale_y);

_GET_(BiNode,anchor_x,mrb_float_value);
_GET_(BiNode,anchor_y,mrb_float_value);
_SET_(BiNode,anchor_x,mrb_float,f);
_SET_(BiNode,anchor_y,mrb_float,f);

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
// visual
//

static mrb_value mrb_node_set_texture_mapping(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    node->texture_mapping = DATA_PTR(obj);

    return self;
}

static mrb_value mrb_node_set_color(mrb_state *mrb, mrb_value self)
{
    mrb_int r,g,b,a;
    mrb_get_args(mrb, "iiii", &r, &g, &b, &a );

    BiNode* node = DATA_PTR(self);
    bi_set_color(node->color,r,g,b,a);

    return self;
}

static mrb_value mrb_node_set_alpha(mrb_state *mrb, mrb_value self)
{
    mrb_int a;
    mrb_get_args(mrb, "i", &a );

    BiNode* node = DATA_PTR(self);
    node->color[3] = a;

    return self;
}

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

//
// callback
//

static mrb_value mrb_node_on_update(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_update_callback_"), obj);
    bi_set_on_update(node, _update_callback_, mrb);

    return self;
}

static mrb_value mrb_node_on_click(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_click_callback_"), obj);
    bi_set_on_click(node, _on_click_callback_, mrb);

    return self;
}

static mrb_value mrb_node_on_move_cursor(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_move_cursor_callback_"), obj);
    bi_set_on_move_cursor(node, _on_move_cursor_callback_, mrb);

    return self;
}

static mrb_value mrb_node_on_key_input(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_key_input_callback_"), obj);
    bi_set_on_keyinput(node, _on_key_input_callback_, mrb);

    return self;
}

static mrb_value mrb_node_on_move_finger(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_move_finger_callback_"), obj);
    bi_set_on_move_finger(node, _on_move_finger_callback_, mrb);

    return self;
}

static mrb_value mrb_node_on_touch(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_touch_callback_"), obj);
    bi_set_on_touch(node, _on_touch_callback_, mrb);

    return self;
}

static mrb_value mrb_node_on_text_input(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    BiNode* node = DATA_PTR(self);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_on_text_input_callback_"), obj);
    bi_set_on_textinput(node, _on_text_input_callback_, mrb);

    return self;
}

//
// Timer
//

static mrb_value mrb_bi_add_timer(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiNode* node = DATA_PTR(self);
    BiTimer* timer = DATA_PTR(obj);

    bi_add_timer(&node->timers,timer);

    return self;
}

static mrb_value mrb_bi_remove_timer(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiNode* node = DATA_PTR(self);
    BiTimer* timer = DATA_PTR(obj);

    bi_remove_timer(&node->timers,timer);

    return self;
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
  mrb_define_method(mrb, node, "w", mrb_BiNode_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "h", mrb_BiNode_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "set_size", mrb_node_set_size, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, node, "scale_x", mrb_BiNode_get_scale_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "scale_y", mrb_BiNode_get_scale_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "scale_x=", mrb_BiNode_set_scale_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "scale_y=", mrb_BiNode_set_scale_y, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, node, "anchor_x=",mrb_BiNode_set_anchor_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "anchor_x", mrb_BiNode_get_anchor_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "anchor_y=",mrb_BiNode_set_anchor_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "anchor_y", mrb_BiNode_get_anchor_y, MRB_ARGS_NONE());

  mrb_define_method(mrb, node, "angle", mrb_node_angle, MRB_ARGS_NONE());
  mrb_define_method(mrb, node, "angle=", mrb_BiNode_set_angle, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "include?", mrb_node_is_include, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, node, "transform_local", mrb_node_transform_local, MRB_ARGS_REQ(2));

  // visual
  mrb_define_method(mrb, node, "set_texture_mapping", mrb_node_set_texture_mapping, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "set_color", mrb_node_set_color, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, node, "set_alpha", mrb_node_set_alpha, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "visible=", mrb_node_set_visible, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "visible", mrb_node_get_visible, MRB_ARGS_NONE());

  // callback
  mrb_define_method(mrb, node, "_on_update_", mrb_node_on_update, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_click_", mrb_node_on_click, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_move_cursor_", mrb_node_on_move_cursor, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_key_input_", mrb_node_on_key_input, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_move_finger_", mrb_node_on_move_finger, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_touch_", mrb_node_on_touch, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_on_text_input_", mrb_node_on_text_input, MRB_ARGS_REQ(1));

  // timer
  mrb_define_method(mrb, node, "_add_timer", mrb_bi_add_timer, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, node, "_remove_timer", mrb_bi_remove_timer, MRB_ARGS_REQ(1));

}
