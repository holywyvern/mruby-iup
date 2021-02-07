#include "mruby.h"

#include "iup.h"

#include "mruby-iup/gui.h"

static mrb_value
mrb_i18n_get(mrb_state *mrb, mrb_value self)
{
  const char *name, *value;
  mrb_get_args(mrb, "zz", &name);
  value = IupGetLanguageString(name);
  return mrb_str_new_cstr(mrb, value ? value : name);
}

static mrb_value
mrb_i18n_set(mrb_state *mrb, mrb_value self)
{
  const char *name, *value;
  mrb_get_args(mrb, "zz", &name, &value);
  IupStoreLanguageString(name, value);
  return mrb_str_new_cstr(mrb, value);
}

static mrb_value
mrb_i18n_load_pack(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Ihandle *handle;
  mrb_get_args(mrb, "o", &obj);
  handle = mrb_iup_get_handle(mrb, obj)->handle;
  IupSetLanguagePack(handle);
  return self;
}

void
mrb_init_iup_languages(mrb_state *mrb, struct RClass *iup)
{
  struct RClass *i18n = mrb_define_module_under(mrb, iup, "I18n");

  mrb_define_module_function(mrb, i18n, "[]", mrb_i18n_get, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, i18n, "[]=", mrb_i18n_set, MRB_ARGS_REQ(2));

  mrb_define_module_function(mrb, i18n, "load_pack", mrb_i18n_load_pack, MRB_ARGS_REQ(1));
}