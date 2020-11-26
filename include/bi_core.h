#ifndef __MRUBY_BI_CORE_H__
#define __MRUBY_BI_CORE_H__

#include <stdbool.h>

struct _BiTexture;
typedef struct _BiTexture BiTexture;

extern mrb_value create_bi_texture(mrb_state *mrb, BiTexture* texture);
extern mrb_value create_bi_texture_from_memory(mrb_state *mrb, void* buffer, int size, bool antialias);

#endif
