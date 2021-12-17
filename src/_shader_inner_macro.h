#ifndef _SHADER_INNER_MACRO_H_
#define _SHADER_INNER_MACRO_H_

#define SET_SHADER_ATTRIBUTE(CLASS,NAME) \
  mrb_int index; \
  mrb_float value; \
  mrb_get_args(mrb, "if", &index, &value ); \
  CLASS *ptr = DATA_PTR(self); \
  if(0<=index&&index<4) ptr->NAME[index]=value;

static inline void set_shader(mrb_state* mrb, mrb_value self,const char* ivname,BiShader** shader_p,mrb_value shader_obj)
{
  struct RClass *shader_class = mrb_class_get_under(mrb,mrb_class_get(mrb,"Bi"),"Shader");
  if( mrb_obj_is_kind_of(mrb, shader_obj, shader_class) ) {
    BiShader* shader = DATA_PTR(shader_obj);
    *shader_p = shader;
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,ivname), shader_obj);
  }else{
    *shader_p = NULL;
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,ivname), mrb_nil_value() );
  }
}

#endif
