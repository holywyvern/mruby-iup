#include "mruby.h"
#include "mruby/string.h"

#include "string.h"

#include "iup.h"
#include "iup_scintilla.h"

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
  IupImageLibOpen();
  IupScintillaOpen();
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

void
mrb_init_iup_loop(mrb_state *mrb, struct RClass *iup)
{
  mrb_define_module_function(mrb, iup, "open", mrb_iup_open, MRB_ARGS_REST());
  mrb_define_module_function(mrb, iup, "loop", mrb_iup_loop, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "close", mrb_iup_close, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, iup, "version", mrb_iup_version, MRB_ARGS_NONE());
}