#include "mruby.h"
#include "iup.h"

#define IUP_CONST(n) mrb_define_const(mrb, iup, #n, mrb_fixnum_value(IUP_##n));

void
mrb_init_iup_constants(mrb_state *mrb, struct RClass *iup)
{
  IUP_CONST(ERROR);
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
}
