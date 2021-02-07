#include "mruby.h"
#include "mruby/array.h"
#include "mruby/hash.h"
#include "mruby/string.h"
#include "mruby/variable.h"

#include "iup.h"

#include "mruby-iup/gui.h"

#include "string.h"

#define CHILDREN mrb_intern_cstr(mrb, "#_children_")
#define PROCS mrb_intern_cstr(mrb, "#_procs_")
#define ATTRS mrb_intern_cstr(mrb, "#_attrs_")
#define HANDLES mrb_intern_cstr(mrb, "#_attrs_")

static void
resize_elements(mrb_state *mrb, mrb_iup_handle *handle)
{
  if (handle->values.len >= handle->values.capa)
  {
    handle->values.capa *= 2;
    handle->values.elements = (char **)mrb_realloc(
      mrb, handle->values.elements, sizeof(char *) * handle->values.capa
    );
  }
}

static char *
push_str_value(mrb_state *mrb, mrb_iup_handle *handle, char *src)
{
  char *dst;
  resize_elements(mrb, handle);
  dst = mrb_malloc(mrb, strlen(src) + 1);
  strcpy(dst, src);
  handle->values.elements[handle->values.len] = dst;
  handle->values.len++;
  return dst;
}

int
mrb_iup_ruby_callback(Ihandle *handle)
{
  mrb_state *mrb = IupGetGlobal("MRB");
  // TODO: Find a way to use handles
  return IUP_NOERROR;
}

static Icallback
get_callback(mrb_state *mrb, const char *name, mrb_iup_handle *handle, mrb_value procs, mrb_value block)
{
  IupSetGlobal("MRB", mrb);
  return mrb_iup_ruby_callback;
}

static mrb_value
mrb_set_attribute(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  const char *name;
  mrb_value value;
  mrb_get_args(mrb, "zo", &name, &value);
  data = mrb_iup_get_handle(mrb, self);
  if (mrb_nil_p(value))
  {
    IupResetAttribute(data->handle, name);
  }
  else if (mrb_fixnum_p(value))
  {
    IupSetInt(data->handle, name, mrb_fixnum(value));
  }
  else if (mrb_float_p(value))
  {
    IupSetDouble(data->handle, name, mrb_float(value));
  }
  else if (mrb_array_p(value))
  {
    mrb_int r, g, b, a;
    r = mrb_int(mrb, mrb_ary_entry(value, 0));
    g = mrb_int(mrb, mrb_ary_entry(value, 0));
    b = mrb_int(mrb, mrb_ary_entry(value, 0));
    a = mrb_int(mrb, mrb_ary_entry(value, 0));
    IupSetRGBA(data->handle, name, r, g, b, a);
  }
  else if (mrb_iup_handle_p(value))
  {
    mrb_iup_handle *handle = mrb_iup_get_handle(mrb, value);
    IupSetAttributeHandle(data->handle, name, handle->handle);
    mrb_hash_set(
      mrb, mrb_iv_get(mrb, self, ATTRS), mrb_str_new_cstr(mrb, name), value
    );
  }
  else if (mrb_true_p(value))
  {
    IupSetAttribute(data->handle, name, "YES"); 
  }
  else if (mrb_false_p(value))
  {
    IupSetAttribute(data->handle, name, "NO"); 
  }
  else if (mrb_proc_p(value))
  {
    mrb_value procs = mrb_iv_get(mrb, self, PROCS);
    mrb_hash_set(mrb, procs, mrb_str_new_cstr(mrb, name), value);
    IupSetCallback(data->handle, name, get_callback(mrb, name, data, procs, value));
  }
  else
  {
    char *v = push_str_value(mrb, data, mrb_str_to_cstr(mrb, mrb_funcall(mrb, value, "to_s", 0)));
    IupSetAttribute(data->handle, name, v);
  }
  return self;
}

static mrb_value
mrb_get_attribute(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_iup_handle *data;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_str_new_cstr(mrb, IupGetAttribute(data->handle, name));
}

static mrb_value
mrb_get_int(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  const char *name;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_fixnum_value(IupGetInt(data->handle, name));
}

static mrb_value
mrb_get_ints(mrb_state *mrb, mrb_value self)
{
  int a, b;
  mrb_iup_handle *data;
  const char *name;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  IupGetIntInt(data->handle, name, &a, &b);
  {
    mrb_value values[] = {
      mrb_fixnum_value(a),
      mrb_fixnum_value(b),
    };
    return mrb_ary_new_from_values(mrb, 2, values);
  }
}

static mrb_value
mrb_get_int2(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  const char *name;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_fixnum_value(IupGetInt2(data->handle, name));
}

static mrb_value
mrb_get_float(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  const char *name;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_float_value(mrb, IupGetDouble(data->handle, name));
}

static mrb_value
mrb_get_color(mrb_state *mrb, mrb_value self)
{
  unsigned char r, g, b, a;
  mrb_iup_handle *data;
  const char *name;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  IupGetRGBA(data->handle, name, &r, &g, &b, &a);
  {
    mrb_value values[] = {
      mrb_fixnum_value(r),
      mrb_fixnum_value(g),
      mrb_fixnum_value(b),
      mrb_fixnum_value(a),
    };
    return mrb_ary_new_from_values(mrb, 4, values);
  }
}

static mrb_value
mrb_get_handle(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  const char *name;
  mrb_get_args(mrb, "z", &name);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_iup_new_handle(mrb, IupGetAttributeHandle(data->handle, name));
}

void
mrb_init_iup_attributes(mrb_state *mrb, struct RClass *handle)
{
  mrb_define_method(mrb, handle, "[]", mrb_get_attribute, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "[]=", mrb_set_attribute, MRB_ARGS_REQ(2));

  mrb_define_method(mrb, handle, "set_attribute", mrb_set_attribute, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, handle, "get_string", mrb_get_attribute, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_int", mrb_get_int, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_ints", mrb_get_ints, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_int2", mrb_get_int2, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_float", mrb_get_float, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_color", mrb_get_color, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_handle", mrb_get_handle, MRB_ARGS_REQ(1));
}
