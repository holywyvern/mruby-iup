#ifndef MRUBY_GUI_H
#define MRUBY_GUI_H 1

#include "mruby.h"
#include "mruby/data.h"
#include "iup.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mrb_iup_handle mrb_iup_handle;

struct mrb_iup_handle
{
  mrb_state *mrb;
  Ihandle *handle;
  mrb_value self;
  struct
  {
    size_t len;
    size_t capa;
    char **elements;
  } values;
};

mrb_iup_handle *
mrb_iup_get_handle(mrb_state *mrb, mrb_value self);

static mrb_value
mrb_iup_new_handle(mrb_state *mrb, Ihandle *handle)
{
  if (!handle) return mrb_nil_value();
  {
    struct RClass *iup = mrb_module_get(mrb, "IUP");
    mrb_value result = mrb_obj_new(mrb, mrb_class_get_under(mrb, iup, "Handle"), 0, NULL);
    mrb_iup_handle *ptr = (mrb_iup_handle *)DATA_PTR(result);
    ptr->handle = handle;
    return result;
  }
}

#ifdef __cplusplus
}
#endif

#endif
