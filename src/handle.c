#include "mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "mruby-iup/gui.h"

#include "iup.h"
#include "iupim.h"
#include "iup_config.h"

#include <string.h>
#include <stdarg.h>

#define CHILDREN mrb_intern_cstr(mrb, "#_children_")
#define PROCS mrb_intern_cstr(mrb, "#_procs_")
#define ATTRS mrb_intern_cstr(mrb, "#_attrs_")
#define HANDLES mrb_intern_cstr(mrb, "#_attrs_")
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

mrb_bool
mrb_iup_handle_p(mrb_value value)
{
  return mrb_data_p(value) && DATA_TYPE(value) == &mrb_iup_handle_data_type;
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

static mrb_value
mrb_load_config(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  return mrb_fixnum_value(IupConfigLoad(data->handle));
}

static mrb_value
mrb_update(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupUpdate(data->handle);
  return self;
}

static mrb_value
mrb_update_children(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupUpdateChildren(data->handle);
  return self;
}

static mrb_value
mrb_redraw(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  IupRedraw(data->handle, 1);
  return self;
}

static mrb_value
mrb_EQUAL(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj);
  if (mrb_iup_handle_p(obj))
  {
    mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
    mrb_iup_handle *other = mrb_iup_get_handle(mrb, obj);
    return mrb_bool_value(data->handle == other->handle);
  }
  return mrb_false_value();
}

static mrb_value
mrb_reparent(mrb_state *mrb, mrb_value self)
{
  mrb_value parent, ref;
  mrb_iup_handle *new_parent;
  Ihandle *ref_child = NULL;
  mrb_int argc = mrb_get_args(mrb, "o|o", &parent, &ref);
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  new_parent = mrb_iup_get_handle(mrb, parent);
  if (argc > 1)
  {
    ref_child = mrb_iup_get_handle(mrb, ref)->handle; 
  }
  if (IupReparent(data->handle, new_parent->handle, ref_child) == IUP_NOERROR)
  {
    mrb_ary_push(mrb, mrb_iv_get(mrb, parent, CHILDREN), self);
  }
  return self;
}

static mrb_value
mrb_parent(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  Ihandle *h = IupGetParent(data->handle);
  return h ? mrb_iup_new_handle(mrb, h) : mrb_nil_value();
}

static mrb_value
mrb_children(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  int count = IupGetChildCount(data->handle);
  mrb_value result = mrb_ary_new_capa(mrb, count);
  for (int i = 0; i < count; ++i)
  {
    Ihandle *child = IupGetChild(data->handle, i);
    mrb_ary_push(mrb, result, mrb_iup_new_handle(mrb, child));
  }
  return result;
}

static mrb_value
mrb_dialog(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *data = mrb_iup_get_handle(mrb, self);
  Ihandle *h = IupGetDialog(data->handle);
  return h ? mrb_iup_new_handle(mrb, h) : mrb_nil_value();
}

void
mrb_iup_handle_push_child(mrb_state *mrb, mrb_value self, Ihandle *child)
{
  mrb_ary_push(mrb, mrb_iv_get(mrb, self, CHILDREN), mrb_iup_new_handle(mrb, child));
}

mrb_value
mrb_iup_new_handle(mrb_state *mrb, Ihandle *handle)
{
  if (!handle) return mrb_nil_value();
  {
    struct RClass *iup = mrb_module_get(mrb, "IUP");
    mrb_value result = mrb_obj_new(mrb, mrb_class_get_under(mrb, iup, "Handle"), 0, NULL);
    mrb_iup_handle *ptr = (mrb_iup_handle *)DATA_PTR(result);
    ptr->handle = handle;
    {
      int size = IupGetChildCount(handle);
      for (int i = 0; i < size; ++i)
      {
        mrb_iup_handle_push_child(mrb, result, IupGetChild(handle, i));
      }
    }
    return result;
  }
}

void
mrb_init_iup_attributes(mrb_state *mrb, struct RClass *handle);

void
mrb_init_iup_draw(mrb_state *mrb, struct RClass *handle);

void
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
  mrb_define_method(mrb, handle, "update", mrb_update, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "update_children", mrb_update_children, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "redraw", mrb_redraw, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "reparent", mrb_reparent, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(1));

  mrb_define_method(mrb, handle, "parent", mrb_parent, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "children", mrb_children, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "dialog", mrb_dialog, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "parent=", mrb_reparent, MRB_ARGS_REQ(1));

  mrb_init_iup_attributes(mrb, handle);

  mrb_define_method(mrb, handle, "popup", mrb_popup, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, handle, "show", mrb_show, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, handle, "hide", mrb_hide, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "save_image", mrb_save_image, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(1));

  mrb_define_method(mrb, handle, "load_config", mrb_load_config, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "==", mrb_EQUAL, MRB_ARGS_REQ(1));

  mrb_init_iup_draw(mrb, handle);

  mrb_mod_cv_set(mrb, iup, HANDLES, mrb_ary_new(mrb));
}