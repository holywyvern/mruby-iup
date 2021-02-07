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
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupRadio(handle));
}

static mrb_value
mrb_iup_normalizer(mrb_state *mrb, mrb_value self)
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
    return mrb_iup_new_handle(mrb, IupNormalizerv(items));
  }
}

static mrb_value
mrb_iup_frame(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupFrame(handle));
}

static mrb_value
mrb_iup_flat_frame(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupFlatFrame(handle));
}

static mrb_value
mrb_iup_tabs(mrb_state *mrb, mrb_value self)
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
    return mrb_iup_new_handle(mrb, IupTabsv(items));
  }
}

static mrb_value
mrb_iup_flat_tabs(mrb_state *mrb, mrb_value self)
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
    return mrb_iup_new_handle(mrb, IupFlatTabsv(items));
  }
}

static mrb_value
mrb_iup_background_box(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupBackgroundBox(handle));
}

static mrb_value
mrb_iup_scroll_box(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupScrollBox(handle));
}

static mrb_value
mrb_iup_flat_scroll_box(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupFlatScrollBox(handle));
}

static mrb_value
mrb_iup_detach_box(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupDetachBox(handle));
}

static mrb_value
mrb_iup_expander(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupExpander(handle));
}

static mrb_value
mrb_iup_sbox(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj))
    {
      handle = mrb_iup_get_handle(mrb, obj)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupSbox(handle));
}

static mrb_value
mrb_iup_split(mrb_state *mrb, mrb_value self)
{
  mrb_value o1, o2;
  Ihandle *h1 = NULL;
  Ihandle *h2 = NULL;
  mrb_int argc = mrb_get_args(mrb, "|oo", &o1, &o2);
  if (argc)
  {
    if (!mrb_nil_p(o1))
    {
      h1 = mrb_iup_get_handle(mrb, o1)->handle;
    }
  }
  if (argc < 2)
  {
    if (!mrb_nil_p(o2))
    {
      h2 = mrb_iup_get_handle(mrb, o2)->handle;
    }
  }
  return mrb_iup_new_handle(mrb, IupSplit(h1, h2));
}

void
mrb_init_iup_containers(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "fill", mrb_iup_fill, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "space", mrb_iup_space, MRB_ARGS_NONE());

  mrb_define_module_function(mrb, iup, "cbox", mrb_iup_cbox, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "gridbox", mrb_iup_gridbox, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "multibox", mrb_iup_multibox, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "hbox", mrb_iup_hbox, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "vbox", mrb_iup_vbox, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "zbox", mrb_iup_zbox, MRB_ARGS_REST());

  mrb_define_module_function(mrb, iup, "radio", mrb_iup_radio, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "normalizer", mrb_iup_normalizer, MRB_ARGS_REST());

  mrb_define_module_function(mrb, iup, "frame", mrb_iup_frame, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "flat_frame", mrb_iup_flat_frame, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "tabs", mrb_iup_tabs, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "flat_tabs", mrb_iup_flat_tabs, MRB_ARGS_REST());

  mrb_define_module_function(mrb, iup, "background_box", mrb_iup_background_box, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "scroll_box", mrb_iup_scroll_box, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "flat_scroll_box", mrb_iup_flat_scroll_box, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "detach_box", mrb_iup_detach_box, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "expander", mrb_iup_expander, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "sbox", mrb_iup_sbox, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "split", mrb_iup_split, MRB_ARGS_OPT(2));
}
