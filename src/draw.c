#include "mruby.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mruby/error.h"

#include "iup.h"
#include "iupdraw.h"

#include "mruby-iup/gui.h"

#include "string.h"

#define IMG "__MRUBY_IMG__"

static mrb_value
mrb_begin_draw(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawBegin(handle->handle);
  return self;
}

static mrb_value
mrb_end_draw(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawEnd(handle->handle);
  return self;
}

static mrb_value
mrb_draw(mrb_state *mrb, mrb_value self)
{
  mrb_value block;
  mrb_get_args(mrb, "&!", &block);
  mrb_begin_draw(mrb, self);
  mrb_yield_argv(mrb, block, 0, NULL);
  mrb_end_draw(mrb, self);
  return self;
}

static mrb_value
mrb_set_clip_rect(mrb_state *mrb, mrb_value self)
{
  mrb_int x1, y1, x2, y2;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
  IupDrawSetClipRect(handle->handle, x1, y1, x2, y2);
  return self;
}

static mrb_value
mrb_reset_clip_rect(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawResetClip(handle->handle);
  return self;
}

static mrb_value
mrb_set_clip_rect_arr(mrb_state *mrb, mrb_value self)
{
  mrb_value value;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "A", &value);
  IupDrawSetClipRect(
    handle->handle,
    mrb_int(mrb, mrb_ary_entry(value, 0)),
    mrb_int(mrb, mrb_ary_entry(value, 1)),
    mrb_int(mrb, mrb_ary_entry(value, 2)),
    mrb_int(mrb, mrb_ary_entry(value, 3))
  );
  return self;
}

static mrb_value
mrb_clip_rect(mrb_state *mrb, mrb_value self)
{
  int x1, x2, y1, y2;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawGetClipRect(handle->handle, &x1, &y1, &x2, &y2);
  {
    mrb_value values[] = {
      mrb_fixnum_value(x1),
      mrb_fixnum_value(y1),
      mrb_fixnum_value(x2),
      mrb_fixnum_value(y2),
    };
    return mrb_ary_new_from_values(mrb, 4, values);
  }
}

static mrb_value
mrb_draw_parent_background(mrb_state *mrb, mrb_value self)
{
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawParentBackground(handle->handle);
  return self;
}

static mrb_value
mrb_draw_line(mrb_state *mrb, mrb_value self)
{
  mrb_int x1, y1, x2, y2;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
  IupDrawLine(handle->handle, x1, y1, x2, y2);
  return self;
}

static mrb_value
mrb_draw_rectangle(mrb_state *mrb, mrb_value self)
{
  mrb_int x1, y1, x2, y2;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
  IupDrawRectangle(handle->handle, x1, y1, x2, y2);
  return self;
}

static mrb_value
mrb_draw_arc(mrb_state *mrb, mrb_value self)
{
  mrb_int x1, y1, x2, y2;
  mrb_float a1, a2;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "iiiiff", &x1, &y1, &x2, &y2, &a1, &a2);
  IupDrawArc(handle->handle, x1, y1, x2, y2, a1, a2);
  return self;
}

static mrb_value
mrb_draw_polygon(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_value *values;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "*", &values, &size);
  {
    int points[size * 2];
    for (mrb_int i = 0; i < size; ++i)
    {
      if (!mrb_array_p(values[i])) mrb_raise(mrb, E_ARGUMENT_ERROR, "Point is not an array");
      points[i * 2 + 0] = mrb_int(mrb, mrb_ary_entry(values[i], 0));
      points[i * 2 + 1] = mrb_int(mrb, mrb_ary_entry(values[i], 1));
    }
    IupDrawPolygon(handle->handle, points, size * 2);
  }
  return self;
}

