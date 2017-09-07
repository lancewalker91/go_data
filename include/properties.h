/******************************************************************************
    Copyright (C) 2014 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include "util/c99defs.h"
#include "obs-data.h"

/**
 * @file
 * @brief libobs header for the properties system used in libobs
 *
 * @page properties Properties
 * @brief Platform and Toolkit independent settings implementation
 *
 * @section prop_overview_sec Overview
 *
 * libobs uses a property system which lets for example sources specify
 * settings that can be displayed to the user by the UI.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Only update when the user presses OK or Apply */
#define PROPERTIES_DEFER_UPDATE            (1<<0)

enum property_type {
	PROPERTY_INVALID,
	PROPERTY_BOOL,
	PROPERTY_INT,
	PROPERTY_INT_SLIDER,
	PROPERTY_FLOAT,
	PROPERTY_TEXT,
	PROPERTY_PATH,
	PROPERTY_LIST,
	PROPERTY_COLOR,
	PROPERTY_BUTTON,
	PROPERTY_FONT,
	PROPERTY_EDITABLE_LIST,
	PROPERTY_STRING,
	PROPERTY_POINT,
	PROPERTY_FUNC
};

enum slider_attribute{
	SLIDER_MIN = 0,
	SLIDER_MAX,
	SLIDER_STEP,
	SLIDER_VALUE,
	SLIDER_DEDAULT
};

enum combo_format {
	COMBO_FORMAT_INVALID,
	COMBO_FORMAT_INT,
	COMBO_FORMAT_FLOAT,
	COMBO_FORMAT_STRING
};

enum combo_type {
	COMBO_TYPE_INVALID,
	COMBO_TYPE_EDITABLE,
	COMBO_TYPE_LIST,
};

enum path_type {
	PATH_FILE,
	PATH_DIRECTORY
};

enum text_type {
	TEXT_DEFAULT,
	TEXT_PASSWORD,
	TEXT_MULTILINE,
};

#define FONT_BOLD      (1<<0)
#define FONT_ITALIC    (1<<1)
#define FONT_UNDERLINE (1<<2)
#define FONT_STRIKEOUT (1<<3)

struct properties;
struct property;
typedef struct properties properties_t;
typedef struct property   property_t;

/* ------------------------------------------------------------------------- */

EXPORT properties_t *properties_create(void);
EXPORT properties_t *properties_create_param(void *param,
		void (*destroy)(void *param));
EXPORT void properties_destroy(properties_t *props);

EXPORT void properties_set_flags(properties_t *props, uint32_t flags);
EXPORT uint32_t properties_get_flags(properties_t *props);

EXPORT void properties_set_param(properties_t *props,
		void *param, void (*destroy)(void *param));
EXPORT void *properties_get_param(properties_t *props);

EXPORT property_t *properties_first(properties_t *props);

EXPORT property_t *properties_get(properties_t *props,
		const char *property);

//给property绑定属性
EXPORT void property_set_param(property_t *property, void *param, void(*destroy)(void *param));
EXPORT void *property_get_param(property_t *property);

EXPORT void property_destroy(struct property *property);

/**
 * Applies settings to the properties by calling all the necessary
 * modification callbacks
 */
EXPORT void properties_apply_settings(properties_t *props,
	obs_data_t *settings);

/* ------------------------------------------------------------------------- */

/**
 * Callback for when a button property is clicked.  If the properties
 * need to be refreshed due to changes to the property layout, return true,
 * otherwise return false.
 */
typedef bool (*property_clicked_t)(properties_t *props,
		property_t *property, void *data);

EXPORT property_t *properties_add_bool(properties_t *props,
		const char *name, const char *description);

EXPORT property_t *properties_add_int(properties_t *props,
		const char *name, const char *description,
		int value, int min, int max, int step);

EXPORT property_t *properties_add_float(properties_t *props,
		const char *name, const char *description,
		double min, double max, double step);

EXPORT property_t *properties_add_text(properties_t *props,
		const char *name, const char *description,
		enum text_type type);

/**
 * Adds a 'path' property.  Can be a directory or a file.
 *
 * If target is a file path, the filters should be this format, separated by
 * double semi-colens, and extensions separated by space:
 *   "Example types 1 and 2 (*.ex1 *.ex2);;Example type 3 (*.ex3)"
 *
 * @param  props        Properties object
 * @param  name         Settings name
 * @param  description  Description (display name) of the property
 * @param  type         Type of path (directory or file)
 * @param  filter       If type is a file path, then describes the file filter
 *                      that the user can browse.  Items are separated via
 *                      double semi-colens.  If multiple file types in a
 *                      filter, separate with space.
 */
EXPORT property_t *properties_add_path(properties_t *props,
		const char *name, const char *description,
		enum path_type type, const char *filter,
		const char *default_path);

EXPORT property_t *properties_add_list(properties_t *props,
		const char *name, const char *description,
		enum combo_type type, enum combo_format format);

EXPORT property_t *properties_add_color(properties_t *props,
		const char *name, const char *description);

EXPORT property_t *properties_add_button(properties_t *props,
		const char *name, const char *text,
		property_clicked_t callback);

/**
 * Adds a font selection property.
 *
 * A font is an data sub-object which contains the following items:
 *   face:   face name string
 *   style:  style name string
 *   size:   size integer
 *   flags:  font flags integer (FONT_* defined above)
 */
