#include <mruby.h>
#include <mruby/class.h>
#include <bi/bismite.h>

static mrb_value mrb_bi_version_gl_version(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, (const char*)glGetString(GL_VERSION));
}

static mrb_value mrb_bi_version_gl_renderer(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, (const char*)glGetString(GL_RENDERER));
}

static mrb_value mrb_bi_version_gl_shading_language_version(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

static mrb_value mrb_bi_version_gl_vendor(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, (const char*)glGetString(GL_VENDOR));
}

static mrb_value mrb_bi_version_gl_extensions(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, (const char*)glGetString(GL_EXTENSIONS));
}

static mrb_value mrb_bi_version_glew_version(mrb_state *mrb, mrb_value self)
{
#ifdef GLEW_VERSION
  return mrb_str_new_cstr(mrb, (const char*)glewGetString(GLEW_VERSION));
#else
  return mrb_nil_value();
#endif
}

void mrb_init_bi_version(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *version = mrb_define_module_under(mrb,bi,"Version");

  mrb_define_const(mrb, version, "BI_CORE_MAJOR", mrb_fixnum_value(BISMITE_MAJOR_VERSION));
  mrb_define_const(mrb, version, "BI_CORE_MINOR", mrb_fixnum_value(BISMITE_MINOR_VERSION));
  mrb_define_const(mrb, version, "BI_CORE_PATCH", mrb_fixnum_value(BISMITE_PATCHLEVEL));

#ifdef EMSCRIPTEN
  mrb_define_const(mrb, version, "EMSCRIPTEN_MAJOR", mrb_fixnum_value(__EMSCRIPTEN_major__));
  mrb_define_const(mrb, version, "EMSCRIPTEN_MINOR", mrb_fixnum_value(__EMSCRIPTEN_minor__));
  mrb_define_const(mrb, version, "EMSCRIPTEN_PATCH", mrb_fixnum_value(__EMSCRIPTEN_tiny__));
#else
  mrb_define_const(mrb, version, "EMSCRIPTEN_MAJOR", mrb_nil_value());
  mrb_define_const(mrb, version, "EMSCRIPTEN_MINOR", mrb_nil_value());
  mrb_define_const(mrb, version, "EMSCRIPTEN_PATCH", mrb_nil_value());
#endif

#ifdef __clang_major__
  mrb_define_const(mrb, version, "CLANG_MAJOR", mrb_fixnum_value(__clang_major__));
  mrb_define_const(mrb, version, "CLANG_MINOR", mrb_fixnum_value(__clang_minor__));
  mrb_define_const(mrb, version, "CLANG_PATCH", mrb_fixnum_value(__clang_patchlevel__));
#else
  mrb_define_const(mrb, version, "CLANG_MAJOR", mrb_nil_value());
  mrb_define_const(mrb, version, "CLANG_MINOR", mrb_nil_value());
  mrb_define_const(mrb, version, "CLANG_PATCH", mrb_nil_value());
#endif

#ifdef __GNUC__
  mrb_define_const(mrb, version, "GNUC_MAJOR", mrb_fixnum_value(__GNUC__));
  mrb_define_const(mrb, version, "GNUC_MINOR", mrb_fixnum_value(__GNUC_MINOR__));
  mrb_define_const(mrb, version, "GNUC_PATCH", mrb_fixnum_value(__GNUC_PATCHLEVEL__));
#else
  mrb_define_const(mrb, version, "GNUC_MAJOR", mrb_nil_value());
  mrb_define_const(mrb, version, "GNUC_MINOR", mrb_nil_value());
  mrb_define_const(mrb, version, "GNUC_PATCH", mrb_nil_value());
#endif

  SDL_version compiled;
  SDL_VERSION(&compiled);
  mrb_define_const(mrb, version, "SDL_COMPILED_MAJOR", mrb_fixnum_value(compiled.major));
  mrb_define_const(mrb, version, "SDL_COMPILED_MINOR", mrb_fixnum_value(compiled.minor));
  mrb_define_const(mrb, version, "SDL_COMPILED_PATCH", mrb_fixnum_value(compiled.patch));

  SDL_version linked;
  SDL_GetVersion(&linked);
  mrb_define_const(mrb, version, "SDL_LINKED_MAJOR", mrb_fixnum_value(linked.major));
  mrb_define_const(mrb, version, "SDL_LINKED_MINOR", mrb_fixnum_value(linked.minor));
  mrb_define_const(mrb, version, "SDL_LINKED_PATCH", mrb_fixnum_value(linked.patch));

  // XXX: require initialized OpenGL context.
  mrb_define_class_method(mrb, version, "gl_version", mrb_bi_version_gl_version, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, version, "gl_renderer", mrb_bi_version_gl_renderer, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, version, "gl_shading_language_version", mrb_bi_version_gl_shading_language_version, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, version, "gl_vendor", mrb_bi_version_gl_vendor, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, version, "gl_extensions", mrb_bi_version_gl_extensions, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, version, "glew_version", mrb_bi_version_glew_version, MRB_ARGS_NONE());
}
