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

void
mrb_iup_handle_push_child(mrb_state *mrb, mrb_value self, Ihandle *child);

mrb_value
mrb_iup_new_handle(mrb_state *mrb, Ihandle *handle);

#ifdef __cplusplus
}
#endif

#endif