EXPORT property_t *properties_add_font(properties_t *props,
		const char *name, const char *description);

EXPORT property_t *properties_add_editable_list(properties_t *props,
		const char *name, const char *description,
		bool allow_files, const char *filter,
		const char *default_path);

//添加str属性
EXPORT property_t *properties_add_str(properties_t *props, const char *name, const char *description, const char *str);
EXPORT property_t *new_property_str(const char *name, const char *desc, const char *_str);
EXPORT bool property_str_set(property_t *p,const char *str);
EXPORT const char* property_str_get(property_t *p);


//添加专递指针的属性
EXPORT property_t *new_property_point(const char *name, const char *desc, void *_point);
EXPORT bool property_point_set(property_t *p, void *_point);
EXPORT void** property_point_get(property_t *p);


//添加方法属性
typedef void (*property_func)(void *data);
EXPORT property_t *new_property_func(const char *name, const char *desc, property_func func);
EXPORT void property_func_process(property_t *p);

//添加bool属性
EXPORT property_t *new_property_bool(const char *name, const char *desc, bool b);
EXPORT bool property_bool_set(property_t *p, bool b);
EXPORT bool property_bool_get(property_t *p);

//添加int属性
EXPORT property_t *new_property_int(const char *name, const char *desc, int b);
EXPORT bool property_int_set(property_t *p, int b);
EXPORT int property_int_get(property_t *p);

//添加int滑块属性
EXPORT property_t *new_property_int_slider(const char *name, const char *desc, int min, int max, int step, int value, int def);
EXPORT bool property_int_slider_set(property_t *p, int b, enum slider_attribute attr);
EXPORT int property_int_slider_get(property_t *p, enum slider_attribute attr);

//添加list属性
EXPORT property_t  *new_property_list(const char *name, const char *desc, void *data);
/* ------------------------------------------------------------------------- */

/**
 * Optional callback for when a property is modified.  If the properties
 * need to be refreshed due to changes to the property layout, return true,
 * otherwise return false.
 */
typedef bool (*property_modified_t)(properties_t *props,
	property_t *property, obs_data_t *settings);

EXPORT void property_set_modified_callback(property_t *p,
		property_modified_t modified);

//属性修改回调函数
typedef bool(*property_changed_t)(property_t *property, void *data);
EXPORT void property_set_changed_callback(property_t *p, property_changed_t changed);
EXPORT bool property_changed(property_t *p);

EXPORT void property_set_changed_callback_1(property_t *p, property_changed_t changed, size_t index);
EXPORT bool property_changed_1(property_t *p, size_t index);


EXPORT bool property_modified(property_t *p, obs_data_t *settings);

EXPORT bool property_button_clicked(property_t *p, void *obj);

EXPORT void property_set_visible(property_t *p, bool visible);
EXPORT void property_set_enabled(property_t *p, bool enabled);

EXPORT void property_set_description(property_t *p,
		const char *description);

EXPORT const char *           property_name(property_t *p);
EXPORT const char *           property_description(property_t *p);
EXPORT enum property_type property_get_type(property_t *p);
EXPORT bool                   property_enabled(property_t *p);
EXPORT bool                   property_visible(property_t *p);

EXPORT bool                   property_next(property_t **p);

EXPORT double                 property_float_min(property_t *p);
EXPORT double                 property_float_max(property_t *p);
EXPORT double                 property_float_step(property_t *p);
EXPORT enum text_type     proprety_text_type(property_t *p);
EXPORT enum path_type     property_path_type(property_t *p);
EXPORT const char *           property_path_filter(property_t *p);
EXPORT const char *           property_path_default_path(property_t *p);
EXPORT enum combo_type    property_list_type(property_t *p);
EXPORT enum combo_format  property_list_format(property_t *p);

EXPORT void property_list_clear(property_t *p);

//设置list的当前值
EXPORT void property_list_set_current(property_t *p, int idx);
EXPORT int property_list_current(property_t *p);

EXPORT size_t property_list_add_string(property_t *p,
		const char *name, const char *val);
EXPORT size_t property_list_add_int(property_t *p,
		const char *name, long long val);
EXPORT size_t property_list_add_float(property_t *p,
		const char *name, double val);

EXPORT void property_list_insert_string(property_t *p, size_t idx,
		const char *name, const char *val);
EXPORT void property_list_insert_int(property_t *p, size_t idx,
		const char *name, long long val);
EXPORT void property_list_insert_float(property_t *p, size_t idx,
		const char *name, double val);

EXPORT void property_list_item_disable(property_t *p, size_t idx,
								bool disabled);
EXPORT bool property_list_item_disabled(property_t *p, size_t idx);

EXPORT void property_list_item_remove(property_t *p, size_t idx);

EXPORT size_t      property_list_item_count(property_t *p);
EXPORT const char *property_list_item_name(property_t *p, size_t idx);
EXPORT const char *property_list_item_string(property_t *p, size_t idx);
EXPORT long long   property_list_item_int(property_t *p, size_t idx);
EXPORT double      property_list_item_float(property_t *p, size_t idx);

EXPORT bool        property_editable_list_allow_files(property_t *p);
EXPORT const char *property_editable_list_filter(property_t *p);
EXPORT const char *property_editable_list_default_path(property_t *p);

#ifdef __cplusplus
}
#endif
