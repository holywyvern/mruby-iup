#include "mruby.h"
#include "iup.h"
#include "mruby-iup/gui.h"

static mrb_value
mrb_iup_message_box(mrb_state *mrb, mrb_value self)
{
  const char *title, *message;
  mrb_get_args(mrb, "zz", &title, &message);
  IupMessage(title, message);
  return self;
}

static mrb_value
mrb_iup_error_box(mrb_state *mrb, mrb_value self)
{
  const char *message;
  mrb_get_args(mrb, "z", &message);
  IupMessageError(NULL, message);
  return self;
}

static mrb_value
mrb_iup_alarm_box(mrb_state *mrb, mrb_value self)
{
  const char *title, *message, *buttons;
  mrb_get_args(mrb, "zzz", &title, &message, &buttons);
  return mrb_fixnum_value(IupMessageAlarm(NULL, title, message, buttons));
}

void
mrb_init_iup_messages(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "message_box", mrb_iup_message_box, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, iup, "error_box", mrb_iup_error_box, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, iup, "alarm_box", mrb_iup_alarm_box, MRB_ARGS_REQ(3));
}