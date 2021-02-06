#include "mruby.h"
#include "iup.h"

#include "mruby/gui.h"

static mrb_value
mrb_iup_layout_dialog(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_iup_handle *data;
  mrb_get_args(mrb, "o", &obj);
  data = mrb_iup_get_handle(mrb, obj);
  return mrb_iup_new_handle(mrb, IupLayoutDialog(data->handle));
}

static mrb_value
mrb_iup_element_properties_dialog(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_iup_handle *data;
  mrb_get_args(mrb, "o", &obj);
  data = mrb_iup_get_handle(mrb, obj);
  return mrb_iup_new_handle(mrb, IupElementPropertiesDialog(NULL, data->handle));
}

static mrb_value
mrb_iup_globals_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupGlobalsDialog());
}

static mrb_value
mrb_iup_class_info_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupClassInfoDialog(NULL));
}

void
mrb_init_iup_dev(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "layout_dialog", mrb_iup_layout_dialog, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, iup, "element_properties_dialog", mrb_iup_element_properties_dialog, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, iup, "globals_dialog", mrb_iup_globals_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "class_info_dialog", mrb_iup_class_info_dialog, MRB_ARGS_NONE());
}