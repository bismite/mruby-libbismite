#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <time.h>

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

// Bi class
static struct mrb_data_type const mrb_bi_data_type = { "Bi", mrb_free };

static mrb_value mrb_bi_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_int width, height, fps;
    mrb_value title_obj;
    mrb_get_args(mrb, "iiiS", &width, &height, &fps, &title_obj );
    const char* title_str = mrb_string_value_cstr(mrb,&title_obj);

    BiContext* c = mrb_malloc(mrb,sizeof(BiContext));
    bi_init_context(c, width, height, fps, title_str, NULL);
    c->userdata = mrb; // XXX: hold mrb_state

    DATA_PTR(self) = c;
    DATA_TYPE(self) = &mrb_bi_data_type;

    return self;
}

static mrb_value mrb_bi_w(mrb_state *mrb, mrb_value self)
{
    // XXX: error check
    BiContext* c = DATA_PTR(self);
    return mrb_fixnum_value(c->w);
}

static mrb_value mrb_bi_h(mrb_state *mrb, mrb_value self)
{
    // XXX: error check
    BiContext* c = DATA_PTR(self);
    return mrb_fixnum_value(c->h);
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

static mrb_value mrb_bi_set_debug(mrb_state *mrb, mrb_value self)
{
    mrb_bool debug;
    mrb_get_args(mrb, "b", &debug );

    // TODO: error check?
    BiContext* c = DATA_PTR(self);
    c->debug = debug;

    return self;
}

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


//
// Timer
//

static mrb_value mrb_bi_add_timer(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiContext* c = DATA_PTR(self);
    BiTimer* timer = DATA_PTR(obj);

    bi_add_timer(c,timer);

    return self;
}

static mrb_value mrb_bi_remove_timer(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj );

    // TODO: error check
    BiContext* c = DATA_PTR(self);
    BiTimer* timer = DATA_PTR(obj);

    bi_remove_timer(c,timer);

    return self;
}

void mrb_mruby_bi_core_gem_init(mrb_state* mrb)
{
  struct RClass *bi;
  bi = mrb_define_class(mrb, "Bi", mrb->object_class);
  MRB_SET_INSTANCE_TT(bi, MRB_TT_DATA);

  mrb_define_method(mrb, bi, "initialize", mrb_bi_initialize, MRB_ARGS_REQ(4));

  mrb_define_method(mrb, bi, "w", mrb_bi_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "h", mrb_bi_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "now", mrb_bi_now, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "fps", mrb_bi_fps, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "start_run_loop", mrb_bi_start_run_loop, MRB_ARGS_NONE());
  mrb_define_method(mrb, bi, "debug=", mrb_bi_set_debug, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, bi, "set_title", mrb_bi_set_title, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, bi, "add_layer", mrb_bi_add_layer, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "remove_layer", mrb_bi_remove_layer, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, bi, "add_timer", mrb_bi_add_timer, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, bi, "remove_timer", mrb_bi_remove_timer, MRB_ARGS_REQ(1));

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
