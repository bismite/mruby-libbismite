#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/error.h>
#include <mruby/presym.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <time.h>
#include "_inner_macro.h"
#include "_init_layer_group.h"
#include "_color.h"

// modules
extern void mrb_init_bi_profile(mrb_state*, struct RClass*);
extern void mrb_init_bi_node(mrb_state*, struct RClass*);
extern void mrb_init_bi_texture(mrb_state*, struct RClass*);
extern void mrb_init_bi_timer(mrb_state*, struct RClass*);
extern void mrb_init_bi_layer(mrb_state*, struct RClass*);
extern void mrb_init_bi_layer_group(mrb_state*, struct RClass*);
extern void mrb_init_bi_canvas(mrb_state*, struct RClass*);
extern void mrb_init_bi_shader(mrb_state*, struct RClass*);
extern void mrb_init_bi_key(mrb_state*, struct RClass*);
extern void mrb_init_bi_version(mrb_state*, struct RClass*);
extern void mrb_init_bi_color(mrb_state*, struct RClass*);
extern void mrb_init_bi_framebuffer(mrb_state*, struct RClass*);
// modules ext
extern void mrb_init_font(mrb_state *mrb, struct RClass *sg);
extern void mrb_init_label(mrb_state *mrb, struct RClass *sg);
extern void mrb_init_action(mrb_state *mrb, struct RClass *sg);

//
// Bi class
//
static struct mrb_data_type const mrb_bi_data_type = { "Bi", mrb_free };

// XXX: for static class in context structure
static struct mrb_data_type const mrb_layer_group_data_type = { "LayerGroup", NULL };
static struct mrb_data_type const mrb_shader_data_type = { "Shader", NULL };

static mrb_value mrb_bi_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int width, height, fps;
  mrb_bool highdpi;
  mrb_value title_obj;
  mrb_get_args(mrb, "iiibS", &width, &height, &fps, &highdpi, &title_obj );
  const char* title_str = mrb_string_value_cstr(mrb,&title_obj);
  //
  BiContext* c = mrb_malloc(mrb,sizeof(BiContext));
  bi_init_context(c, width, height, fps, highdpi, title_str);
  c->userdata = mrb; // XXX: hold mrb_state
  mrb_data_init(self, c, &mrb_bi_data_type);
  // layer group
  struct RClass *lg_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"LayerGroup");
  struct RData *lg_data = mrb_data_object_alloc(mrb,lg_class,&c->layers,&mrb_layer_group_data_type);
  mrb_value lg_obj = _init_layer_group_(mrb,mrb_obj_value(lg_data),&c->layers);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@layers"),lg_obj);
  // default shader
  struct RClass *shader_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Shader");
  struct RData *shader_data = mrb_data_object_alloc(mrb,shader_class,&c->default_shader,&mrb_shader_data_type);
  mrb_value shader_obj = mrb_obj_value(shader_data);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@default_shader"),shader_obj);
  // color
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@color"), color_obj(mrb,&c->color) );
  return self;
}

static void on_mainloop_end(BiContext* context)
{
  mrb_state *mrb = context->userdata;
  // catch exception in mainloop
  if (mrb->exc) {
    MRB_EXC_CHECK_EXIT(mrb, mrb->exc);
    // mrb_p(mrb, mrb_obj_value(mrb->exc));
    mrb_print_error(mrb);
    bi_stop_run_loop();
  }
}

static mrb_value mrb_bi_start_run_loop(mrb_state *mrb, mrb_value self)
{
  BiContext* c = DATA_PTR(self);
  c->on_mainloop_end = on_mainloop_end;
  bi_start_run_loop(c);
  return self;
}

//
// Window Size
//
_GET_INT_(BiContext,w);
_GET_INT_(BiContext,h);

//
// Color
//
static mrb_value mrb_bi_set_color(mrb_state *mrb, mrb_value self)
{
  mrb_value color_obj;
  mrb_get_args(mrb, "o", &color_obj );
  BiContext* c = DATA_PTR(self);
  BiColor* color = DATA_PTR(color_obj);
  c->color = *color;
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@color"));;
}
static mrb_value mrb_bi_get_color(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@color"));
}

//
// Title
//
static mrb_value mrb_bi_set_title(mrb_state *mrb, mrb_value self)
{
  mrb_value title_obj;
  mrb_get_args(mrb, "S", &title_obj );
  BiContext* c = DATA_PTR(self);
  bi_set_title( c, mrb_string_value_cstr(mrb,&title_obj) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@title"), title_obj );
  return self;
}
static mrb_value mrb_bi_get_title(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@title"));
}

//
// misc
//

_GET_BOOL_(BiContext,debug);
_SET_BOOL_(BiContext,debug);
static mrb_value mrb_bi_now(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value( bi_get_now() );
}

static mrb_value mrb_bi_messagebox(mrb_state *mrb, mrb_value self)
{
  mrb_value title,message;
  mrb_sym dialog_type;
  mrb_get_args(mrb, "SSn",  &title, &message, &dialog_type );
  BiContext *context = DATA_PTR(self);
  if(dialog_type == mrb_intern_lit(mrb,"error")){
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             RSTRING_CSTR(mrb,title),
                             RSTRING_CSTR(mrb,message),
                             context->window);
  }
  else if(dialog_type==mrb_intern_lit(mrb,"warning")){
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
                             RSTRING_CSTR(mrb,title),
                             RSTRING_CSTR(mrb,message),
                             context->window);
  }else{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             RSTRING_CSTR(mrb,title),
                             RSTRING_CSTR(mrb,message),
                             context->window);
  }
  return self;
}

