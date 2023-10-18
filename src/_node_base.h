#ifndef __MRUBY_LIBBISMITE_NODE_BASE_H__
#define __MRUBY_LIBBISMITE_NODE_BASE_H__

inline static mrb_value _iv_children_(mrb_state *mrb, mrb_value self)
{
  mrb_sym iv_name = MRB_IVSYM(_children_);
  mrb_value v = mrb_iv_get(mrb,self,iv_name);
  if( mrb_nil_p(v) ) {
    v = mrb_ary_new(mrb);
    mrb_iv_set(mrb,self,iv_name,v);
  }
  return v;
}

#endif // __MRUBY_LIBBISMITE_NODE_BASE_H__
