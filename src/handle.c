#include "mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "mruby/gui.h"

#include "iup.h"
#include "iupim.h"

#include <string.h>
#include <stdarg.h>

#define CHILDREN mrb_intern_cstr(mrb, "#_children_")
#define PROCS mrb_intern_cstr(mrb, "#_procs_")
#define ATTRS mrb_intern_cstr(mrb, "#_attrs_")

struct iup_known_callbacks
{
  const char *name;
  Icallback   cb;
};

static void
mrb_free_handle(mrb_state *mrb, void *ptr)
{
  if (ptr)
  {
    mrb_iup_handle *handle = (mrb_iup_handle *)ptr;
    if (handle->handle)
    {
      IupDestroy(handle->handle);
    }
    for (size_t i = 0; i < handle->values.len; ++i)
    {
      mrb_free(mrb, handle->values.elements[i]);
    }
    mrb_free(mrb, handle->values.elements);
    mrb_free(mrb, handle);
  }
}

const struct mrb_data_type mrb_iup_handle_data_type = {
   "Iup::Handle", mrb_free_handle
};

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

static Icallback
get_callback(mrb_state *mrb, mrb_value block, mrb_value self)
{
  int
  cb()
  {
    mrb_funcall(mrb, block, "call", 1, self);
    return 0;
  }
  return cb;
}

mrb_iup_handle *
mrb_iup_get_handle(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  Data_Get_Struct(mrb, self, &mrb_iup_handle_data_type, data);
  if (!data->handle)
  {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Handle not available");
  }
  return data;
}

static mrb_value
mrb_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *handle = (mrb_iup_handle *)mrb_malloc(mrb, sizeof(mrb_iup_handle));
  handle->handle = NULL;
  handle->self = self;
  handle->mrb = mrb;
  handle->values.len = 0;
  handle->values.capa = 16;
  handle->values.elements = mrb_malloc(mrb, sizeof(char *) * 16);
  DATA_TYPE(self) = &mrb_iup_handle_data_type;
  DATA_PTR(self) = handle;
  mrb_iv_set(mrb, self, CHILDREN, mrb_ary_new(mrb));
  mrb_iv_set(mrb, self, PROCS, mrb_hash_new(mrb));
  mrb_iv_set(mrb, self, ATTRS, mrb_hash_new(mrb));
  return self;
}

static mrb_value
mrb_append(mrb_state *mrb, mrb_value self)
{
  mrb_value child_obj;
  mrb_iup_handle *child, *data;
  mrb_get_args(mrb, "o", &child_obj);
  child = mrb_iup_get_handle(mrb, child_obj);
  data = mrb_iup_get_handle(mrb, self);
  IupAppend(data->handle, child->handle);
  mrb_ary_push(mrb, mrb_iv_get(mrb, self, CHILDREN), child_obj);
  return self;
}

static mrb_value
mrb_insert(mrb_state *mrb, mrb_value self)
{
  mrb_value child_obj;
  mrb_iup_handle *ref, *child, *data;
  mrb_get_args(mrb, "do", &ref, &mrb_iup_handle_data_type, &child_obj);
  data = mrb_iup_get_handle(mrb, self);
  child = mrb_iup_get_handle(mrb, child_obj);
  IupInsert(data->handle, ref->handle, child->handle);
  mrb_ary_push(mrb, mrb_iv_get(mrb, self, CHILDREN), child_obj);
  return self;
}

static mrb_value
mrb_refresh(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupRefresh(data->handle);
  return self;
}

static mrb_value
mrb_detach(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupDetach(data->handle);
  return self;
}

static mrb_value
mrb_map(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupMap(data->handle);
  return self;
}

static mrb_value
mrb_unmap(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupUnmap(data->handle);
  return self;
}

static mrb_value
mrb_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupDestroy(data->handle);
  data->handle = NULL;
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

static inline mrb_bool
mrb_iup_handle_p(mrb_value value)
{
  return mrb_data_p(value) && DATA_TYPE(value) == &mrb_iup_handle_data_type;
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
  else if (mrb_proc_p(value) || mrb_respond_to(mrb, value, mrb_intern_cstr(mrb, "call")))
  {
    mrb_hash_set(mrb, mrb_iv_get(mrb, self, PROCS), mrb_str_new_cstr(mrb, name), value);
    IupSetCallback(data->handle, name, get_callback(mrb, value, self));
  }
  else
  {
    char *v = push_str_value(mrb, data, mrb_str_to_cstr(mrb, mrb_funcall(mrb, value, "to_s", 0)));
    IupSetAttribute(data->handle, name, v);
  }
  return self;
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
  mrb_value values[] = {
    mrb_fixnum_value(a),
    mrb_fixnum_value(b),
  };
  return mrb_ary_new_from_values(mrb, 2, values);
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
  mrb_value values[] = {
    mrb_fixnum_value(r),
    mrb_fixnum_value(g),
    mrb_fixnum_value(b),
    mrb_fixnum_value(a),
  };
  return mrb_ary_new_from_values(mrb, 4, values);
}

static mrb_value
mrb_popup(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  mrb_int x, y;
  mrb_get_args(mrb, "ii", &x, &y);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_fixnum_value(IupPopup(data->handle, x, y));
}

static mrb_value
mrb_show(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data;
  mrb_int x, y;
  mrb_get_args(mrb, "ii", &x, &y);
  data = mrb_iup_get_handle(mrb, self);
  return mrb_fixnum_value(IupShowXY(data->handle, x, y));
}

static mrb_value
mrb_hide(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  return mrb_fixnum_value(IupHide(data->handle));
}

static mrb_value
mrb_save_image(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  const char *name, *format;
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  argc = mrb_get_args(mrb, "z|z", &name, &format);
  if (argc < 2) format = "PNG";
  IupSaveImage(data->handle, name, format);
  return self;
}

struct RClass *
mrb_init_iup_handle(mrb_state *mrb, struct RClass *iup)
{
  struct RClass *handle = mrb_define_class_under(mrb, iup, "Handle", mrb->object_class);
  MRB_SET_INSTANCE_TT(handle, MRB_TT_DATA);

  mrb_define_method(mrb, handle, "initialize", mrb_initialize, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "append", mrb_append, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "insert", mrb_insert, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, handle, "refresh", mrb_refresh, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "detach", mrb_detach, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "map", mrb_map, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "unmap", mrb_unmap, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "destroy", mrb_destroy, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "[]", mrb_get_attribute, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "[]=", mrb_set_attribute, MRB_ARGS_REQ(2));

  mrb_define_method(mrb, handle, "get_string", mrb_get_attribute, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_int", mrb_get_int, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_ints", mrb_get_ints, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_int2", mrb_get_int2, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_float", mrb_get_float, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "get_color", mrb_get_color, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, handle, "popup", mrb_popup, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, handle, "show", mrb_show, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, handle, "hide", mrb_hide, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "save_image", mrb_save_image, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(1));

  return handle;
}