static mrb_value
mrb_draw_text(mrb_state *mrb, mrb_value self)
{
  size_t len;
  const char *text;
  mrb_int x, y, w, h;
  int w0, h0;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_int argc = mrb_get_args(mrb, "zii|ii", &text, &x, &y, &w, &h);
  len = strlen(text);
  IupDrawGetTextSize(handle->handle, text, len, &w0, &h0);
  if (argc < 4) w = w0;
  if (argc < 5) h = h0;
  IupDrawText(handle->handle, text, len, x, y, w, h);
  return self;
}

static mrb_value
mrb_draw_image(mrb_state *mrb, mrb_value self)
{
  Ihandle *img;
  mrb_value obj;
  int w1, h1, bpp;
  mrb_int x = 0, y = 0, w, h, argc;
  argc = mrb_get_args(mrb, "o|iiii", &obj, &x, &y, &w, &h);
  img = mrb_iup_get_handle(mrb, obj)->handle;
  IupSetHandle(IMG, img);
  IupDrawGetImageInfo(IMG, &w1, &h1, &bpp); 
  if (argc < 4) w = w1;
  if (argc < 5) h = h1;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawImage(handle->handle, IMG, x, y, w, h);
  return self;
}

static mrb_value
mrb_canvas_size(mrb_state *mrb, mrb_value self)
{
  int w, h;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupDrawGetSize(handle->handle, &w, &h);
  {
    mrb_value values[] = {
      mrb_fixnum_value(w),
      mrb_fixnum_value(h),
    };
    return mrb_ary_new_from_values(mrb, 2, values);
  }
}

static mrb_value
mrb_measure_text(mrb_state *mrb, mrb_value self)
{
  int w, h;
  const char *text;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  mrb_get_args(mrb, "z", &text);
  IupDrawGetTextSize(handle->handle, text, strlen(text), &w, &h);
  {
    mrb_value values[] = {
      mrb_fixnum_value(w),
      mrb_fixnum_value(h),
    };
    return mrb_ary_new_from_values(mrb, 2, values);
  }
}

static mrb_value
mrb_image_size(mrb_state *mrb, mrb_value self)
{
  int w, h, bpp;
  mrb_iup_handle *handle = mrb_iup_get_handle(mrb, self);
  IupSetHandle(IMG, handle->handle);
  IupDrawGetImageInfo(IMG, &w, &h, &bpp);
  {
    mrb_value values[] = {
      mrb_fixnum_value(w),
      mrb_fixnum_value(h),
      mrb_fixnum_value(bpp),
    };
    return mrb_ary_new_from_values(mrb, 3, values);
  }
}

void
mrb_init_iup_draw(mrb_state *mrb, struct RClass *handle)
{
  mrb_define_method(mrb, handle, "begin_draw", mrb_begin_draw, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "end_draw", mrb_end_draw, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "draw", mrb_draw, MRB_ARGS_BLOCK());

  mrb_define_method(mrb, handle, "set_clip_rect", mrb_set_clip_rect, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, handle, "reset_clip_rect", mrb_reset_clip_rect, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "clip_rect=", mrb_set_clip_rect_arr, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, handle, "clip_rect", mrb_clip_rect, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "draw_parent_background", mrb_draw_parent_background, MRB_ARGS_NONE());

  mrb_define_method(mrb, handle, "draw_line", mrb_draw_line, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, handle, "draw_rectangle", mrb_draw_rectangle, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, handle, "draw_arc", mrb_draw_arc, MRB_ARGS_REQ(6));
  mrb_define_method(mrb, handle, "draw_polygon", mrb_draw_polygon, MRB_ARGS_REST());
  mrb_define_method(mrb, handle, "draw_text", mrb_draw_text, MRB_ARGS_REQ(3)|MRB_ARGS_OPT(2));
  mrb_define_method(mrb, handle, "draw_image", mrb_draw_image, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(4));

  mrb_define_method(mrb, handle, "canvas_size", mrb_canvas_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, handle, "measure_text", mrb_measure_text, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, handle, "image_size", mrb_image_size, MRB_ARGS_NONE());
}
