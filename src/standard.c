#include "mruby.h"
#include "iup.h"

#include "mruby-iup/gui.h"

static mrb_value
mrb_iup_animated_label(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *animation = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj)) animation = mrb_iup_get_handle(mrb, obj)->handle;
  }
  return mrb_iup_new_handle(mrb, IupAnimatedLabel(animation));
}

static mrb_value
mrb_iup_button(mrb_state *mrb, mrb_value self)
{
  const char *title, *action;
  title = NULL;
  action = NULL;
  mrb_get_args(mrb, "|zz", &title, &action);
  return mrb_iup_new_handle(mrb, IupButton(title, action));
}

static mrb_value
mrb_iup_flat_button(mrb_state *mrb, mrb_value self)
{
  const char *title;
  title = NULL;
  mrb_get_args(mrb, "|z", &title);
  return mrb_iup_new_handle(mrb, IupFlatButton(title));
}

static mrb_value
mrb_iup_drop_button(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *child = NULL;
  if (mrb_get_args(mrb, "|o", &obj))
  {
    if (!mrb_nil_p(obj)) child = mrb_iup_get_handle(mrb, obj)->handle;
  }
  return mrb_iup_new_handle(mrb, IupDropButton(child));
}

static mrb_value
mrb_iup_calendar(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupCalendar());
}

static mrb_value
mrb_iup_canvas(mrb_state *mrb, mrb_value self)
{
  const char *action;
  action = NULL;
  mrb_get_args(mrb, "|z", &action);
  return mrb_iup_new_handle(mrb, IupCanvas(action));
}

void
mrb_init_iup_standard(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "animated_label", mrb_iup_animated_label, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "button", mrb_iup_button, MRB_ARGS_OPT(2));
  mrb_define_module_function(mrb, iup, "flat_button", mrb_iup_flat_button, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "drop_button", mrb_iup_drop_button, MRB_ARGS_OPT(1));

  mrb_define_module_function(mrb, iup, "calendar", mrb_iup_calendar, MRB_ARGS_NONE());

  mrb_define_module_function(mrb, iup, "canvas", mrb_iup_canvas, MRB_ARGS_OPT(1));
}
