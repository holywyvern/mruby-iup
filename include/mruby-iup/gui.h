#ifndef MRUBY_GUI_H
#define MRUBY_GUI_H 1

#include "mruby.h"
#include "mruby/data.h"
#include "iup.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MRB_IUP_USERDATA_HANDLE "__MRB_IUP_USERDATA_HANDLE__"

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

void
mrb_iup_handle_push_child(mrb_state *mrb, mrb_value self, Ihandle *child);

mrb_value
mrb_iup_new_handle(mrb_state *mrb, Ihandle *handle);

mrb_bool
mrb_iup_handle_p(mrb_value value);

#ifdef __cplusplus
}
#endif

#endif
