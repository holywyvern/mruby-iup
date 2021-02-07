#include "mruby.h"
#include "iup.h"

#include "mruby-iup/gui.h"

static mrb_value
mrb_iup_fill(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupFill());
}

static mrb_value
mrb_iup_space(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupSpace());
}

static mrb_value
mrb_iup_cbox(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_get_args(mrb, "*", &values, &size);
  {
    Ihandle *items[size + 1];
    for (mrb_int i = 0; i < size; ++i)
    {
      items[i] = mrb_iup_get_handle(mrb, values[i])->handle;
    }
    items[size] = NULL;
    return mrb_iup_new_handle(mrb, IupCboxv(items));
  }
}

static mrb_value
mrb_iup_gridbox(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_get_args(mrb, "*", &values, &size);
  {
    Ihandle *items[size + 1];
    for (mrb_int i = 0; i < size; ++i)
    {
      items[i] = mrb_iup_get_handle(mrb, values[i])->handle;
    }
    items[size] = NULL;
    return mrb_iup_new_handle(mrb, IupGridBoxv(items));
  }
}

static mrb_value
mrb_iup_multibox(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_get_args(mrb, "*", &values, &size);
  {
    Ihandle *items[size + 1];
    for (mrb_int i = 0; i < size; ++i)
    {
      items[i] = mrb_iup_get_handle(mrb, values[i])->handle;
    }
    items[size] = NULL;
    return mrb_iup_new_handle(mrb, IupMultiBoxv(items));
  }
}

static mrb_value
mrb_iup_hbox(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_get_args(mrb, "*", &values, &size);
  {
    Ihandle *items[size + 1];
    for (mrb_int i = 0; i < size; ++i)
    {
      items[i] = mrb_iup_get_handle(mrb, values[i])->handle;
    }
    items[size] = NULL;
    return mrb_iup_new_handle(mrb, IupHboxv(items));
  }
}

static mrb_value
mrb_iup_vbox(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_get_args(mrb, "*", &values, &size);
  {
    Ihandle *items[size + 1];
    for (mrb_int i = 0; i < size; ++i)
    {
      items[i] = mrb_iup_get_handle(mrb, values[i])->handle;
    }
    items[size] = NULL;
    return mrb_iup_new_handle(mrb, IupVboxv(items));
  }
}

static mrb_value
mrb_iup_zbox(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_get_args(mrb, "*", &values, &size);
  {
    Ihandle *items[size + 1];
    for (mrb_int i = 0; i < size; ++i)
    {
      items[i] = mrb_iup_get_handle(mrb, values[i])->handle;
    }
    items[size] = NULL;
    return mrb_iup_new_handle(mrb, IupZboxv(items));
  }
}

static mrb_value
mrb_iup_radio(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    handle = mrb_iup_get_handle(mrb, obj)->handle;
  }
  return mrb_iup_new_handle(mrb, IupRadio(handle));
}

void
mrb_init_iup_containers(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "fill", mrb_iup_fill, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "space", mrb_iup_space, MRB_ARGS_NONE());

  mrb_define_module_function(mrb, iup, "cbox", mrb_iup_cbox, MRB_ARGS_ANY());
  mrb_define_module_function(mrb, iup, "gridbox", mrb_iup_gridbox, MRB_ARGS_ANY());
  mrb_define_module_function(mrb, iup, "multibox", mrb_iup_multibox, MRB_ARGS_ANY());
  mrb_define_module_function(mrb, iup, "hbox", mrb_iup_hbox, MRB_ARGS_ANY());
  mrb_define_module_function(mrb, iup, "vbox", mrb_iup_vbox, MRB_ARGS_ANY());
  mrb_define_module_function(mrb, iup, "zbox", mrb_iup_zbox, MRB_ARGS_ANY());

  mrb_define_module_function(mrb, iup, "radio", mrb_iup_radio, MRB_ARGS_OPT(1));
}
