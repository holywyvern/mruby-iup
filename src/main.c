/*
** mrb_example.c - Example class
**
** Copyright (c) Ramiro Rojo 2021
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "mruby/error.h"
#include "mruby/gui.h"

#include "iup.h"
#include "iup_scintilla.h"
#include "iupfiledlg.h"

#include <string.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

static mrb_bool iup_open = FALSE;

static int    iup_argc;
static char **iup_argv;

static mrb_value
mrb_iup_open(mrb_state *mrb, mrb_value self)
{
  int arena;
  mrb_int m_argc;
  mrb_value *m_argv;
  if (iup_open) return self;

  mrb_get_args(mrb, "*", &m_argv, &m_argc);
  iup_argc = m_argc;
  arena = mrb_gc_arena_save(mrb);
  iup_argv = mrb_malloc(mrb, sizeof(char *) * m_argc);
  for (int i = 0; i < iup_argc; ++i)
  {
    char *dest, *src;
    src = mrb_str_to_cstr(mrb, m_argv[i]);
    dest = mrb_malloc(mrb, strlen(src) + 1);
    strcpy(dest, src);
    iup_argv[i] = dest;
  }
  if (IupOpen(&iup_argc, &iup_argv) == IUP_ERROR)
  {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to initialize IUP Library");
  }
  mrb_gc_arena_restore(mrb, arena);
  iup_open = TRUE;
  return self;
}

static mrb_value
mrb_iup_loop(mrb_state *mrb, mrb_value self)
{
  IupMainLoop();
  return self;
}

static mrb_value
mrb_iup_close(mrb_state *mrb, mrb_value self)
{
  if (!iup_open) return self;

  IupClose();
  for (int i = 0; i < iup_argc; ++i)
  {
    mrb_free(mrb, iup_argv[i]);
  }
  mrb_free(mrb, iup_argv);
  iup_open = FALSE;
  return self;
}

static mrb_value
mrb_iup_version(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, IupVersion());
}

static mrb_value
new_handle(mrb_state *mrb, Ihandle *handle)
{
  struct RClass *iup = mrb_module_get(mrb, "IUP");
  mrb_value result = mrb_obj_new(mrb, mrb_class_get_under(mrb, iup, "Handle"), 0, NULL);
  mrb_iup_handle *ptr = DATA_PTR(result);
  ptr->handle = handle;
  return result;
}

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
  return new_handle(mrb, IupDialog(ptr));
}

static mrb_value
mrb_iup_file_dialog(mrb_state *mrb, mrb_value self)
{
  return new_handle(mrb, IupFileDlg());
}

static mrb_value
mrb_iup_message_dialog(mrb_state *mrb, mrb_value self)
{
  return new_handle(mrb, IupMessageDlg());
}

static mrb_value
mrb_iup_color_dialog(mrb_state *mrb, mrb_value self)
{
  return new_handle(mrb, IupColorDlg());
}

static mrb_value
mrb_iup_font_dialog(mrb_state *mrb, mrb_value self)
{
  return new_handle(mrb, IupFontDlg());
}

static mrb_value
mrb_iup_progress_dialog(mrb_state *mrb, mrb_value self)
{
  return new_handle(mrb, IupProgressDlg());
}

static mrb_value
mrb_iup_scintilla_dialog(mrb_state *mrb, mrb_value self)
{
  return new_handle(mrb, IupScintillaDlg());
}

struct RClass *
mrb_init_iup_handle(mrb_state *mrb, struct RClass *iup);

#define IUP_CONST(n) mrb_define_const(mrb, iup, #n, mrb_fixnum_value(IUP_##n));

void
mrb_mruby_iup_gem_init(mrb_state *mrb)
{
  struct RClass *iup;
  iup = mrb_define_module(mrb, "IUP");

  mrb_define_module_function(mrb, iup, "open", mrb_iup_open, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "loop", mrb_iup_loop, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "close", mrb_iup_close, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "version", mrb_iup_version, MRB_ARGS_NONE());

  mrb_init_iup_handle(mrb, iup);

  mrb_define_module_function(mrb, iup, "dialog", mrb_iup_dialog, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, iup, "file_dialog", mrb_iup_file_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "message_dialog", mrb_iup_message_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "color_dialog", mrb_iup_color_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "font_dialog", mrb_iup_font_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "progress_dialog", mrb_iup_progress_dialog, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "scintilla_dialog", mrb_iup_scintilla_dialog, MRB_ARGS_NONE());

  IUP_CONST(NOERROR);
  IUP_CONST(INVALID);

  IUP_CONST(LEFT);
  IUP_CONST(CENTER);
  IUP_CONST(RIGHT);
  IUP_CONST(MOUSEPOS);
  IUP_CONST(CENTERPARENT);
  IUP_CONST(CURRENT);
  IUP_CONST(LEFTPARENT);
  IUP_CONST(RIGHTPARENT);
  IUP_CONST(TOP);
  IUP_CONST(BOTTOM);
  IUP_CONST(TOPPARENT);
  IUP_CONST(BOTTOMPARENT);

  DONE;
}

void
mrb_mruby_iup_gem_final(mrb_state *mrb)
{
}

