#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <time.h>
#include "_inner_macro.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
int timespec_get(struct timespec *ts, int base)
{
  double msec = EM_ASM_DOUBLE({ return Date.now(); });
  ts->tv_sec = msec / 1000;
  int ms = (uint64_t)msec % 1000;
  ts->tv_nsec = ms * 1000 * 1000;
  return 1;
}
#endif

// modules
extern void mrb_init_bi_profile(mrb_state*, struct RClass*);
extern void mrb_init_bi_node(mrb_state*, struct RClass*);
extern void mrb_init_bi_texture(mrb_state*, struct RClass*);
extern void mrb_init_bi_texture_mapping(mrb_state*, struct RClass*);
extern void mrb_init_bi_timer(mrb_state*, struct RClass*);
extern void mrb_init_bi_timer_runner(mrb_state*, struct RClass*);
extern void mrb_init_bi_layer(mrb_state*, struct RClass*);
extern void mrb_init_bi_layer_group(mrb_state*, struct RClass*);
extern void mrb_init_bi_canvas(mrb_state*, struct RClass*);
extern void mrb_init_bi_shader(mrb_state*, struct RClass*);
extern void mrb_init_bi_key(mrb_state*, struct RClass*);
extern void mrb_init_bi_version(mrb_state*, struct RClass*);
// modules ext
extern void mrb_init_font(mrb_state *mrb, struct RClass *sg);
extern void mrb_init_label(mrb_state *mrb, struct RClass *sg);
extern void mrb_init_action(mrb_state *mrb, struct RClass *sg);
extern void mrb_init_transition(mrb_state *mrb, struct RClass *sg);

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

  BiContext* c = mrb_malloc(mrb,sizeof(BiContext));
  bi_init_context(c, width, height, fps, highdpi, title_str);
  c->userdata = mrb; // XXX: hold mrb_state
  mrb_data_init(self, c, &mrb_bi_data_type);

  // layer group
  struct RClass *lg_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"LayerGroup");
  struct RData *lg_data = mrb_data_object_alloc(mrb,lg_class,&c->layers,&mrb_layer_group_data_type);
  mrb_value lg_obj = mrb_obj_value(lg_data);
  c->layers.userdata = mrb_ptr(lg_obj);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@layers"), lg_obj);
  mrb_iv_set(mrb, lg_obj, mrb_intern_cstr(mrb,"@layers"),mrb_ary_new(mrb));
  mrb_iv_set(mrb, lg_obj, mrb_intern_cstr(mrb,"@post_processes"),mrb_ary_new(mrb));

  // default shader
  struct RClass *shader_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Shader");
  struct RData *shader_data = mrb_data_object_alloc(mrb,shader_class,&c->default_shader,&mrb_shader_data_type);
  mrb_value shader_obj = mrb_obj_value(shader_data);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@default_shader"),shader_obj);

  return self;
}

static mrb_value mrb_bi_now(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value( bi_get_now() );
}

static mrb_value mrb_bi_start_run_loop(mrb_state *mrb, mrb_value self)
{
  // XXX: error check
  BiContext* c = DATA_PTR(self);

  // XXX: start infinit loop
  bi_start_run_loop(c);

  return self;
}

// window width, height
_GET_(BiContext,w,bi_mrb_fixnum_value);
_GET_(BiContext,h,bi_mrb_fixnum_value);

_GET_(BiContext,debug,bi_mrb_bool_value);
_SET_(BiContext,debug,mrb_bool,b);


static mrb_value mrb_bi_set_title(mrb_state *mrb, mrb_value self)
{
  mrb_value title;
  mrb_get_args(mrb, "S", &title );

  // XXX: error check
  BiContext* c = DATA_PTR(self);

  bi_set_title( c, mrb_string_value_cstr(mrb,&title) );

  return self;
}

//
// misc
//
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

  mrb_define_method(mrb, bi, "w", mrb_BiContext_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "h", mrb_BiContext_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "now", mrb_bi_now, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "start_run_loop", mrb_bi_start_run_loop, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "debug=", mrb_BiContext_set_debug, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "debug", mrb_BiContext_get_debug, MRB_ARGS_NONE());

  mrb_define_method(mrb, bi, "set_title", mrb_bi_set_title, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, bi, "messagebox", mrb_bi_messagebox, MRB_ARGS_REQ(3));

#define DONE mrb_gc_arena_restore(mrb, 0)
  mrb_init_bi_profile(mrb,bi); DONE;
  mrb_init_bi_node(mrb,bi); DONE;
  mrb_init_bi_texture(mrb,bi); DONE;
  mrb_init_bi_texture_mapping(mrb,bi); DONE;
  mrb_init_bi_timer(mrb,bi); DONE;
  mrb_init_bi_timer_runner(mrb,bi); DONE;
  mrb_init_bi_layer(mrb,bi); DONE;
  mrb_init_bi_layer_group(mrb,bi); DONE;
  mrb_init_bi_canvas(mrb,bi); DONE;
  mrb_init_bi_shader(mrb,bi); DONE;
  mrb_init_bi_key(mrb,bi); DONE;
  mrb_init_bi_version(mrb,bi); DONE;
  // ext
  mrb_init_font(mrb,bi); DONE;
  mrb_init_label(mrb,bi); DONE;
  mrb_init_action(mrb,bi); DONE;
  mrb_init_transition(mrb,bi); DONE;
#undef DONE
}

void mrb_mruby_libbismite_gem_final(mrb_state* mrb)
{
}
