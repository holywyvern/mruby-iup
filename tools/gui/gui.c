/* This file is generated by mruby-mrbgems-template
   We recommend you don't touch this if you aren't familiar with C . */
/* Original from mruby-cli. (c) Terence Lee and Zachary Scott under MIT Lisence.
   ref: https://github.com/hone/mruby-cli/blob/master/mrblib/mruby-cli/setup.rb#L233 */
#include <stdlib.h>
#include <stdio.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/variable.h>

int main(int argc, char *argv[])
{
  mrb_state *mrb = mrb_open();
  mrb_value ARGV = mrb_ary_new_capa(mrb, argc);
  int i;
  int return_value;

  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$0"), mrb_str_new_cstr(mrb, argv[0]));
  for (i = 0; i < argc; i++) {
    mrb_ary_push(mrb, ARGV, mrb_str_new_cstr(mrb, argv[i]));
  }
  mrb_define_global_const(mrb, "ARGV", ARGV);

  mrb_funcall(mrb, mrb_top_self(mrb), "require", 1, mrb_str_new_cstr(mrb, "main.rb"));

  return_value = EXIT_SUCCESS;

  if (mrb->exc) {
    mrb_print_error(mrb);
    return_value = EXIT_FAILURE;
  }
  mrb_close(mrb);

  return return_value;
}
