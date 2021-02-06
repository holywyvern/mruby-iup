/*
** mrb_example.c - Example class
**
** Copyright (c) Ramiro Rojo 2021
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "iup.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

void
mrb_init_iup_loop(mrb_state *mrb, struct RClass *iup);

void
mrb_init_iup_handle(mrb_state *mrb, struct RClass *iup);

void
mrb_init_iup_dialogs(mrb_state *mrb, struct RClass *iup);

void
mrb_init_iup_tools(mrb_state *mrb, struct RClass *iup);

void
mrb_init_iup_messages(mrb_state *mrb, struct RClass *iup);

void
mrb_init_iup_dev(mrb_state *mrb, struct RClass *iup);

void
mrb_init_iup_constants(mrb_state *mrb, struct RClass *iup);

void
mrb_mruby_iup_gem_init(mrb_state *mrb)
{
  struct RClass *iup;
  iup = mrb_define_module(mrb, "IUP");

  mrb_init_iup_loop(mrb, iup);
  mrb_init_iup_handle(mrb, iup);
  mrb_init_iup_dialogs(mrb, iup);
  mrb_init_iup_tools(mrb, iup);
  mrb_init_iup_messages(mrb, iup);
  mrb_init_iup_dev(mrb, iup);
  mrb_init_iup_constants(mrb, iup);

  DONE;
}

void
mrb_mruby_iup_gem_final(mrb_state *mrb)
{
}