//
// init
//
void mrb_mruby_libbismite_gem_init(mrb_state* mrb)
{
  struct RClass *bi;
  bi = mrb_define_class(mrb, "Bi", mrb->object_class);
  MRB_SET_INSTANCE_TT(bi, MRB_TT_DATA);
  mrb_define_method(mrb, bi, "initialize", mrb_bi_initialize, MRB_ARGS_REQ(5));
  mrb_define_method(mrb, bi, "start_run_loop", mrb_bi_start_run_loop, MRB_ARGS_NONE());
  // window size
  mrb_define_method(mrb, bi, "w", mrb_BiContext_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "h", mrb_BiContext_get_h, MRB_ARGS_NONE());
  // color
  mrb_define_method(mrb, bi, "color=", mrb_bi_set_color, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "color", mrb_bi_get_color, MRB_ARGS_NONE());
  // title
  mrb_define_method(mrb, bi, "title=", mrb_bi_set_title, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "title", mrb_bi_get_title, MRB_ARGS_NONE());
  //
  mrb_define_method(mrb, bi, "now", mrb_bi_now, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "debug=", mrb_BiContext_set_debug, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "debug", mrb_BiContext_get_debug, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "messagebox", mrb_bi_messagebox, MRB_ARGS_REQ(3));
  //
  mrb_init_bi_profile(mrb,bi);;
  mrb_init_bi_node(mrb,bi);;
  mrb_init_bi_texture(mrb,bi);;
  mrb_init_bi_timer(mrb,bi);
  mrb_init_bi_layer(mrb,bi);
  mrb_init_bi_layer_group(mrb,bi);
  mrb_init_bi_canvas(mrb,bi);
  mrb_init_bi_shader(mrb,bi);
  mrb_init_bi_key(mrb,bi);
  mrb_init_bi_version(mrb,bi);
  mrb_init_bi_color(mrb,bi);
  mrb_init_bi_framebuffer(mrb,bi);
  // ext
  mrb_init_font(mrb,bi);
  mrb_init_label(mrb,bi);
  mrb_init_action(mrb,bi);
  // blend functions
  // source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml
  mrb_define_const(mrb, mrb->kernel_module, "GL_ZERO", mrb_fixnum_value(GL_ZERO));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE", mrb_fixnum_value(GL_ONE));
  mrb_define_const(mrb, mrb->kernel_module, "GL_SRC_COLOR", mrb_fixnum_value(GL_SRC_COLOR));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE_MINUS_SRC_COLOR", mrb_fixnum_value(GL_ONE_MINUS_SRC_COLOR));
  mrb_define_const(mrb, mrb->kernel_module, "GL_DST_COLOR", mrb_fixnum_value(GL_DST_COLOR));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE_MINUS_DST_COLOR", mrb_fixnum_value(GL_ONE_MINUS_DST_COLOR));
  mrb_define_const(mrb, mrb->kernel_module, "GL_SRC_ALPHA", mrb_fixnum_value(GL_SRC_ALPHA));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE_MINUS_SRC_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_SRC_ALPHA));
  mrb_define_const(mrb, mrb->kernel_module, "GL_DST_ALPHA", mrb_fixnum_value(GL_DST_ALPHA));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE_MINUS_DST_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_DST_ALPHA));
  mrb_define_const(mrb, mrb->kernel_module, "GL_CONSTANT_COLOR", mrb_fixnum_value(GL_CONSTANT_COLOR));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE_MINUS_CONSTANT_COLOR", mrb_fixnum_value(GL_ONE_MINUS_CONSTANT_COLOR));
  mrb_define_const(mrb, mrb->kernel_module, "GL_CONSTANT_ALPHA", mrb_fixnum_value(GL_CONSTANT_ALPHA));
  mrb_define_const(mrb, mrb->kernel_module, "GL_ONE_MINUS_CONSTANT_ALPHA", mrb_fixnum_value(GL_ONE_MINUS_CONSTANT_ALPHA));
  mrb_define_const(mrb, mrb->kernel_module, "GL_SRC_ALPHA_SATURATE", mrb_fixnum_value(GL_SRC_ALPHA_SATURATE));
}

void mrb_mruby_libbismite_gem_final(mrb_state* mrb)
{
}
