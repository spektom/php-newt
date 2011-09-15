/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2004 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Michael Spector <michael@php.net>                            |
  +----------------------------------------------------------------------+
*/

/* $Id: php_newt.h 312442 2011-06-24 19:35:17Z michael $ */

#include <newt.h>

#ifndef PHP_NEWT_H
#define PHP_NEWT_H

extern zend_module_entry newt_module_entry;
#define phpext_newt_ptr &newt_module_entry

#define PHP_NEWT_VERSION "1.2.3-dev"

#ifdef PHP_WIN32
#define PHP_NEWT_API __declspec(dllexport)
#else
#define PHP_NEWT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(newt);
PHP_MSHUTDOWN_FUNCTION(newt);
PHP_MINFO_FUNCTION(newt);

PHP_FUNCTION(newt_init);
PHP_FUNCTION(newt_finished);
PHP_FUNCTION(newt_cls);
#ifdef HAVE_NEWT_RESIZE_SCREEN
PHP_FUNCTION(newt_resize_screen);
#endif
PHP_FUNCTION(newt_wait_for_key);
PHP_FUNCTION(newt_clear_key_buffer);
PHP_FUNCTION(newt_delay);
PHP_FUNCTION(newt_open_window);
PHP_FUNCTION(newt_centered_window);
PHP_FUNCTION(newt_pop_window);
PHP_FUNCTION(newt_set_colors);
PHP_FUNCTION(newt_refresh);
PHP_FUNCTION(newt_suspend);
PHP_FUNCTION(newt_set_suspend_callback);
#ifdef HAVE_NEWT_SET_HELP_CALLBACK
PHP_FUNCTION(newt_set_help_callback);
#endif
PHP_FUNCTION(newt_resume);
PHP_FUNCTION(newt_push_help_line);
PHP_FUNCTION(newt_redraw_help_line);
PHP_FUNCTION(newt_pop_help_line);
PHP_FUNCTION(newt_draw_root_text);
PHP_FUNCTION(newt_bell);
#ifdef HAVE_NEWT_CURSOR_OFF
PHP_FUNCTION(newt_cursor_off);
#endif
#ifdef HAVE_NEWT_CURSOR_ON
PHP_FUNCTION(newt_cursor_on);
#endif
PHP_FUNCTION(newt_compact_button);
PHP_FUNCTION(newt_button);
PHP_FUNCTION(newt_checkbox);
PHP_FUNCTION(newt_checkbox_get_value);
PHP_FUNCTION(newt_checkbox_set_value);
PHP_FUNCTION(newt_checkbox_set_flags);
PHP_FUNCTION(newt_radiobutton);
PHP_FUNCTION(newt_radio_get_current);
#ifdef HAVE_NEWT_LISTITEM
PHP_FUNCTION(newt_listitem);
#endif
#ifdef HAVE_NEWT_LISTITEM_SET
PHP_FUNCTION(newt_listitem_set);
#endif
#ifdef HAVE_NEWT_LISTITEM_GET_DATA
PHP_FUNCTION(newt_listitem_get_data);
#endif
PHP_FUNCTION(newt_get_screen_size);
PHP_FUNCTION(newt_label);
PHP_FUNCTION(newt_label_set_text);
PHP_FUNCTION(newt_vertical_scrollbar);
PHP_FUNCTION(newt_scrollbar_set);
PHP_FUNCTION(newt_listbox);
PHP_FUNCTION(newt_listbox_get_current);
PHP_FUNCTION(newt_listbox_set_current);
PHP_FUNCTION(newt_listbox_set_current_by_key);
PHP_FUNCTION(newt_listbox_set_entry);
PHP_FUNCTION(newt_listbox_set_width);
PHP_FUNCTION(newt_listbox_set_data);
PHP_FUNCTION(newt_listbox_append_entry);
PHP_FUNCTION(newt_listbox_insert_entry);
PHP_FUNCTION(newt_listbox_delete_entry);
PHP_FUNCTION(newt_listbox_clear);
PHP_FUNCTION(newt_listbox_get_entry);
PHP_FUNCTION(newt_listbox_get_selection);
PHP_FUNCTION(newt_listbox_clear_selection);
PHP_FUNCTION(newt_listbox_select_item);
#ifdef HAVE_NEWT_LISTBOX_ITEM_COUNT
PHP_FUNCTION(newt_listbox_item_count);
#endif
PHP_FUNCTION(newt_checkbox_tree);
PHP_FUNCTION(newt_checkbox_tree_multi);
PHP_FUNCTION(newt_checkbox_tree_get_selection);
#ifdef HAVE_NEWT_CHECKBOX_TREE_GET_CURRENT
PHP_FUNCTION(newt_checkbox_tree_get_current);
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_CURRENT
PHP_FUNCTION(newt_checkbox_tree_set_current);
#endif
PHP_FUNCTION(newt_checkbox_tree_get_multi_selection);
PHP_FUNCTION(newt_checkbox_tree_add_item);
PHP_FUNCTION(newt_checkbox_tree_add_array);
PHP_FUNCTION(newt_checkbox_tree_find_item);
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY
PHP_FUNCTION(newt_checkbox_tree_set_entry);
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_WIDTH
PHP_FUNCTION(newt_checkbox_tree_set_width);
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_GET_ENTRY_VALUE
PHP_FUNCTION(newt_checkbox_tree_get_entry_value);
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY_VALUE
PHP_FUNCTION(newt_checkbox_tree_set_entry_value);
#endif
PHP_FUNCTION(newt_textbox_reflowed);
PHP_FUNCTION(newt_textbox);
PHP_FUNCTION(newt_textbox_set_text);
PHP_FUNCTION(newt_textbox_set_height);
PHP_FUNCTION(newt_textbox_get_num_lines);
PHP_FUNCTION(newt_reflow_text);
PHP_FUNCTION(newt_form);
#ifdef HAVE_NEWT_FORM_SET_TIMER
PHP_FUNCTION(newt_form_set_timer);
#endif
PHP_FUNCTION(newt_form_watch_fd);
PHP_FUNCTION(newt_form_set_size);
PHP_FUNCTION(newt_form_get_current);
PHP_FUNCTION(newt_form_set_background);
PHP_FUNCTION(newt_form_set_current);
PHP_FUNCTION(newt_form_add_component);
PHP_FUNCTION(newt_form_add_components);
PHP_FUNCTION(newt_form_set_height);
PHP_FUNCTION(newt_form_set_width);
PHP_FUNCTION(newt_run_form);
PHP_FUNCTION(newt_form_run);
PHP_FUNCTION(newt_draw_form);
PHP_FUNCTION(newt_form_add_hot_key);
PHP_FUNCTION(newt_entry);
PHP_FUNCTION(newt_entry_set);
PHP_FUNCTION(newt_entry_set_filter);
PHP_FUNCTION(newt_entry_get_value);
PHP_FUNCTION(newt_entry_set_flags);
PHP_FUNCTION(newt_scale);
PHP_FUNCTION(newt_scale_set);
PHP_FUNCTION(newt_component_add_callback);
PHP_FUNCTION(newt_component_takes_focus);
PHP_FUNCTION(newt_form_destroy);
PHP_FUNCTION(newt_create_grid);
PHP_FUNCTION(newt_grid_v_stacked);
PHP_FUNCTION(newt_grid_v_close_stacked);
PHP_FUNCTION(newt_grid_h_stacked);
PHP_FUNCTION(newt_grid_h_close_stacked);
PHP_FUNCTION(newt_grid_basic_window);
PHP_FUNCTION(newt_grid_simple_window);
PHP_FUNCTION(newt_grid_set_field);
PHP_FUNCTION(newt_grid_place);
PHP_FUNCTION(newt_grid_free);
PHP_FUNCTION(newt_grid_get_size);
PHP_FUNCTION(newt_grid_wrapped_window);
PHP_FUNCTION(newt_grid_wrapped_window_at);
PHP_FUNCTION(newt_grid_add_components_to_form);
PHP_FUNCTION(newt_button_barv);
PHP_FUNCTION(newt_button_bar);
PHP_FUNCTION(newt_win_message);
PHP_FUNCTION(newt_win_messagev);
PHP_FUNCTION(newt_win_choice);
PHP_FUNCTION(newt_win_ternary);
PHP_FUNCTION(newt_win_menu);
PHP_FUNCTION(newt_win_entries);

