#include "mruby.h"
#include "iup.h"
#include "mruby/gui.h"

#include "string.h"

#include "iup_scintilla.h"
#include "iupfiledlg.h"

static mrb_value
mrb_iup_dialog(mrb_state *mrb, mrb_value self)
{
  Ihandle *ptr = NULL;
  mrb_value child = mrb_nil_value();
  mrb_get_args(mrb, "|o", &child);
  if (!mrb_nil_p(child))
  {
    ptr = mrb_iup_get_handle(mrb, child)->handle;
  }
  return mrb_iup_new_handle(mrb, IupDialog(ptr));
}

static mrb_value
mrb_iup_file_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupFileDlg());
}

static mrb_value
mrb_iup_message_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupMessageDlg());
}

static mrb_value
mrb_iup_color_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupColorDlg());
}

static mrb_value
mrb_iup_font_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupFontDlg());
}

static mrb_value
mrb_iup_progress_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupProgressDlg());
}

static mrb_value
mrb_iup_scintilla_dialog(mrb_state *mrb, mrb_value self)
{
  return mrb_iup_new_handle(mrb, IupScintillaDlg());
}

void
mrb_init_iup_dialogs(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "dialog", mrb_iup_dialog, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "file_dialog", mrb_iup_file_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "message_dialog", mrb_iup_message_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "color_dialog", mrb_iup_color_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "font_dialog", mrb_iup_font_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "progress_dialog", mrb_iup_progress_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "scintilla_dialog", mrb_iup_scintilla_dialog, MRB_ARGS_NONE());
}