#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <bi/framebuffer.h>
#include <bi/texture.h>
#include <bi_core.h>

static struct mrb_data_type const mrb_bi_framebuffer_data_type = { "Framebuffer", mrb_free };

static mrb_value mrb_bi_framebuffer_init(mrb_state *mrb, mrb_value self)
{
  mrb_int w,h,num=1;
  mrb_get_args(mrb, "ii|i", &w,&h,&num );
  BiFramebuffer *fb = mrb_malloc(mrb, sizeof(BiFramebuffer));
  bi_framebuffer_init_with_texture_num(fb,w,h,num);
  mrb_data_init(self, fb, &mrb_bi_framebuffer_data_type);
  return self;
}

#define GET(x) return(mrb_fixnum_value(((BiFramebuffer*)DATA_PTR(self))->x));
static mrb_value mrb_bi_framebuffer_get_w(mrb_state *mrb, mrb_value self) { GET(w); }
static mrb_value mrb_bi_framebuffer_get_h(mrb_state *mrb, mrb_value self) { GET(h); }
static mrb_value mrb_bi_framebuffer_set_w(mrb_state *mrb, mrb_value self) {
  mrb_int w;
  mrb_get_args(mrb, "i", &w );
  BiFramebuffer* fb = DATA_PTR(self);
  fb->w = w;
  return mrb_fixnum_value(w);
}
static mrb_value mrb_bi_framebuffer_set_h(mrb_state *mrb, mrb_value self) {
  mrb_int h;
  mrb_get_args(mrb, "i", &h );
  BiFramebuffer* fb = DATA_PTR(self);
  fb->h = h;
  return mrb_fixnum_value(h);
}

static mrb_value mrb_bi_framebuffer_clear(mrb_state *mrb, mrb_value self)
{
  mrb_int r,g,b,a;
  mrb_get_args(mrb, "iiii", &r,&g,&b,&a );
  BiFramebuffer* fb = DATA_PTR(self);
  bi_framebuffer_clear(fb,r,g,b,a);
  return self;
}

static mrb_value mrb_bi_framebuffer_to_texture(mrb_state *mrb, mrb_value self)
{
  mrb_int num=0;
  mrb_get_args(mrb, "|i", &num );
  BiFramebuffer* fb = DATA_PTR(self);
  BiTexture* tex = mrb_malloc(mrb,sizeof(BiTexture));
  bi_texture_init_with_texture_id(tex,fb->w,fb->h,fb->texture_ids[num] );
  return create_bi_texture(mrb, tex);
}

static mrb_value mrb_bi_framebuffer_save_png(mrb_state *mrb, mrb_value self)
{
  char *name;
  mrb_get_args(mrb, "z", &name );
  BiFramebuffer* fb = DATA_PTR(self);
  bi_framebuffer_save_png_image(fb,name);
  return self;
}

//
// gem
//
void mrb_init_bi_framebuffer(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *framebuffer = mrb_define_class_under(mrb, bi, "Framebuffer", mrb->object_class);
  MRB_SET_INSTANCE_TT(framebuffer, MRB_TT_DATA);
  mrb_define_method(mrb, framebuffer, "initialize", mrb_bi_framebuffer_init, MRB_ARGS_REQ(2)|MRB_ARGS_OPT(1)); // w,h,num
  mrb_define_method(mrb, framebuffer, "w", mrb_bi_framebuffer_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, framebuffer, "h", mrb_bi_framebuffer_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, framebuffer, "w=", mrb_bi_framebuffer_set_w, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, framebuffer, "h=", mrb_bi_framebuffer_set_h, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, framebuffer, "_clear_", mrb_bi_framebuffer_clear, MRB_ARGS_REQ(4)); // r,g,b,a
  mrb_define_method(mrb, framebuffer, "to_texture", mrb_bi_framebuffer_to_texture, MRB_ARGS_OPT(1)); // num
  mrb_define_method(mrb, framebuffer, "save_png", mrb_bi_framebuffer_save_png, MRB_ARGS_OPT(1)); // name
}
