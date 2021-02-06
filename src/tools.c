#include "mruby.h"
#include "mruby/array.h"
#include "mruby/gui.h"
#include "iup.h"
#include "iupim.h"
#include "iup_config.h"

#include "string.h"

static mrb_value
mrb_iup_image(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_get_args(mrb, "z", &name);
  return mrb_iup_new_handle(mrb, IupLoadImage(name));
}

static mrb_value
mrb_iup_clipboard(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupClipboard());
}

static mrb_value
mrb_iup_user(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupUser());
}

static mrb_value
mrb_iup_help(mrb_state *mrb, mrb_value self)
{
  const char *url;
  mrb_get_args(mrb, "z", &url);
  return mrb_fixnum_value(IupHelp(url));
}

static mrb_value
mrb_iup_config(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupConfig());
}

static mrb_value
mrb_iup_alarm(mrb_state *mrb, mrb_value self)
{
  const char *title, *message, *b1, *b2, *b3;
  mrb_int argc = mrb_get_args(mrb, "zzz|zz", &title, &message, &b1, &b2, &b3);
  if (argc < 4) b2 = NULL;
  if (argc < 5) b3 = NULL;
  return mrb_fixnum_value(IupAlarm(title, message, b1, b2, b3));
}

static mrb_value
mrb_iup_get_file(mrb_state *mrb, mrb_value self)
{
  char result[4097];
  const char *filename;
  mrb_value value;
  mrb_get_args(mrb, "z", &filename);
  value = mrb_ary_new(mrb);
  strcpy(result, filename);
  mrb_ary_push(mrb, value, mrb_fixnum_value(IupGetFile(result)));
  mrb_ary_push(mrb, value, mrb_str_new_cstr(mrb, result));
  return value;
}

static mrb_value
mrb_iup_get_color(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  unsigned char r, g, b;
  mrb_get_args(mrb, "ii", &x, &y);
  if (IupGetColor(x, y, &r, &g, &b))
  {
    mrb_value values[] = {
      mrb_fixnum_value(r),
      mrb_fixnum_value(g),
      mrb_fixnum_value(b),
      mrb_fixnum_value(255)
    };
    return mrb_ary_new_from_values(mrb, 4, values);
  }
  return mrb_nil_value();
}

void
mrb_init_iup_tools(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "alarm", mrb_iup_alarm, MRB_ARGS_REQ(3)|MRB_ARGS_OPT(2));
  mrb_define_module_function(mrb, iup, "get_file", mrb_iup_get_file, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, iup, "get_color", mrb_iup_get_color, MRB_ARGS_REQ(2));

  mrb_define_module_function(mrb, iup, "help", mrb_iup_help, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, iup, "image", mrb_iup_image, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, iup, "clipboard", mrb_iup_clipboard, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "user", mrb_iup_user, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "config", mrb_iup_config, MRB_ARGS_NONE());
}