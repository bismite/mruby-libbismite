#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <bi/node.h>
#include <bi/ext/label.h>
#include <stdlib.h>

// Bi::Label class
static void label_free(mrb_state *mrb, void *p)
{
  BiNode* node = (BiNode*)p;
  if(node != NULL){
    BiNode* label = NULL;
    if(node->children.size>0){
      label = bi_node_child_at(node,0);
    }
    if (NULL != label) {
      for(int i=0;i<label->children.size;i++){
        BiNode* n = label->children.objects[i];
        free(n);
      }
      free(label->children.objects);
      free(label);
    }
    free(node->children.objects);
    mrb_free(mrb, node);
  }
}

static struct mrb_data_type const mrb_label_data_type = { "Label", label_free };

static mrb_value mrb_label_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value font_obj;
  mrb_get_args(mrb, "o", &font_obj);
  BiFont *font = DATA_PTR(font_obj);
  BiLabel* label = bi_label_init(mrb_malloc(mrb, sizeof(BiLabel)),font);
  mrb_data_init(self, label, &mrb_label_data_type);
  label->node.userdata = mrb_ptr(self);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@font"), font_obj);
  return self;
}

static mrb_value mrb_label_set_text(mrb_state *mrb, mrb_value self)
{
  mrb_value text;
  mrb_get_args(mrb, "S", &text );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@text"), text);
  BiLabel* n = DATA_PTR(self);
  bi_label_set_text(n,mrb_string_value_cstr(mrb,&text));
  return self;
}

static mrb_value mrb_label_set_text_color(mrb_state *mrb, mrb_value self)
{
  mrb_int r,g,b,a;
  mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
  BiLabel* label = DATA_PTR(self);
  bi_label_set_modulate_color(label, RGBA(r,g,b,a) );
  return self;
}

static mrb_value mrb_label_set_text_color_with_range(mrb_state *mrb, mrb_value self)
{
  mrb_int start,end;
  mrb_int rm,gm,bm,am;
  mrb_int rt,gt,bt,at;
  mrb_get_args(mrb, "iiiiiiiiii", &start, &end, &rm,&gm,&bm,&am, &rt,&gt,&bt,&at );
  BiLabel* label = DATA_PTR(self);
  BiColor tint = RGBA(rt,gt,bt,at);
  BiColor modulate = RGBA(rm,gm,bm,am);
  bi_label_set_color_with_range(label,start,end,tint,modulate);
  return self;
}

static mrb_value mrb_label_anchor_reposition(mrb_state *mrb, mrb_value self)
{
  BiLabel* label = DATA_PTR(self);
  bi_label_anchor_reposition(label);
  return self;
}

void mrb_init_label(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *node = mrb_class_get_under(mrb, bi, "Node");
  struct RClass *label = mrb_define_class_under(mrb, bi, "Label", node);
  MRB_SET_INSTANCE_TT(label, MRB_TT_DATA);
  mrb_define_method(mrb, label, "initialize", mrb_label_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, label, "set_text", mrb_label_set_text, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, label, "set_text_color", mrb_label_set_text_color, MRB_ARGS_REQ(4)); // r,g,b,a
  mrb_define_method(mrb, label, "set_text_color_with_range", mrb_label_set_text_color_with_range, MRB_ARGS_REQ(10) ); // start,end,r,g,b,a,r,g,b,a
  mrb_define_method(mrb, label, "anchor_reposition", mrb_label_anchor_reposition, MRB_ARGS_NONE());
}
