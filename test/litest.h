/*
 * Copyright © 2013 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef LITEST_H
#define LITEST_H

#include <stdbool.h>
#include <check.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <libinput.h>
#include <math.h>

extern void litest_setup_tests_udev(void);
extern void litest_setup_tests_path(void);
extern void litest_setup_tests_pointer(void);
extern void litest_setup_tests_touch(void);
extern void litest_setup_tests_log(void);
extern void litest_setup_tests_tablet(void);
extern void litest_setup_tests_pad(void);
extern void litest_setup_tests_touchpad(void);
extern void litest_setup_tests_touchpad_tap(void);
extern void litest_setup_tests_touchpad_buttons(void);
extern void litest_setup_tests_trackpoint(void);
extern void litest_setup_tests_trackball(void);
extern void litest_setup_tests_misc(void);
extern void litest_setup_tests_keyboard(void);
extern void litest_setup_tests_device(void);
extern void litest_setup_tests_gestures(void);

void
litest_fail_condition(const char *file,
		      int line,
		      const char *func,
		      const char *condition,
		      const char *message,
		      ...);
void
litest_fail_comparison_int(const char *file,
			   int line,
			   const char *func,
			   const char *operator,
			   int a,
			   int b,
			   const char *astr,
			   const char *bstr);
void
litest_fail_comparison_ptr(const char *file,
			   int line,
			   const char *func,
			   const char *comparison);

#define litest_assert(cond) \
	do { \
		if (!(cond)) \
			litest_fail_condition(__FILE__, __LINE__, __func__, \
					      #cond, NULL); \
	} while(0)

#define litest_assert_msg(cond, ...) \
	do { \
		if (!(cond)) \
			litest_fail_condition(__FILE__, __LINE__, __func__, \
					      #cond, __VA_ARGS__); \
	} while(0)

#define litest_abort_msg(...) \
	litest_fail_condition(__FILE__, __LINE__, __func__, \
			      "aborting", __VA_ARGS__); \

#define litest_assert_notnull(cond) \
	do { \
		if ((cond) == NULL) \
			litest_fail_condition(__FILE__, __LINE__, __func__, \
					      #cond, " expected to be not NULL\n"); \
	} while(0)

#define litest_assert_comparison_int_(a_, op_, b_) \
	do { \
		__typeof__(a_) _a = a_; \
		__typeof__(b_) _b = b_; \
		if (trunc(_a) != _a || trunc(_b) != _b) \
			litest_abort_msg("litest_assert_int_* used for non-integer value\n"); \
		if (!((_a) op_ (_b))) \
			litest_fail_comparison_int(__FILE__, __LINE__, __func__,\
						   #op_, _a, _b, \
						   #a_, #b_); \
	} while(0)

#define litest_assert_int_eq(a_, b_) \
	litest_assert_comparison_int_(a_, ==, b_)

#define litest_assert_int_ne(a_, b_) \
	litest_assert_comparison_int_(a_, !=, b_)

#define litest_assert_int_lt(a_, b_) \
	litest_assert_comparison_int_(a_, <, b_)

#define litest_assert_int_le(a_, b_) \
	litest_assert_comparison_int_(a_, <=, b_)

#define litest_assert_int_ge(a_, b_) \
	litest_assert_comparison_int_(a_, >=, b_)

#define litest_assert_int_gt(a_, b_) \
	litest_assert_comparison_int_(a_, >, b_)

#define litest_assert_comparison_ptr_(a_, op_, b_) \
	do { \
		__typeof__(a_) _a = a_; \
		__typeof__(b_) _b = b_; \
		if (!((_a) op_ (_b))) \
			litest_fail_comparison_ptr(__FILE__, __LINE__, __func__,\
						   #a_ " " #op_ " " #b_); \
	} while(0)

#define litest_assert_ptr_eq(a_, b_) \
	litest_assert_comparison_ptr_(a_, ==, b_)

#define litest_assert_ptr_ne(a_, b_) \
	litest_assert_comparison_ptr_(a_, !=, b_)

#define litest_assert_ptr_null(a_) \
	litest_assert_comparison_ptr_(a_, ==, NULL)

#define litest_assert_ptr_notnull(a_) \
	litest_assert_comparison_ptr_(a_, !=, NULL)

#define litest_assert_double_eq(a_, b_)\
	ck_assert_int_eq((int)((a_) * 256), (int)((b_) * 256))

#define litest_assert_double_ne(a_, b_)\
	ck_assert_int_ne((int)((a_) * 256), (int)((b_) * 256))

#define litest_assert_double_lt(a_, b_)\
	ck_assert_int_lt((int)((a_) * 256), (int)((b_) * 256))

#define litest_assert_double_le(a_, b_)\
	ck_assert_int_le((int)((a_) * 256), (int)((b_) * 256))

#define litest_assert_double_gt(a_, b_)\
	ck_assert_int_gt((int)((a_) * 256), (int)((b_) * 256))

#define litest_assert_double_ge(a_, b_)\
	ck_assert_int_ge((int)((a_) * 256), (int)((b_) * 256))

enum litest_device_type {
	LITEST_NO_DEVICE = -1,
	LITEST_SYNAPTICS_CLICKPAD = -1000,
	LITEST_SYNAPTICS_TOUCHPAD,
	LITEST_SYNAPTICS_TOPBUTTONPAD,
	LITEST_BCM5974,
	LITEST_KEYBOARD,
	LITEST_TRACKPOINT,
	LITEST_MOUSE,
	LITEST_WACOM_TOUCH,
	LITEST_ALPS_SEMI_MT,
	LITEST_GENERIC_SINGLETOUCH,
	LITEST_MS_SURFACE_COVER,
	LITEST_QEMU_TABLET,
	LITEST_XEN_VIRTUAL_POINTER,
	LITEST_VMWARE_VIRTMOUSE,
	LITEST_SYNAPTICS_HOVER_SEMI_MT,
	LITEST_SYNAPTICS_TRACKPOINT_BUTTONS,
	LITEST_PROTOCOL_A_SCREEN,
	LITEST_WACOM_FINGER,
	LITEST_KEYBOARD_BLACKWIDOW,
	LITEST_WHEEL_ONLY,
	LITEST_MOUSE_ROCCAT,
	LITEST_LOGITECH_TRACKBALL,
	LITEST_ATMEL_HOVER,
	LITEST_ALPS_DUALPOINT,
	LITEST_MOUSE_LOW_DPI,
	LITEST_GENERIC_MULTITOUCH_SCREEN,
	LITEST_NEXUS4_TOUCH_SCREEN,
	LITEST_MAGIC_TRACKPAD,
	LITEST_ELANTECH_TOUCHPAD,
	LITEST_MOUSE_GLADIUS,
	LITEST_MOUSE_WHEEL_CLICK_ANGLE,
	LITEST_APPLE_KEYBOARD,
	LITEST_ANKER_MOUSE_KBD,
	LITEST_WACOM_BAMBOO,
	LITEST_WACOM_CINTIQ,
	LITEST_WACOM_INTUOS,
	LITEST_WACOM_ISDV4,
	LITEST_WALTOP,
	LITEST_HUION_TABLET,
	LITEST_CYBORG_RAT,
	LITEST_YUBIKEY,
	LITEST_SYNAPTICS_I2C,
	LITEST_WACOM_CINTIQ_24HD,
	LITEST_MULTITOUCH_FUZZ_SCREEN,
	LITEST_WACOM_INTUOS3_PAD,
	LITEST_WACOM_INTUOS5_PAD,
	LITEST_KEYBOARD_ALL_CODES,
	LITEST_MAGICMOUSE,
	LITEST_WACOM_EKR,
	LITEST_WACOM_CINTIQ_24HDT_PAD,
	LITEST_WACOM_CINTIQ_13HDT_PEN,
	LITEST_WACOM_CINTIQ_13HDT_PAD,
	LITEST_WACOM_CINTIQ_13HDT_FINGER,
};

enum litest_device_feature {
	LITEST_DISABLE_DEVICE = -1,
	LITEST_ANY = 0,
	LITEST_TOUCHPAD = 1 << 0,
	LITEST_CLICKPAD = 1 << 1,
	LITEST_BUTTON = 1 << 2,
	LITEST_KEYS = 1 << 3,
	LITEST_RELATIVE = 1 << 4,
	LITEST_WHEEL = 1 << 5,
	LITEST_TOUCH = 1 << 6,
	LITEST_SINGLE_TOUCH = 1 << 7,
	LITEST_APPLE_CLICKPAD = 1 << 8,
	LITEST_TOPBUTTONPAD = 1 << 9,
	LITEST_SEMI_MT = 1 << 10,
	LITEST_POINTINGSTICK = 1 << 11,
	LITEST_FAKE_MT = 1 << 12,
	LITEST_ABSOLUTE = 1 << 13,
	LITEST_PROTOCOL_A = 1 << 14,
	LITEST_HOVER = 1 << 15,
	LITEST_ELLIPSE = 1 << 16,
	LITEST_TABLET = 1 << 17,
	LITEST_DISTANCE = 1 << 18,
	LITEST_TOOL_SERIAL = 1 << 19,
	LITEST_TILT = 1 << 20,
	LITEST_TABLET_PAD = 1 << 21,
	LITEST_RING = 1 << 22,
	LITEST_STRIP = 1 << 23,
	LITEST_TRACKBALL = 1 << 24,
};

struct litest_device {
	struct libevdev *evdev;
	struct libevdev_uinput *uinput;
	struct libinput *libinput;
	bool owns_context;
	struct libinput_device *libinput_device;
	struct litest_device_interface *interface;

	int ntouches_down;
	bool skip_ev_syn;

	void *private; /* device-specific data */
};