#ifndef newtComponent_struct
struct newtComponent_struct {
	int height, width;
	int top, left;
	int takesFocus;
	int isMapped;
	struct componentOps *ops;
	newtCallback callback;
	void *callbackData;
	void *data;
};
#endif

typedef struct {
	char *func_name;
	char *key;
	zval *callback;
	zval *data;
}
php_newt_cb;

// Must be greater, that length of hexadecimal representation of memory address
#define PHP_NEWT_RK_SIZE 8 *sizeof(void*)

ZEND_BEGIN_MODULE_GLOBALS (newt)
	HashTable callbacks;
	HashTable data;
	int newt_has_inited;
ZEND_END_MODULE_GLOBALS (newt)

#define REGISTER_NEWT_CONSTANT(__c) REGISTER_LONG_CONSTANT(#__c, __c, CONST_CS | CONST_PERSISTENT)

/* Callback wrappers */
static void newt_comp_callback_wrapper (newtComponent component, void *cb_key);
static int newt_entry_filter_callback_wrapper (newtComponent entry, void *cb_key, int ch, int cursor);
static void newt_suspend_callback_wrapper (void *cb_key);
static void newt_help_callback_wrapper (newtComponent form, char *help);

static void php_newt_init_globals (zend_newt_globals *newt_globals TSRMLS_DC);
static void php_newt_destroy_globals(zend_newt_globals *newt_globals TSRMLS_DC);
void php_newt_free_cb (php_newt_cb **cb_ptr);
int php_newt_fetch_resource (zval *rsrc, void *data, int le_type);

static void newt_call_php_function (INTERNAL_FUNCTION_PARAMETERS, char *func_name, zval **ret_val, int argc, zval ***args);

#ifdef ZTS
#define NEWT_G(v) TSRMG(newt_globals_id, zend_newt_globals *, v)
#else
#define NEWT_G(v) (newt_globals.v)
#endif

#endif	/* PHP_NEWT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
