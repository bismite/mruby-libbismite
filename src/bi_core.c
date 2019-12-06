#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <time.h>
#include "bi_core_inner_macro.h"

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
void mrb_init_node(mrb_state *mrb, struct RClass *sg);
void mrb_init_texture_image(mrb_state *mrb, struct RClass *sg);
void mrb_init_texture(mrb_state *mrb, struct RClass *sg);
void mrb_init_timer(mrb_state *mrb, struct RClass *sg);
void mrb_init_layer(mrb_state *mrb, struct RClass *sg);
void mrb_init_key(mrb_state *mrb, struct RClass *sg);

//
// callback function
//

static void _update_callback_(BiContext* context, void *userdata, double delta)
{
  mrb_state *mrb = context->userdata;
  mrb_value obj = mrb_obj_value(userdata);
  mrb_value delta_value = mrb_float_value(mrb,delta);
  mrb_value argv[1] = { delta_value };

  if( mrb_type(obj) == MRB_TT_PROC ) {
    mrb_yield_argv(mrb,obj,1,argv);
  }
}

//
// Bi class
//

static struct mrb_data_type const mrb_bi_data_type = { "Bi", mrb_free };

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

    return self;
}

static mrb_value mrb_bi_now(mrb_state *mrb, mrb_value self)
{
    // XXX: error check
    BiContext* c = DATA_PTR(self);
    return mrb_float_value(mrb,c->profile.frame_start_at);
}

static mrb_value mrb_bi_fps(mrb_state *mrb, mrb_value self)
{
    // XXX: error check
    BiContext* c = DATA_PTR(self);
    return mrb_fixnum_value(c->profile.current_fps);
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
// Layer
//

static mrb_value mrb_bi_add_layer(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiContext* c = DATA_PTR(self);
    BiLayer* layer = DATA_PTR(obj);

    bi_add_layer(c,layer);

    return self;
}

static mrb_value mrb_bi_remove_layer(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiContext* c = DATA_PTR(self);
    BiLayer* layer = DATA_PTR(obj);

    bi_remove_layer(c,layer);

    return self;
}

static mrb_value mrb_bi_add_update_callback(mrb_state *mrb, mrb_value self)
{
    mrb_value callback;
    mrb_get_args(mrb, "o", &callback );

    BiContext *context = DATA_PTR(self);
    int i = context->on_update_callbacks_size;
    context->on_update_callbacks[i].callback = _update_callback_;
    context->on_update_callbacks[i].userdata = mrb_ptr(callback);
    context->on_update_callbacks_size += 1;

    return self;
}

void mrb_mruby_bi_core_gem_init(mrb_state* mrb)
{
  struct RClass *bi;
  bi = mrb_define_class(mrb, "Bi", mrb->object_class);
  MRB_SET_INSTANCE_TT(bi, MRB_TT_DATA);

  mrb_define_method(mrb, bi, "initialize", mrb_bi_initialize, MRB_ARGS_REQ(5));

  mrb_define_method(mrb, bi, "w", mrb_BiContext_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "h", mrb_BiContext_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "now", mrb_bi_now, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "fps", mrb_bi_fps, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "start_run_loop", mrb_bi_start_run_loop, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "debug=", mrb_BiContext_set_debug, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "debug", mrb_BiContext_get_debug, MRB_ARGS_NONE());

  mrb_define_method(mrb, bi, "set_title", mrb_bi_set_title, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, bi, "add_layer", mrb_bi_add_layer, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "remove_layer", mrb_bi_remove_layer, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, bi, "add_update_callback", mrb_bi_add_update_callback, MRB_ARGS_REQ(1));

#define DONE mrb_gc_arena_restore(mrb, 0)
  mrb_init_node(mrb,bi); DONE;
  mrb_init_texture_image(mrb,bi); DONE;
  mrb_init_texture(mrb,bi); DONE;
  mrb_init_timer(mrb,bi); DONE;
  mrb_init_layer(mrb,bi); DONE;
  mrb_init_key(mrb,bi); DONE;
#undef DONE
}

void mrb_mruby_bi_core_gem_final(mrb_state* mrb)
{
}
