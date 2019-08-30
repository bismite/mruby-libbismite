#ifndef __MRUBY_BI_CORE_H__
#define __MRUBY_BI_CORE_H__

#include <stdbool.h>

struct _BiTextureImage;
typedef struct _BiTextureImage BiTextureImage;

extern mrb_value create_bi_texture_image(mrb_state *mrb, BiTextureImage* texture_image);
extern mrb_value create_bi_texture_image_from_buffer(mrb_state *mrb, void* buffer, int size, bool antialias);

#endif