struct axis_replacement {
	int32_t evcode;
	double value;
};

static inline void
litest_axis_set_value(struct axis_replacement *axes, int code, double value)
{
	litest_assert_double_ge(value, 0.0);
	litest_assert_double_le(value, 100.0);

	while (axes->evcode != -1) {
		if (axes->evcode == code) {
			axes->value = value;
			return;
		}
		axes++;
	}

	litest_abort_msg("Missing axis code %d\n", code);
}

/* A loop range, resolves to:
   for (i = lower; i < upper; i++)
 */
struct range {
	int lower; /* inclusive */
	int upper; /* exclusive */
};

struct libinput *litest_create_context(void);
void litest_disable_log_handler(struct libinput *libinput);
void litest_restore_log_handler(struct libinput *libinput);

#define litest_add(name_, func_, ...) \
	_litest_add(name_, #func_, func_, __VA_ARGS__)
#define litest_add_ranged(name_, func_, ...) \
	_litest_add_ranged(name_, #func_, func_, __VA_ARGS__)
#define litest_add_for_device(name_, func_, ...) \
	_litest_add_for_device(name_, #func_, func_, __VA_ARGS__)
#define litest_add_ranged_for_device(name_, func_, ...) \
	_litest_add_ranged_for_device(name_, #func_, func_, __VA_ARGS__)
#define litest_add_no_device(name_, func_) \
	_litest_add_no_device(name_, #func_, func_)
#define litest_add_ranged_no_device(name_, func_, ...) \
	_litest_add_ranged_no_device(name_, #func_, func_, __VA_ARGS__)
void
_litest_add(const char *name,
	    const char *funcname,
	    void *func,
	    enum litest_device_feature required_feature,
	    enum litest_device_feature excluded_feature);
void
_litest_add_ranged(const char *name,
		   const char *funcname,
		   void *func,
		   enum litest_device_feature required,
		   enum litest_device_feature excluded,
		   const struct range *range);
void
_litest_add_for_device(const char *name,
		       const char *funcname,
		       void *func,
		       enum litest_device_type type);
void
_litest_add_ranged_for_device(const char *name,
			      const char *funcname,
			      void *func,
			      enum litest_device_type type,
			      const struct range *range);
void
_litest_add_no_device(const char *name,
		      const char *funcname,
		      void *func);
void
_litest_add_ranged_no_device(const char *name,
			     const char *funcname,
			     void *func,
			     const struct range *range);

struct litest_device *
litest_create_device(enum litest_device_type which);

struct litest_device *
litest_add_device(struct libinput *libinput,
		  enum litest_device_type which);
struct libevdev_uinput *
litest_create_uinput_device_from_description(const char *name,
					     const struct input_id *id,
					     const struct input_absinfo *abs,
					     const int *events);
struct litest_device *
litest_create_device_with_overrides(enum litest_device_type which,
				    const char *name_override,
				    struct input_id *id_override,
				    const struct input_absinfo *abs_override,
				    const int *events_override);
struct litest_device *
litest_add_device_with_overrides(struct libinput *libinput,
				 enum litest_device_type which,
				 const char *name_override,
				 struct input_id *id_override,
				 const struct input_absinfo *abs_override,
				 const int *events_override);

struct litest_device *
litest_current_device(void);

void
litest_delete_device(struct litest_device *d);

int
litest_handle_events(struct litest_device *d);

void
litest_event(struct litest_device *t,
	     unsigned int type,
	     unsigned int code,
	     int value);
int
litest_auto_assign_value(struct litest_device *d,
			 const struct input_event *ev,
			 int slot, double x, double y,
			 struct axis_replacement *axes,
			 bool touching);
void
litest_touch_up(struct litest_device *d, unsigned int slot);

void
litest_touch_move(struct litest_device *d,
		  unsigned int slot,
		  double x,
		  double y);

void
litest_touch_move_extended(struct litest_device *d,
			   unsigned int slot,
			   double x,
			   double y,
			   struct axis_replacement *axes);

void
litest_touch_down(struct litest_device *d,
		  unsigned int slot,
		  double x,
		  double y);

void
litest_touch_down_extended(struct litest_device *d,
			   unsigned int slot,
			   double x,
			   double y,
			   struct axis_replacement *axes);

void
litest_touch_move_to(struct litest_device *d,
		     unsigned int slot,
		     double x_from, double y_from,
		     double x_to, double y_to,
		     int steps, int sleep_ms);

void
litest_touch_move_two_touches(struct litest_device *d,
			      double x0, double y0,
			      double x1, double y1,
			      double dx, double dy,
			      int steps, int sleep_ms);

void
litest_touch_move_three_touches(struct litest_device *d,
				double x0, double y0,
				double x1, double y1,
				double x2, double y2,
				double dx, double dy,
				int steps, int sleep_ms);

void
litest_tablet_proximity_in(struct litest_device *d,
			   int x, int y,
			   struct axis_replacement *axes);

void
litest_tablet_proximity_out(struct litest_device *d);

void
litest_tablet_motion(struct litest_device *d,
		     int x, int y,
		     struct axis_replacement *axes);

void
litest_pad_ring_start(struct litest_device *d, double value);

void
litest_pad_ring_change(struct litest_device *d, double value);

void
litest_pad_ring_end(struct litest_device *d);

void
litest_pad_strip_start(struct litest_device *d, double value);

void
litest_pad_strip_change(struct litest_device *d, double value);

void
litest_pad_strip_end(struct litest_device *d);

void
litest_hover_start(struct litest_device *d,
		   unsigned int slot,
		   double x,
		   double y);

void
litest_hover_end(struct litest_device *d, unsigned int slot);

void litest_hover_move(struct litest_device *d,
		       unsigned int slot,
		       double x,
		       double y);

void
litest_hover_move_to(struct litest_device *d,
		     unsigned int slot,
		     double x_from, double y_from,
		     double x_to, double y_to,
		     int steps, int sleep_ms);

void
litest_hover_move_two_touches(struct litest_device *d,
			      double x0, double y0,
			      double x1, double y1,
			      double dx, double dy,
			      int steps, int sleep_ms);

void
litest_button_click(struct litest_device *d,
		    unsigned int button,
		    bool is_press);

void
litest_button_scroll(struct litest_device *d,
		     unsigned int button,
		     double dx, double dy);

void
litest_keyboard_key(struct litest_device *d,
		    unsigned int key,
		    bool is_press);

void
litest_wait_for_event(struct libinput *li);

void
litest_wait_for_event_of_type(struct libinput *li, ...);

void
litest_drain_events(struct libinput *li);

void
litest_assert_empty_queue(struct libinput *li);

struct libinput_event_pointer *
litest_is_button_event(struct libinput_event *event,
		       unsigned int button,
		       enum libinput_button_state state);

struct libinput_event_pointer *
litest_is_axis_event(struct libinput_event *event,
		     enum libinput_pointer_axis axis,
		     enum libinput_pointer_axis_source source);

struct libinput_event_pointer *
litest_is_motion_event(struct libinput_event *event);

struct libinput_event_touch *
litest_is_touch_event(struct libinput_event *event,
		      enum libinput_event_type type);

struct libinput_event_keyboard *
litest_is_keyboard_event(struct libinput_event *event,
			 unsigned int key,
			 enum libinput_key_state state);

struct libinput_event_gesture *
litest_is_gesture_event(struct libinput_event *event,
			enum libinput_event_type type,
			int nfingers);

struct libinput_event_tablet_tool *
litest_is_tablet_event(struct libinput_event *event,
		       enum libinput_event_type type);

struct libinput_event_tablet_pad *
litest_is_pad_button_event(struct libinput_event *event,
			   unsigned int button,
			   enum libinput_button_state state);
struct libinput_event_tablet_pad *
litest_is_pad_ring_event(struct libinput_event *event,
			 unsigned int number,
			 enum libinput_tablet_pad_ring_axis_source source);
struct libinput_event_tablet_pad *
litest_is_pad_strip_event(struct libinput_event *event,
			  unsigned int number,
			  enum libinput_tablet_pad_strip_axis_source source);

void
litest_assert_button_event(struct libinput *li,
			   unsigned int button,
			   enum libinput_button_state state);

void
litest_assert_scroll(struct libinput *li,
		     enum libinput_pointer_axis axis,
		     int minimum_movement);

void
litest_assert_only_typed_events(struct libinput *li,
				enum libinput_event_type type);

void
litest_assert_tablet_button_event(struct libinput *li,
				  unsigned int button,
				  enum libinput_button_state state);

void
litest_assert_tablet_proximity_event(struct libinput *li,
				     enum libinput_tablet_tool_proximity_state state);

void
litest_assert_pad_button_event(struct libinput *li,
				    unsigned int button,
				    enum libinput_button_state state);
struct libevdev_uinput *
litest_create_uinput_device(const char *name,
			    struct input_id *id,
			    ...);

struct libevdev_uinput *
litest_create_uinput_abs_device(const char *name,
				struct input_id *id,
				const struct input_absinfo *abs,
				...);

void
litest_timeout_tap(void);

void
litest_timeout_tapndrag(void);

void
litest_timeout_softbuttons(void);

void
litest_timeout_buttonscroll(void);

void
litest_timeout_edgescroll(void);

void
litest_timeout_finger_switch(void);

void
litest_timeout_middlebutton(void);

void
litest_timeout_dwt_short(void);

void
litest_timeout_dwt_long(void);

void
litest_timeout_gesture(void);

void
litest_push_event_frame(struct litest_device *dev);

void
litest_pop_event_frame(struct litest_device *dev);

/* this is a semi-mt device, so we keep track of the touches that the tests
 * send and modify them so that the first touch is always slot 0 and sends
 * the top-left of the bounding box, the second is always slot 1 and sends
 * the bottom-right of the bounding box.
 * Lifting any of two fingers terminates slot 1
 */
struct litest_semi_mt {
	int tracking_id;
	/* The actual touches requested by the test for the two slots
	 * in the 0..100 range used by litest */
	struct {
		double x, y;
	} touches[2];
};

void
litest_semi_mt_touch_down(struct litest_device *d,
			  struct litest_semi_mt *semi_mt,
			  unsigned int slot,
			  double x, double y);

void
litest_semi_mt_touch_move(struct litest_device *d,
			  struct litest_semi_mt *semi_mt,
			  unsigned int slot,
			  double x, double y);

void
litest_semi_mt_touch_up(struct litest_device *d,
			struct litest_semi_mt *semi_mt,
			unsigned int slot);

#ifndef ck_assert_notnull
#define ck_assert_notnull(ptr) ck_assert_ptr_ne(ptr, NULL)
#endif

static inline void
litest_enable_tap(struct libinput_device *device)
{
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_tap_set_enabled(device,
							LIBINPUT_CONFIG_TAP_ENABLED);

	litest_assert_int_eq(status, expected);
}

static inline void
litest_disable_tap(struct libinput_device *device)
{
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_tap_set_enabled(device,
							LIBINPUT_CONFIG_TAP_DISABLED);

	litest_assert_int_eq(status, expected);
}

static inline void
litest_enable_tap_drag(struct libinput_device *device)
{
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_tap_set_drag_enabled(device,
							     LIBINPUT_CONFIG_DRAG_ENABLED);

	litest_assert_int_eq(status, expected);
}

static inline void
litest_disable_tap_drag(struct libinput_device *device)
{
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_tap_set_drag_enabled(device,
							     LIBINPUT_CONFIG_DRAG_DISABLED);

	litest_assert_int_eq(status, expected);
}

static inline bool
litest_has_2fg_scroll(struct litest_device *dev)
{
	struct libinput_device *device = dev->libinput_device;

	return !!(libinput_device_config_scroll_get_methods(device) &
		  LIBINPUT_CONFIG_SCROLL_2FG);
}

static inline void
litest_enable_2fg_scroll(struct litest_device *dev)
{
	enum libinput_config_status status, expected;
	struct libinput_device *device = dev->libinput_device;

	status = libinput_device_config_scroll_set_method(device,
					  LIBINPUT_CONFIG_SCROLL_2FG);

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	litest_assert_int_eq(status, expected);
}

static inline void
litest_enable_edge_scroll(struct litest_device *dev)
{
	enum libinput_config_status status, expected;
	struct libinput_device *device = dev->libinput_device;

	status = libinput_device_config_scroll_set_method(device,
					  LIBINPUT_CONFIG_SCROLL_EDGE);

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	litest_assert_int_eq(status, expected);
}

static inline void
litest_enable_clickfinger(struct litest_device *dev)
{
	enum libinput_config_status status, expected;
	struct libinput_device *device = dev->libinput_device;

	status = libinput_device_config_click_set_method(device,
				 LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER);
	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	litest_assert_int_eq(status, expected);
}

static inline void
litest_enable_buttonareas(struct litest_device *dev)
{
	enum libinput_config_status status, expected;
	struct libinput_device *device = dev->libinput_device;

	status = libinput_device_config_click_set_method(device,
				 LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS);
	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	litest_assert_int_eq(status, expected);
}

static inline void
litest_enable_drag_lock(struct libinput_device *device)
{
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_tap_set_drag_lock_enabled(device,
								  LIBINPUT_CONFIG_DRAG_LOCK_ENABLED);

	litest_assert_int_eq(status, expected);
}

static inline void
litest_disable_drag_lock(struct libinput_device *device)
{
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_tap_set_drag_lock_enabled(device,
								  LIBINPUT_CONFIG_DRAG_LOCK_DISABLED);

	litest_assert_int_eq(status, expected);
}

static inline void
litest_enable_middleemu(struct litest_device *dev)
{
	struct libinput_device *device = dev->libinput_device;
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_middle_emulation_set_enabled(device,
								     LIBINPUT_CONFIG_MIDDLE_EMULATION_ENABLED);

	litest_assert_int_eq(status, expected);
}

static inline void
litest_disable_middleemu(struct litest_device *dev)
{
	struct libinput_device *device = dev->libinput_device;
	enum libinput_config_status status, expected;

	expected = LIBINPUT_CONFIG_STATUS_SUCCESS;
	status = libinput_device_config_middle_emulation_set_enabled(device,
								     LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED);

	litest_assert_int_eq(status, expected);
}

#define CK_DOUBLE_EQ_EPSILON 1E-3
#define ck_assert_double_eq(X,Y)  \
	do { \
		double _ck_x = X; \
		double _ck_y = Y; \
		ck_assert_msg(fabs(_ck_x - _ck_y) < CK_DOUBLE_EQ_EPSILON, \
			      "Assertion '" #X " == " #Y \
			      "' failed: "#X"==%f, "#Y"==%f", \
			      _ck_x, \
			      _ck_y); \
	} while (0)

#define ck_assert_double_ne(X,Y)  \
	do { \
		double _ck_x = X; \
		double _ck_y = Y; \
		ck_assert_msg(fabs(_ck_x - _ck_y) > CK_DOUBLE_EQ_EPSILON, \
			      "Assertion '" #X " != " #Y \
			      "' failed: "#X"==%f, "#Y"==%f", \
			      _ck_x, \
			      _ck_y); \
	} while (0)

#define _ck_assert_double_eq(X, OP, Y)  \
	do { \
		double _ck_x = X; \
		double _ck_y = Y; \
		ck_assert_msg(_ck_x OP _ck_y || \
			      fabs(_ck_x - _ck_y) < CK_DOUBLE_EQ_EPSILON, \
			      "Assertion '" #X#OP#Y \
			      "' failed: "#X"==%f, "#Y"==%f", \
			      _ck_x, \
			      _ck_y); \
	} while (0)

#define _ck_assert_double_ne(X, OP,Y) \
	do { \
		double _ck_x = X; \
		double _ck_y = Y; \
		ck_assert_msg(_ck_x OP _ck_y && \
			      fabs(_ck_x - _ck_y) > CK_DOUBLE_EQ_EPSILON, \
			      "Assertion '" #X#OP#Y \
			      "' failed: "#X"==%f, "#Y"==%f", \
			      _ck_x, \
			      _ck_y); \
	} while (0)
#define ck_assert_double_lt(X, Y) _ck_assert_double_ne(X, <, Y)
#define ck_assert_double_le(X, Y) _ck_assert_double_eq(X, <=, Y)
#define ck_assert_double_gt(X, Y) _ck_assert_double_ne(X, >, Y)
#define ck_assert_double_ge(X, Y) _ck_assert_double_eq(X, >=, Y)
#endif /* LITEST_H */
