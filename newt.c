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

/* $Id: newt.c 312452 2011-06-25 09:56:46Z michael $ */

#include <newt.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_newt.h"
#include "newt_vcall.h"

/* True global resources - no need for thread safety here */
static int le_newt_comp;
static char le_newt_comp_name[] = "newt component";
static int le_newt_grid;
static char le_newt_grid_name[] = "newt grid";

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(first_arg_force_ref, 0)
	ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(second_arg_force_ref, 0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(secondandthird_arg_force_ref, 0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(1)
	ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(fifthandsixth_arg_force_ref, 0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(1)
	ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(seventh_arg_force_ref, 0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(eighth_arg_force_ref, 0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(0)
	ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ newt_functions[]
 */
zend_function_entry newt_functions[] = {
	PHP_FE(newt_init,							NULL)
	PHP_FE(newt_finished,						NULL)
	PHP_FE(newt_cls,							NULL)
#ifdef HAVE_NEWT_RESIZE_SCREEN
	PHP_FE(newt_resize_screen,					NULL)
#endif
	PHP_FE(newt_wait_for_key,					NULL)
	PHP_FE(newt_clear_key_buffer,				NULL)
	PHP_FE(newt_delay,							NULL)
	PHP_FE(newt_open_window,					NULL)
	PHP_FE(newt_centered_window,				NULL)
	PHP_FE(newt_pop_window,						NULL)
	PHP_FE(newt_set_colors,						NULL)
	PHP_FE(newt_refresh,						NULL)
	PHP_FE(newt_suspend,						NULL)
	PHP_FE(newt_set_suspend_callback,			NULL)
#ifdef HAVE_NEWT_SET_HELP_CALLBACK
	PHP_FE(newt_set_help_callback,				NULL)
#endif
	PHP_FE(newt_resume,							NULL)
	PHP_FE(newt_push_help_line,					NULL)
	PHP_FE(newt_redraw_help_line,				NULL)
	PHP_FE(newt_pop_help_line,					NULL)
	PHP_FE(newt_draw_root_text,					NULL)
	PHP_FE(newt_bell,							NULL)
#ifdef HAVE_NEWT_CURSOR_OFF
	PHP_FE(newt_cursor_off,						NULL)
#endif
#ifdef HAVE_NEWT_CURSOR_ON
	PHP_FE(newt_cursor_on,						NULL)
#endif
	PHP_FE(newt_compact_button,					NULL)
	PHP_FE(newt_button,							NULL)
	PHP_FE(newt_checkbox,						NULL)
	PHP_FE(newt_checkbox_get_value,				NULL)
	PHP_FE(newt_checkbox_set_value,				NULL)
	PHP_FE(newt_checkbox_set_flags,				NULL)
	PHP_FE(newt_radiobutton,					NULL)
	PHP_FE(newt_radio_get_current,				NULL)
#ifdef HAVE_NEWT_LISTITEM
	PHP_FE(newt_listitem,						NULL)
#endif
#ifdef HAVE_NEWT_LISTITEM_SET
	PHP_FE(newt_listitem_set,					NULL)
#endif
#ifdef HAVE_NEWT_LISTITEM_GET_DATA
	PHP_FE(newt_listitem_get_data,				NULL)
#endif
#ifdef all_args_by_ref
	PHP_FE(newt_get_screen_size,				all_args_by_ref)
#else
	PHP_FE(newt_get_screen_size,				NULL)
#endif
	PHP_FE(newt_label,							NULL)
	PHP_FE(newt_label_set_text,					NULL)
	PHP_FE(newt_vertical_scrollbar,				NULL)
	PHP_FE(newt_scrollbar_set,					NULL)
	PHP_FE(newt_listbox,						NULL)
	PHP_FE(newt_listbox_get_current,			NULL)
	PHP_FE(newt_listbox_set_current,			NULL)
	PHP_FE(newt_listbox_set_current_by_key,		NULL)
	PHP_FE(newt_listbox_set_entry,				NULL)
	PHP_FE(newt_listbox_set_width,				NULL)
	PHP_FE(newt_listbox_set_data,				NULL)
	PHP_FE(newt_listbox_append_entry,			NULL)
	PHP_FALIAS(newt_listbox_add_entry,	newt_listbox_append_entry,	NULL)
	PHP_FE(newt_listbox_insert_entry,			NULL)
	PHP_FE(newt_listbox_delete_entry,			NULL)
	PHP_FE(newt_listbox_clear,					NULL)
	PHP_FE(newt_listbox_get_entry,				NULL)
	PHP_FE(newt_listbox_get_selection,			NULL)
	PHP_FE(newt_listbox_clear_selection,		NULL)
	PHP_FE(newt_listbox_select_item,			NULL)
#ifdef HAVE_NEWT_LISTBOX_ITEM_COUNT
	PHP_FE(newt_listbox_item_count,				NULL)
#endif
	PHP_FE(newt_checkbox_tree,					NULL)
	PHP_FE(newt_checkbox_tree_multi,			NULL)
	PHP_FE(newt_checkbox_tree_get_selection,	NULL)
#ifdef HAVE_NEWT_CHECKBOX_TREE_GET_CURRENT
	PHP_FE(newt_checkbox_tree_get_current,		NULL)
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_CURRENT
	PHP_FE(newt_checkbox_tree_set_current,		NULL)
#endif
	PHP_FE(newt_checkbox_tree_get_multi_selection,	NULL)
	PHP_FE(newt_checkbox_tree_add_item,			NULL)
	PHP_FE(newt_checkbox_tree_add_array,		NULL)
	PHP_FE(newt_checkbox_tree_find_item,		NULL)
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY
	PHP_FE(newt_checkbox_tree_set_entry,		NULL)
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_WIDTH
	PHP_FE(newt_checkbox_tree_set_width,		NULL)
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_GET_ENTRY_VALUE
	PHP_FE(newt_checkbox_tree_get_entry_value,	NULL)
#endif
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY_VALUE
	PHP_FE(newt_checkbox_tree_set_entry_value,	NULL)
#endif
	PHP_FE(newt_textbox_reflowed,				NULL)
	PHP_FE(newt_textbox,						NULL)
	PHP_FE(newt_textbox_set_text,				NULL)
	PHP_FE(newt_textbox_set_height,				NULL)
	PHP_FE(newt_textbox_get_num_lines,			NULL)
	PHP_FE(newt_reflow_text,					fifthandsixth_arg_force_ref)
	PHP_FE(newt_form,							NULL)
#ifdef HAVE_NEWT_FORM_SET_TIMER
	PHP_FE(newt_form_set_timer,					NULL)
#endif
	PHP_FE(newt_form_watch_fd,					NULL)
	PHP_FE(newt_form_set_size,					NULL)
	PHP_FE(newt_form_get_current,				NULL)
	PHP_FE(newt_form_set_background,			NULL)
	PHP_FE(newt_form_set_current,				NULL)
	PHP_FE(newt_form_add_component,				NULL)
	PHP_FE(newt_form_add_components,			NULL)
	PHP_FE(newt_form_set_height,				NULL)
	PHP_FE(newt_form_set_width,					NULL)
	PHP_FE(newt_run_form,						NULL)
	PHP_FE(newt_form_run,						second_arg_force_ref)
	PHP_FE(newt_draw_form,						NULL)
	PHP_FE(newt_form_add_hot_key,				NULL)
	PHP_FE(newt_entry,							NULL)
	PHP_FE(newt_entry_set,						NULL)
	PHP_FE(newt_entry_set_filter,				NULL)
	PHP_FE(newt_entry_get_value,				NULL)
	PHP_FE(newt_entry_set_flags,				NULL)
	PHP_FE(newt_scale,							NULL)
	PHP_FE(newt_scale_set,						NULL)
	PHP_FE(newt_component_add_callback,			NULL)
	PHP_FE(newt_component_takes_focus,			NULL)
	PHP_FE(newt_form_destroy,					NULL)
	PHP_FE(newt_create_grid,					NULL)
	PHP_FE(newt_grid_v_stacked,					NULL)
	PHP_FE(newt_grid_v_close_stacked,			NULL)
	PHP_FE(newt_grid_h_stacked,					NULL)
	PHP_FE(newt_grid_h_close_stacked,			NULL)
	PHP_FE(newt_grid_basic_window,				NULL)
	PHP_FE(newt_grid_simple_window,				NULL)
	PHP_FE(newt_grid_set_field,					NULL)
	PHP_FE(newt_grid_place,						NULL)
	PHP_FE(newt_grid_free,						NULL)
	PHP_FALIAS(newt_grid_destroy, newt_grid_free, NULL)
	PHP_FE(newt_grid_get_size,					secondandthird_arg_force_ref)
	PHP_FE(newt_grid_wrapped_window,			NULL)
	PHP_FE(newt_grid_wrapped_window_at,			NULL)
	PHP_FE(newt_grid_add_components_to_form,	NULL)
	PHP_FE(newt_button_bar,						first_arg_force_ref)
	PHP_FALIAS(newt_button_barv, newt_button_bar, first_arg_force_ref)
	PHP_FE(newt_win_message,					NULL)
	PHP_FE(newt_win_messagev,					NULL)
	PHP_FE(newt_win_choice,						NULL)
	PHP_FE(newt_win_ternary,					NULL)
	PHP_FE(newt_win_menu,						eighth_arg_force_ref)
	PHP_FE(newt_win_entries,					seventh_arg_force_ref)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ newt_module_entry
 */
zend_module_entry newt_module_entry = {
	STANDARD_MODULE_HEADER,
	"newt",
	newt_functions,
	PHP_MINIT(newt),
	PHP_MSHUTDOWN(newt),
	NULL,
	NULL,
	PHP_MINFO(newt),
	PHP_NEWT_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

ZEND_DECLARE_MODULE_GLOBALS (newt)

#ifdef COMPILE_DL_NEWT
ZEND_GET_MODULE(newt)
#endif

/* {{{ php_newt_init_globals
 */
static void php_newt_init_globals (zend_newt_globals *newt_globals TSRMLS_DC)
{
	newt_globals->newt_has_inited = 0;

	zend_hash_init(&newt_globals->callbacks, 0, NULL, (dtor_func_t) php_newt_free_cb, 1);
	zend_hash_init(&newt_globals->data, 0, NULL, ZVAL_PTR_DTOR, 1);
}
/* }}} */

/* {{{ php_newt_destroy_globals
 */
static void php_newt_destroy_globals(zend_newt_globals *newt_globals TSRMLS_DC)
{
	if (newt_globals->newt_has_inited) {
	  newtFinished();
	}

	zend_hash_destroy (&newt_globals->callbacks);
	zend_hash_destroy (&newt_globals->data);
}
/* }}} */

/* {{{ php_newt_free_cb
 */
void php_newt_free_cb (php_newt_cb **cb_ptr)
{
	php_newt_cb *cb = *cb_ptr;
	if(cb) {
		if(cb->key) efree (cb->key);
		if(cb->func_name) efree (cb->func_name);
		zval_ptr_dtor (&cb->callback);
		zval_ptr_dtor (&cb->data);
		efree (cb);
	}
}
/* }}} */

/* {{{ php_newt_fetch_resource (zval *rsrc, void *data, int le_type)
 */
int php_newt_fetch_resource (zval *rsrc, void *data, int le_type)
{
	zend_rsrc_list_entry *le;
	char *key = NULL;
	uint key_len;
   	ulong rsrc_id;

	TSRMLS_FETCH();

	if (!rsrc) {	
	  MAKE_STD_ZVAL (rsrc);
	}

	zend_hash_internal_pointer_reset (&EG(regular_list));
	while (zend_hash_get_current_data (&EG(regular_list), (void **) &le) == SUCCESS) {
		zend_hash_get_current_key_ex(&EG(regular_list), &key, &key_len, &rsrc_id, 0, NULL);
		if(le->type == le_type && le->ptr == data) {
			Z_LVAL_P (rsrc) = rsrc_id;
			Z_TYPE_P (rsrc) = IS_RESOURCE;
			zval_copy_ctor (rsrc);
			return SUCCESS;
		}
		zend_hash_move_forward(&EG(regular_list));
	}
	return FAILURE;
}
/* }}} */

#define PHP_NEWT_STORE_DATA(z_data, key) \
	do { \
		SEPARATE_ZVAL(&z_data); \
		zval_add_ref(&z_data); \
		key = zend_hash_num_elements(&NEWT_G(data)); \
		zend_hash_next_index_insert (&NEWT_G(data), &z_data, sizeof(zval *), NULL); \
	} while (0)

#define PHP_NEWT_FETCH_DATA(key, z_data) \
	do { \
		if (!z_data) { \
			MAKE_STD_ZVAL(z_data); \
		} \
		zval **z_data_pnfk = NULL; \
		if(zend_hash_index_find (&NEWT_G(data), key, (void **)&z_data_pnfk) == SUCCESS) { \
			*z_data = **z_data_pnfk; \
			zval_copy_ctor(z_data); \
		} \
	} while (0)

#define PHP_NEWT_FETCH_KEY(z_data, key) \
	do { \
		zval **z_data_pnfk = NULL, res_pnfk; \
		zend_hash_internal_pointer_reset (&NEWT_G(data)); \
		while (zend_hash_get_current_data (&NEWT_G(data), (void **)&z_data_pnfk) == SUCCESS) { \
			is_equal_function (&res_pnfk, (zval *) *z_data_pnfk, z_data TSRMLS_CC); \
			if(zval_is_true (&res_pnfk)) { \
				zend_hash_get_current_key(&NEWT_G(data), NULL, &key, 0); \
				break; \
			} \
			zend_hash_move_forward(&NEWT_G(data)); \
		} \
	} while (0) 

#define PHP_NEWT_STORE_CALLBACK(cb) \
	do { \
		if (!cb->key) { \
		  cb->key = emalloc (PHP_NEWT_RK_SIZE+1); \
		  snprintf (cb->key, PHP_NEWT_RK_SIZE, "%x", (long) cb); \
		} \
		zend_hash_update(&NEWT_G(callbacks), cb->key, strlen(cb->key)+1, (void *)&cb, sizeof(php_newt_cb *), NULL); \
	} while (0)

#define PHP_NEWT_FETCH_CALLBACK(key, cb) \
	do { \
		php_newt_cb **cb_pnfc = NULL; \
		zend_hash_find (&NEWT_G(callbacks), (char *)key, strlen(key)+1, (void **)&cb_pnfc); \
		cb = *cb_pnfc; \
	} while (0)

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(newt)
{
	le_newt_comp = zend_register_list_destructors_ex(NULL, NULL, le_newt_comp_name, module_number);
	le_newt_grid = zend_register_list_destructors_ex(NULL, NULL, le_newt_grid_name, module_number);

	ZEND_INIT_MODULE_GLOBALS (newt, php_newt_init_globals, php_newt_destroy_globals);

	/* Colorsets */
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ROOT);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_BORDER);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_WINDOW);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_SHADOW);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_TITLE);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_BUTTON);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ACTBUTTON);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_CHECKBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ACTCHECKBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ENTRY);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_LABEL);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_LISTBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ACTLISTBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_TEXTBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ACTTEXTBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_HELPLINE);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ROOTTEXT);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_EMPTYSCALE);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_FULLSCALE);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_DISENTRY);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_COMPACTBUTTON);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_ACTSELLISTBOX);
	REGISTER_NEWT_CONSTANT(NEWT_COLORSET_SELLISTBOX);

	REGISTER_NEWT_CONSTANT(NEWT_ARG_LAST);
	REGISTER_NEWT_CONSTANT(NEWT_ARG_APPEND);

	/* enum newtFlagsSense */
	REGISTER_NEWT_CONSTANT(NEWT_FLAGS_SET);
	REGISTER_NEWT_CONSTANT(NEWT_FLAGS_RESET);
	REGISTER_NEWT_CONSTANT(NEWT_FLAGS_TOGGLE);

	/* Components' flags */
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_RETURNEXIT);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_HIDDEN);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_SCROLL);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_DISABLED);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_BORDER);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_WRAP);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_NOF12);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_MULTIPLE);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_SELECTED);
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_CHECKBOX);
#ifdef NEWT_FLAG_PASSWORD
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_PASSWORD);
#endif
#ifdef NEWT_FLAG_SHOWCURSOR
	REGISTER_NEWT_CONSTANT(NEWT_FLAG_SHOWCURSOR);
#endif
	REGISTER_NEWT_CONSTANT(NEWT_FD_READ);
	REGISTER_NEWT_CONSTANT(NEWT_FD_WRITE);
#ifdef NEWT_FD_EXCEPT
	REGISTER_NEWT_CONSTANT(NEWT_FD_EXCEPT);
#endif

#ifdef NEWT_CHECKBOXTREE_UNSELECTABLE
	REGISTER_NEWT_CONSTANT(NEWT_CHECKBOXTREE_UNSELECTABLE);
#endif
#ifdef NEWT_CHECKBOXTREE_HIDE_BOX
	REGISTER_NEWT_CONSTANT(NEWT_CHECKBOXTREE_HIDE_BOX);
#endif
#ifdef NEWT_CHECKBOXTREE_COLLAPSED
	REGISTER_NEWT_CONSTANT(NEWT_CHECKBOXTREE_COLLAPSED);
#endif
#ifdef NEWT_CHECKBOXTREE_EXPANDED
	REGISTER_NEWT_CONSTANT(NEWT_CHECKBOXTREE_EXPANDED);
#endif
#ifdef NEWT_CHECKBOXTREE_UNSELECTED
	REGISTER_NEWT_CONSTANT(NEWT_CHECKBOXTREE_UNSELECTED);
#endif
#ifdef NEWT_CHECKBOXTREE_SELECTED
	REGISTER_NEWT_CONSTANT(NEWT_CHECKBOXTREE_SELECTED);
#endif

	/* Backwards compatibility */
	REGISTER_NEWT_CONSTANT(NEWT_LISTBOX_RETURNEXIT);
	REGISTER_NEWT_CONSTANT(NEWT_ENTRY_SCROLL);
	REGISTER_NEWT_CONSTANT(NEWT_ENTRY_HIDDEN);
	REGISTER_NEWT_CONSTANT(NEWT_ENTRY_RETURNEXIT);
	REGISTER_NEWT_CONSTANT(NEWT_ENTRY_DISABLED);
	REGISTER_NEWT_CONSTANT(NEWT_TEXTBOX_WRAP);
	REGISTER_NEWT_CONSTANT(NEWT_TEXTBOX_SCROLL);
	REGISTER_NEWT_CONSTANT(NEWT_FORM_NOF12);

	/* Form exit reasons */
	REGISTER_NEWT_CONSTANT(NEWT_EXIT_HOTKEY);
	REGISTER_NEWT_CONSTANT(NEWT_EXIT_COMPONENT);
	REGISTER_NEWT_CONSTANT(NEWT_EXIT_FDREADY);
#ifdef NEWT_EXIT_TIMER
	REGISTER_NEWT_CONSTANT(NEWT_EXIT_TIMER);
#endif

	/* Key codes */
	REGISTER_NEWT_CONSTANT(NEWT_KEY_TAB);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_ENTER);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_SUSPEND);
#ifdef NEWT_KEY_ESCAPE
	REGISTER_NEWT_CONSTANT(NEWT_KEY_ESCAPE);
#endif
	REGISTER_NEWT_CONSTANT(NEWT_KEY_RETURN);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_EXTRA_BASE);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_UP);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_DOWN);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_LEFT);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_RIGHT);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_BKSPC);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_DELETE);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_HOME);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_END);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_UNTAB);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_PGUP);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_PGDN);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_INSERT);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F1);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F2);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F3);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F4);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F5);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F6);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F7);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F8);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F9);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F10);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F11);
	REGISTER_NEWT_CONSTANT(NEWT_KEY_F12);
	/* not really a key, but newtGetKey returns it */
	REGISTER_NEWT_CONSTANT(NEWT_KEY_RESIZE);
	REGISTER_NEWT_CONSTANT(NEWT_ANCHOR_LEFT);
	REGISTER_NEWT_CONSTANT(NEWT_ANCHOR_RIGHT);
	REGISTER_NEWT_CONSTANT(NEWT_ANCHOR_TOP);
	REGISTER_NEWT_CONSTANT(NEWT_ANCHOR_BOTTOM);

	REGISTER_NEWT_CONSTANT(NEWT_GRID_FLAG_GROWX);
	REGISTER_NEWT_CONSTANT(NEWT_GRID_FLAG_GROWY);

	/* enum newtGridElement  */
	REGISTER_NEWT_CONSTANT(NEWT_GRID_EMPTY);
	REGISTER_NEWT_CONSTANT(NEWT_GRID_COMPONENT);
	REGISTER_NEWT_CONSTANT(NEWT_GRID_SUBGRID);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(newt)
{
#ifndef ZTS
	php_newt_destroy_globals (&newt_globals TSRMLS_CC);
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(newt)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "newt support", "enabled");
	php_info_print_table_row(2, "version", PHP_NEWT_VERSION);
	php_info_print_table_end();
}
/* }}} */

/* {{{ newt_comp_callback_wrapper
 */
static void newt_comp_callback_wrapper (newtComponent component, void *cb_key)
{
	zval *args[2];
	zval retval;
	php_newt_cb *cb = NULL;

	TSRMLS_FETCH();

	PHP_NEWT_FETCH_CALLBACK (cb_key, cb);

	/* First argument is component resource */
	MAKE_STD_ZVAL (args[0]);
	php_newt_fetch_resource (args[0], component, le_newt_comp);

	/* Second argument is mixed data */
	args[1] = cb->data;

	if (call_user_function(EG(function_table), NULL, cb->callback, &retval, 2, args TSRMLS_CC) == SUCCESS) {
		zval_dtor(&retval);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call user function: %s(component, data)", cb->func_name);
	}
	zval_ptr_dtor (&args[0]);
}
/* }}} */

/* {{{
 * newt_entry_filter_callback_wrapper (newtComponent entry, void *data, int ch, int cursor)
 */
static int newt_entry_filter_callback_wrapper (newtComponent entry, void *cb_key, int ch, int cursor)
{
	zval *args[4];
	zval retval;
	php_newt_cb *cb = NULL;
	
	TSRMLS_FETCH();

	PHP_NEWT_FETCH_CALLBACK (cb_key, cb);

	/* First argument is component resource */
	MAKE_STD_ZVAL (args[0]);
	php_newt_fetch_resource (args[0], entry, le_newt_comp);

	/* Second argument is mixed data */
	args[1] = cb->data;

	/* Third parameter is single character */
	MAKE_STD_ZVAL (args[2]);
	if (ch >= 0 && ch <= 255) {
		ZVAL_STRING (args[2], (char*)&ch, 1);
	} else {
		ZVAL_LONG (args[2], ch);
	}

	/* Fourth parameter is integer  */
	MAKE_STD_ZVAL (args[3]);
	ZVAL_LONG (args[3], cursor);

	if (call_user_function(EG(function_table), NULL, cb->callback, &retval, 4, args TSRMLS_CC) == FAILURE ) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, 
				"Unable to call user function: %s(component, data, ch, cursor)", cb->func_name);
	} else {
		if (Z_TYPE (retval) == IS_STRING) {
			ch = *Z_STRVAL (retval);
		} else if (Z_TYPE (retval) == IS_LONG) {
			ch = Z_LVAL (retval);
		}
		zval_dtor (&retval);
	}
	zval_ptr_dtor (&args[0]);
	zval_ptr_dtor (&args[2]);
	zval_ptr_dtor (&args[3]);

	return ch;
}
/* }}} */

/* {{{
 * newt_suspend_callback_wrapper (void *data)
 */
static void newt_suspend_callback_wrapper (void *cb_key)
{
	zval *args[1];
	zval retval;
	php_newt_cb *cb = NULL;
	
	TSRMLS_FETCH();

	PHP_NEWT_FETCH_CALLBACK (cb_key, cb);

	/* First argument is mixed data */
	args[0] = cb->data;

	if (call_user_function(EG(function_table), NULL, cb->callback, &retval, 1, args TSRMLS_CC) == SUCCESS) {
		zval_dtor(&retval);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call user function: %s(data)", cb->func_name);
	}
}
/* }}} */

/* {{{
 * newt_help_callback_wrapper (newtComponent form, char *help)
 */
static void newt_help_callback_wrapper (newtComponent form, char *help)
{
	zval *args[2];
	zval retval;
	php_newt_cb *cb = NULL;
	
	TSRMLS_FETCH();

	PHP_NEWT_FETCH_CALLBACK ("php_newt_help_cb_key", cb);

	/* First argument is component resource */
	MAKE_STD_ZVAL (args[0]);
	php_newt_fetch_resource (args[0], form, le_newt_comp);

	/* Second argument is string */
	MAKE_STD_ZVAL (args[1]);
	ZVAL_STRING (args[1], help, 1);

	if (call_user_function(EG(function_table), NULL, cb->callback, &retval, 2, args TSRMLS_CC) == SUCCESS) {
	  php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call user function: %s(form, help)", cb->func_name);
	} else {
	  zval_dtor(&retval);
	}
	
	zval_ptr_dtor (&args[0]);
	zval_ptr_dtor (&args[1]);
}
/* }}} */

/* {{{
 * newt_call_php_function
 */
static void newt_call_php_function (INTERNAL_FUNCTION_PARAMETERS, char *func_name, zval **ret_val, int argc, zval ***args)
{
	zval *z_func_name;

	MAKE_STD_ZVAL(z_func_name);
	ZVAL_STRING(z_func_name, func_name, 1);

	if (call_user_function_ex(EG(function_table), NULL, z_func_name, ret_val, argc, args, 0, NULL TSRMLS_CC) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "An error was occurred while calling to function '%s'", func_name);
		return;
	}
	zval_ptr_dtor(&z_func_name);
}
/* }}} */

/* {{{
 * proto void newt_init()
 */
PHP_FUNCTION(newt_init)
{
	if(ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }

	NEWT_G(newt_has_inited) = 1;
	RETURN_LONG (newtInit());
}
/* }}} */

/* {{{
 * proto void newt_finished()
 */
PHP_FUNCTION(newt_finished)
{
	if(ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }

	NEWT_G(newt_has_inited) = 0;
	RETURN_LONG (newtFinished());
}
/* }}} */

/* {{{
 * proto void newt_cls()
 */
PHP_FUNCTION(newt_cls)
{
	if(ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtCls();
}
/* }}} */

#ifdef HAVE_NEWT_RESIZE_SCREEN
/* {{{
 * proto void newt_resize_screen([bool redraw])
 */
PHP_FUNCTION(newt_resize_screen)
{
	zend_bool redraw = 1;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "|b", &redraw) == FAILURE) {
		return;
	}

	newtResizeScreen (redraw);
}
/* }}} */
#endif

/* {{{
 * proto void newt_wait_for_key()
 */
PHP_FUNCTION(newt_wait_for_key)
{
	if(ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtWaitForKey();
}
/* }}} */

/* {{{
 * proto void newt_clear_key_buffer()
 */
PHP_FUNCTION(newt_clear_key_buffer)
{
	if(ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtClearKeyBuffer();
}
/* }}} */

/* {{{
 * proto void newt_delay (long microseconds)
 */
PHP_FUNCTION(newt_delay)
{
	long usecs;
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "l", &usecs) == FAILURE) {
		return;
	}
	newtDelay (usecs);
}
/* }}} */

/* {{{
 * proto void newt_open_window (long left, long top, long width, long height [, string title])
 */
PHP_FUNCTION(newt_open_window)
{
	long left;
	long top;
   	long width;
   	long height;
	char *title = NULL;
	int title_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llll|s", &left, &top, &width, &height, &title, &title_len) == FAILURE) {
		return;
	}

	RETURN_LONG (newtOpenWindow (left, top, width, height, title));
}
/* }}} */

/* {{{
 * proto void newt_centered_window (long width, long height [, string title])
 */
PHP_FUNCTION(newt_centered_window)
{
   	long width;
   	long height;
	char *title = NULL;
	int title_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "ll|s", &width, &height, &title, &title_len) == FAILURE) {
		return;
	}

	RETURN_LONG (newtCenteredWindow (width, height, title));
}
/* }}} */

/* {{{
 * proto void newt_pop_window()
 */
PHP_FUNCTION(newt_pop_window)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtPopWindow();
}
/* }}} */

/* {{{
 */
PHP_FUNCTION(newt_set_colors)
{
}
/* }}} */

/* {{{
 * proto void newt_refresh()
 */
PHP_FUNCTION(newt_refresh)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtRefresh ();
}
/* }}} */

/* {{{
 * proto void newt_suspend()
 */
PHP_FUNCTION(newt_suspend)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtSuspend ();
}
/* }}} */

/* {{{
 * proto void newt_set_suspend_callback (mixed function, mixed data)
 */
PHP_FUNCTION(newt_set_suspend_callback)
{
	zval *z_callback, *z_data;
	php_newt_cb *cb = NULL;

	cb = emalloc(sizeof(php_newt_cb));
	memset (cb, 0, sizeof(php_newt_cb));
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "zz", &z_callback, &z_data) == FAILURE) {
		efree (cb);
		return;
	}

	if (Z_TYPE_P(z_callback) != IS_STRING && Z_TYPE_P(z_callback) != IS_ARRAY) {
		SEPARATE_ZVAL (&z_callback);
		convert_to_string_ex (&z_callback);
	}

	if (!zend_is_callable(z_callback, 0, &cb->func_name TSRMLS_CC)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s: not valid callback", cb->func_name);
		efree (cb->func_name);
		efree (cb);
		return;
	}

	MAKE_STD_ZVAL(cb->callback);
	*cb->callback = *z_callback;
	zval_copy_ctor(cb->callback);

	MAKE_STD_ZVAL(cb->data);
	*cb->data = *z_data;
	zval_copy_ctor(cb->data);

	PHP_NEWT_STORE_CALLBACK (cb);
	newtSetSuspendCallback ((newtSuspendCallback) newt_suspend_callback_wrapper, cb->key);
}
/* }}} */

/* {{{
 * proto void newt_set_help_callback (mixed function)
 */
#ifdef HAVE_NEWT_SET_HELP_CALLBACK
PHP_FUNCTION(newt_set_help_callback)
{
	zval *z_callback;
	php_newt_cb *cb = NULL;

	cb = emalloc(sizeof(php_newt_cb));
	memset (cb, 0, sizeof(php_newt_cb));
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "z", &z_callback) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(z_callback) != IS_STRING && Z_TYPE_P(z_callback) != IS_ARRAY) {
		SEPARATE_ZVAL (&z_callback);
		convert_to_string_ex (&z_callback);
	}

	if (!zend_is_callable(z_callback, 0, &cb->func_name TSRMLS_CC)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s: not valid callback", cb->func_name);
		efree (cb->func_name);
		efree (cb);
		return;
	}

	MAKE_STD_ZVAL(cb->callback);
	*cb->callback = *z_callback;
	zval_copy_ctor(cb->callback);

	cb->key = estrdup("php_newt_help_cb_key");
	PHP_NEWT_STORE_CALLBACK (cb);
	newtSetHelpCallback ((newtCallback) newt_help_callback_wrapper);
}
#endif
/* }}} */

/* {{{
 * proto void newt_resume()
 */
PHP_FUNCTION(newt_resume)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtResume ();
}
/* }}} */

/* {{{
 * proto void newt_push_help_line ([string text])
 */
PHP_FUNCTION(newt_push_help_line)
{
	char *text = NULL;
	int text_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "|s!", &text, &text_len) == FAILURE) {
		return;
	}

	newtPushHelpLine (text);
}
/* }}} */

/* {{{
 * proto void newt_redraw_help_line()
 */
PHP_FUNCTION(newt_redraw_help_line)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtRedrawHelpLine ();
}
/* }}} */

/* {{{
 * proto void newt_pop_help_line()
 */
PHP_FUNCTION(newt_pop_help_line)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtPopHelpLine ();
}
/* }}} */

/* {{{
 * proto void newt_draw_root_text (long left, long top, string text)
 */
PHP_FUNCTION(newt_draw_root_text)
{
   	long left;
   	long top;
	char *text = NULL;
	int text_len;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lls", &left, &top, &text, &text_len) == FAILURE) {
		return;
	}
	newtDrawRootText (left, top, text);
}
/* }}} */

/* {{{
 * proto void newt_bell()
 */
PHP_FUNCTION(newt_bell)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtBell ();
}
/* }}} */

/* {{{
 * proto void newt_cursor_off()
 */
#ifdef HAVE_NEWT_CURSOR_OFF
PHP_FUNCTION(newt_cursor_off)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtCursorOff ();
}
#endif
/* }}} */

/* {{{
 * proto void newt_cursor_on()
 */
#ifdef HAVE_NEWT_CURSOR_ON
PHP_FUNCTION(newt_cursor_on)
{
	if (ZEND_NUM_ARGS() != 0) { WRONG_PARAM_COUNT; }
	newtCursorOn ();
}
#endif
/* }}} */

/* {{{
 * proto resource newt_compact_button (long left, long top, string text)
 */
PHP_FUNCTION(newt_compact_button)
{
   	long left;
   	long top;
	char *text = NULL;
	int text_len;
	newtComponent button;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lls", &left, &top, &text, &text_len) == FAILURE) {
		return;
	}
	button = newtCompactButton (left, top, text);
	newtComponentAddCallback (button, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, button, le_newt_comp);
}
/* }}} */

/* {{{
 * proto resource newt_button (long left, long top, string text)
 */
PHP_FUNCTION(newt_button)
{
   	long left;
   	long top;
	char *text = NULL;
	int text_len;
	newtComponent button;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lls", &left, &top, &text, &text_len) == FAILURE) {
		return;
	}
	button = newtButton (left, top, text);
	newtComponentAddCallback (button, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, button, le_newt_comp);
}
/* }}} */

/* {{{
 * proto resource newt_checkbox (long left, long top, string text, string def_value [, string seq])
 */
PHP_FUNCTION(newt_checkbox)
{
	long left;
	long top;
	char *text = NULL;
	int text_len;
	char *def_val = NULL;
	int def_val_len = 0;
	char *seq = NULL;
	int seq_len;
	newtComponent checkbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llss|s", &left, &top, &text, &text_len, &def_val, &def_val_len
				, &seq, &seq_len) == FAILURE) {
		return;
	}

	if (def_val_len != 1) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "fourth argument must be single character");
		return;
	}

	checkbox = newtCheckbox(left, top, text, *def_val, seq, NULL);
	newtComponentAddCallback (checkbox, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, checkbox, le_newt_comp);
}
/* }}} */

/* {{{
 * proto string newt_checkbox_get_value (resource checkbox)
 */
PHP_FUNCTION(newt_checkbox_get_value)
{
	zval *z_checkbox;
	newtComponent checkbox;
	char ret_value[2];

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_checkbox) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkbox, newtComponent, &z_checkbox, -1, le_newt_comp_name, le_newt_comp);

	ret_value[0] = newtCheckboxGetValue(checkbox);
	ret_value[1] = '\0';
	RETURN_STRING(ret_value, 1);
}
/* }}} */

/* {{{
 * proto void newt_checkbox_set_value (resource checkbox, string value)
 */
PHP_FUNCTION(newt_checkbox_set_value)
{
	zval *z_checkbox;
	newtComponent checkbox;
	char *value = NULL;
	int value_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_checkbox, &value, &value_len) == FAILURE) {
		return;
	}

	if (value_len != 1) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "second argument must be single character");
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkbox, newtComponent, &z_checkbox, -1, le_newt_comp_name, le_newt_comp);
	newtCheckboxSetValue (checkbox, *value);
}
/* }}} */

/* {{{
 * proto void newt_checkbox_set_flags (resource checkbox, long flags, long sense)
 */
PHP_FUNCTION(newt_checkbox_set_flags)
{
	zval *z_checkbox;
	newtComponent checkbox;
	long flags;
	long sense;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rll", &z_checkbox, &flags, &sense) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(checkbox, newtComponent, &z_checkbox, -1, le_newt_comp_name, le_newt_comp);
	newtCheckboxSetFlags (checkbox, flags, sense);
}
/* }}} */

/* {{{
 * proto resource newt_radiobutton (long left, long top, string text, bool is_default [, resource prev_button])
 */
PHP_FUNCTION(newt_radiobutton)
{
	long left;
	long top;
	char *text = NULL;
	int text_len;
	zend_bool is_default;
	zval *z_prev_button = NULL;
	newtComponent prev_button = NULL;
	newtComponent radiobutton;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llsb|r!", &left, &top, 
				&text, &text_len, &is_default, &z_prev_button) == FAILURE) {
		return;
	}

	if (z_prev_button) {
		ZEND_FETCH_RESOURCE(prev_button, newtComponent, &z_prev_button, -1, le_newt_comp_name, le_newt_comp);
	}

	radiobutton = newtRadiobutton (left, top, text, is_default, prev_button);
	newtComponentAddCallback (radiobutton, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, radiobutton, le_newt_comp);
}
/* }}} */

/* {{{
 * proto resource newt_radio_get_current (resource set_member)
 */
PHP_FUNCTION(newt_radio_get_current)
{
	zval *z_set_member;
	newtComponent set_member;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_set_member) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(set_member, newtComponent, &z_set_member, -1, le_newt_comp_name, le_newt_comp);
	php_newt_fetch_resource (return_value, newtRadioGetCurrent(set_member), le_newt_comp);
}
/* }}} */

#ifdef HAVE_NEWT_LISTITEM
/* {{{
 * proto resource newt_listitem (long left, long top, string text, bool is_default, resouce prev_item, mixed data [, long flags])
 */
PHP_FUNCTION(newt_listitem)
{
   	long left;
   	long top;
	char *text = NULL;
	int text_len;
	zend_bool is_default;
	zval *z_prev_item;
	newtComponent item, prev_item;
	zval *z_data;
	long flags = 0;
	ulong key;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llsbzz|l", &left, &top, &text, &text_len, 
				&is_default, &z_prev_item, &z_data, &flags) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(prev_item, newtComponent, &z_prev_item, -1, le_newt_comp_name, le_newt_comp);

	PHP_NEWT_STORE_DATA (z_data, key);

	item = newtListitem (left, top, text, is_default, prev_item, (void *)key, flags);
	newtComponentAddCallback (item, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, item, le_newt_comp);
}
/* }}} */
#endif

#ifdef HAVE_NEWT_LISTITEM_SET
/* {{{
 * proto void newt_listitem_set (resource item, string text)
 */
PHP_FUNCTION(newt_listitem_set)
{
	zval *z_item;
	newtComponent item;
	char *text = NULL;
	int text_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_item, &text, &text_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(item, newtComponent, &z_item, -1, le_newt_comp_name, le_newt_comp);
	newtListitemSet (item, text);
}
/* }}} */
#endif

#ifdef HAVE_NEWT_LISTITEM_GET_DATA
/* {{{
 * proto mixed newt_listitem_get_data (resource item)
 */
PHP_FUNCTION(newt_listitem_get_data)
{
	zval *z_item;
	newtComponent item;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_item) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(item, newtComponent, &z_item, -1, le_newt_comp_name, le_newt_comp);
	PHP_NEWT_FETCH_DATA((ulong) newtListitemGetData (item), return_value);
}
/* }}} */
#endif

/* {{{
 * proto void newt_get_screen_size (long &cols, long &rows)
 */
PHP_FUNCTION(newt_get_screen_size)
{
	zval *z_cols = NULL, *z_rows = NULL;
	int cols, rows;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "zz", &z_cols, &z_rows) == FAILURE) {
		return;
	}
	
	newtGetScreenSize ((int *)&cols, (int *)&rows);
	
	if (z_cols) {
		zval_dtor (z_cols);
		ZVAL_LONG (z_cols, cols);
	}
	if (z_rows) {
		zval_dtor (z_rows);
		ZVAL_LONG (z_rows, rows);
	}
}
/* }}} */

/* {{{
 * proto resource newt_label (long left, long top, string text)
 */
PHP_FUNCTION(newt_label)
{
   	long left;
   	long top;
	char *text = NULL;
	int text_len;
	newtComponent label;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lls", &left, &top, &text, &text_len) == FAILURE) {
		return;
	}
	label = newtLabel (left, top, text);
	newtComponentAddCallback (label, NULL, NULL);
	
	ZEND_REGISTER_RESOURCE (return_value, label, le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_label_set_text (resource label, string text)
 */
PHP_FUNCTION(newt_label_set_text)
{
	zval *z_label;
	newtComponent label;
	char *text = NULL;
	int text_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_label, &text, &text_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(label, newtComponent, &z_label, -1, le_newt_comp_name, le_newt_comp);

	newtLabelSetText (label, text);
}
/* }}} */

/* {{{
 * proto resource newt_vertical_scrollbar (long left, long top, long height [, long normal_colorset [, long thumb_colorset]])
 */
PHP_FUNCTION(newt_vertical_scrollbar)
{
	long left, top, height;
	long normalColorset = NEWT_COLORSET_WINDOW, thumbColorset = NEWT_COLORSET_ACTCHECKBOX;
	newtComponent scrollbar;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lll|ll", &left, &top, &height, 
				&normalColorset, &thumbColorset) == FAILURE) {
		return;
	}

	scrollbar = newtVerticalScrollbar (left, top, height, normalColorset, thumbColorset);
	newtComponentAddCallback (scrollbar, NULL, NULL);
	
	ZEND_REGISTER_RESOURCE (return_value, scrollbar, le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_scrollbar_set (resource scrollbar, long where, long total)
 */
PHP_FUNCTION(newt_scrollbar_set)
{
	zval *z_scrollbar;
	long where, total;
	newtComponent scrollbar;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rll", &z_scrollbar, &where, &total) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(scrollbar, newtComponent, &z_scrollbar, -1, le_newt_comp_name, le_newt_comp);
	newtScrollbarSet (scrollbar, where, total);
}
/* }}} */

/* {{{
 * proto resource newt_listbox (long left, long top, long height [, long flags])
 */
PHP_FUNCTION(newt_listbox)
{
   	long left, top, height, flags = 0;
	newtComponent listbox;
	
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lll|l", &left, &top, &height, &flags) == FAILURE) {
		return;
	}

	listbox = newtListbox (left, top, height, flags);
	newtComponentAddCallback (listbox, NULL, NULL);
	
	ZEND_REGISTER_RESOURCE (return_value, listbox, le_newt_comp);
}
/* }}} */

/* {{{
 * proto string newt_listbox_get_current (resource listbox)
 */
PHP_FUNCTION(newt_listbox_get_current)
{
	zval *z_listbox;
	newtComponent listbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_listbox) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	PHP_NEWT_FETCH_DATA((ulong) newtListboxGetCurrent (listbox), return_value);
}
/* }}} */

/* {{{
 * proto void newt_listbox_set_current (resource listbox, long num)
 */
PHP_FUNCTION(newt_listbox_set_current)
{
	zval *z_listbox;
	newtComponent listbox;
	long num;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_listbox, &num) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxSetCurrent (listbox, num);
}
/* }}} */

/* {{{
 * proto void newt_listbox_set_current_by_key (resource listbox, mixed key)
 */
PHP_FUNCTION(newt_listbox_set_current_by_key)
{
	zval *z_listbox;
	newtComponent listbox;
	zval *z_key;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz!", &z_listbox, &z_key) == FAILURE) {
		return;
	}

	PHP_NEWT_FETCH_KEY (z_key, key);

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxSetCurrentByKey (listbox, (void *)key);
}
/* }}} */

/* {{{
 * proto void newt_listbox_set_entry (resource listbox, long num, string text)
 */
PHP_FUNCTION(newt_listbox_set_entry)
{
	zval *z_listbox;
	newtComponent listbox;
	long num;
	char *text = NULL;
	int text_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rls", &z_listbox, &num, &text, &text_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxSetEntry (listbox, num, text);
}
/* }}} */

/* {{{
 * proto void newt_listbox_set_width (resource listbox, long width)
 */
PHP_FUNCTION(newt_listbox_set_width)
{
	zval *z_listbox;
	newtComponent listbox;
	long width;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_listbox, &width) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);

	newtListboxSetWidth (listbox, width);
}
/* }}} */

/* {{{
 * proto void newt_listbox_set_data (resource listbox, long num, mixed data)
 */
PHP_FUNCTION(newt_listbox_set_data)
{
	zval *z_listbox;
	newtComponent listbox;
	long num;
	zval *z_data;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rlz!", &z_listbox, &num, &z_data) == FAILURE) {
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_data, key);

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxSetData (listbox, num, (void *)key);
}
/* }}} */

/* {{{
 * proto void newt_listbox_append_entry (resource listbox, string text, mixed data)
 */
PHP_FUNCTION(newt_listbox_append_entry)
{
	zval *z_listbox;
	newtComponent listbox;
	char *text = NULL;
	int text_len;
	zval *z_data;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rsz!", &z_listbox, &text, &text_len, &z_data) == FAILURE) {
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_data, key);

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxAppendEntry (listbox, text, (void *)key);
}
/* }}} */

/* {{{
 * proto void newt_listbox_insert_entry (resource listbox, string text, mixed data, mixed key)
 */
PHP_FUNCTION(newt_listbox_insert_entry)
{
	zval *z_listbox;
	newtComponent listbox;
	char *text = NULL;
	int text_len;
	zval *z_data, *z_key;
	ulong data_key, key_key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rsz!z!", &z_listbox, &text, &text_len, &z_data, &z_key) == FAILURE) {
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_data, data_key);
	PHP_NEWT_STORE_DATA (z_key, key_key);

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxInsertEntry (listbox, text, (void *)data_key, (void *)key_key);
}
/* }}} */

/* {{{
 * proto void newt_listbox_delete_entry (resource listbox, mixed key)
 */
PHP_FUNCTION(newt_listbox_delete_entry)
{
	zval *z_listbox;
	newtComponent listbox;
	zval *z_key;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz!", &z_listbox, &z_key) == FAILURE) {
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_key, key);

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxDeleteEntry (listbox, (void *)key);
}
/* }}} */

/* {{{
 * proto void newt_listbox_clear (resource listobx)
 */
PHP_FUNCTION(newt_listbox_clear)
{
	zval *z_listbox;
	newtComponent listbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_listbox) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxClear (listbox);
}
/* }}} */

/* {{{
 * newt_listbox_get_entry (resource listbox, long num [, string &text [, mixed &data]])
 */
PHP_FUNCTION(newt_listbox_get_entry)
{
	zval *z_listbox;
	newtComponent listbox;
	long num;
	zval *z_text = NULL, *z_data = NULL;
	char *text = NULL;
	ulong key;

	if (zend_parse_parameters
			(ZEND_NUM_ARGS() TSRMLS_CC, "rl|z!z!", &z_listbox, &num, &z_text, &z_data) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);

	newtListboxGetEntry (listbox, num, &text, (void **)&key);
	if (z_text) {
		zval_dtor (z_text);
		if (text) {
			ZVAL_STRING (z_text, text, 1);
		}
	}
	if (z_data) {
		zval_dtor (z_data);
		PHP_NEWT_FETCH_DATA (key, z_data);
	}
}
/* }}} */

/* {{{
 * proto array newt_listbox_get_selection (resource listbox)
 */
PHP_FUNCTION(newt_listbox_get_selection)
{
	zval *z_listbox;
	newtComponent listbox;
	ulong *retval;
	zval *z_val;
	int num_items;
	int i;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_listbox) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	retval = (ulong *)newtListboxGetSelection (listbox, &num_items);

	array_init (return_value);
	if (retval) {
		for (i=0; i < num_items; i++ ) {
			PHP_NEWT_FETCH_DATA (retval[i], z_val);
			zend_hash_next_index_insert (Z_ARRVAL_P(return_value), &z_val, sizeof(zval *), NULL);
			z_val = NULL;
		}
		free (retval);
	}
}
/* }}} */

/* {{{
 * proto void newt_listbox_clear_selection (resource listbox)
 */
PHP_FUNCTION(newt_listbox_clear_selection)
{
	zval *z_listbox;
	newtComponent listbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_listbox) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxClearSelection (listbox);
}
/* }}} */

/* {{{
 * proto void newt_listbox_select_item (resource listbox, mixed key, long sense)
 */
PHP_FUNCTION(newt_listbox_select_item)
{
	zval *z_listbox;
	newtComponent listbox;
	zval *z_key;
	long sense;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz!l", &z_listbox, &z_key, &sense) == FAILURE) {
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_key, key);

	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	newtListboxSelectItem (listbox, (void *)key, sense);
}
/* }}} */

#ifdef HAVE_NEWT_LISTBOX_ITEM_COUNT
/* {{{
 * proto int newt_listbox_item_count (resource listbox)
 */
PHP_FUNCTION(newt_listbox_item_count)
{
	zval *z_listbox;
	newtComponent listbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_listbox) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(listbox, newtComponent, &z_listbox, -1, le_newt_comp_name, le_newt_comp);
	RETURN_LONG (newtListboxItemCount (listbox));
}
/* }}} */
#endif

/* {{{
 * proto resource newt_checkbox_tree (long left, long top, long height [, long flags])
 */
PHP_FUNCTION(newt_checkbox_tree)
{
	newtComponent checkboxtree;
	long left;
	long top;
	long height;
	long flags = 0;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lll|l", &left, &top, &height, &flags) == FAILURE) {
		return;
	}

	checkboxtree = newtCheckboxTree (left, top, height, flags);
	newtComponentAddCallback (checkboxtree, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, checkboxtree, le_newt_comp);
}
/* }}} */

/* {{{
 * proto resource newt_checkbox_tree_multi (long left, long top, long height, string seq [, long flags])
 */
PHP_FUNCTION(newt_checkbox_tree_multi)
{
	newtComponent checkboxtree;
	long left;
	long top;
	long height;
	char *seq = NULL;
	int seq_len;
	long flags = 0;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llls!|l", &left, &top, &height, &seq, &seq_len, &flags) == FAILURE) {
		return;
	}

	checkboxtree = newtCheckboxTreeMulti (left, top, height, seq, flags);
	newtComponentAddCallback (checkboxtree, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, checkboxtree, le_newt_comp);
}
/* }}} */

/* {{{
 * proto array newt_checkbox_tree_get_selection (resource checkboxtree)
 */
PHP_FUNCTION(newt_checkbox_tree_get_selection)
{
	zval *z_checkboxtree;
	newtComponent checkboxtree;
	ulong *retval;
	zval *z_val;
	int num_items;
	int i;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_checkboxtree) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);
	retval = (ulong *)newtCheckboxTreeGetSelection (checkboxtree, &num_items);

	array_init (return_value);
	if (retval) {
		for (i=0; i < num_items; i++ ) {
			PHP_NEWT_FETCH_DATA (retval[i], z_val);
			zend_hash_next_index_insert (Z_ARRVAL_P(return_value), &z_val, sizeof(zval *), NULL);
			z_val = NULL;
		}
		free (retval);
	}
}
/* }}} */

/* {{{
 * proto mixed newt_checkbox_tree_get_current (resource checkboxtree)
 */
#ifdef HAVE_NEWT_CHECKBOX_TREE_GET_CURRENT
PHP_FUNCTION(newt_checkbox_tree_get_current)
{
	zval *z_checkboxtree;
	newtComponent checkboxtree;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_checkboxtree) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);
	PHP_NEWT_FETCH_DATA ((ulong) newtCheckboxTreeGetCurrent (checkboxtree), return_value);
}
#endif
/* }}} */

/* {{{
 * proto void newt_checkbox_tree_set_current (resource checkboxtree, mixed data)
 */
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_CURRENT
PHP_FUNCTION(newt_checkbox_tree_set_current)
{
	zval *z_checkboxtree, *z_data;
	newtComponent checkboxtree;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz", &z_checkboxtree, &z_data) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);

	PHP_NEWT_STORE_DATA (z_data, key);
	newtCheckboxTreeSetCurrent (checkboxtree, (void *)key);
}
#endif
/* }}} */

/* {{{
 * proto array newt_checkbox_tree_get_multi_selection (resource checkboxtree, string seqnum)
 */
PHP_FUNCTION(newt_checkbox_tree_get_multi_selection)
{
	zval *z_checkboxtree;
	newtComponent checkboxtree;
	ulong *retval;
	zval *z_val;
	int num_items;
	int i;
	char *seqnum = NULL;
	int seqnum_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs!", &z_checkboxtree, &seqnum, &seqnum_len) == FAILURE) {
		return;
	}

	if (seqnum_len > 0 && seqnum_len != 1) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Second argument must be a single character");
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);
	retval = (ulong *)newtCheckboxTreeGetMultiSelection (checkboxtree, &num_items, (seqnum ? *seqnum : 0));

	array_init (return_value);
	if (retval) {
		for (i=0; i < num_items; i++ ) {
			PHP_NEWT_FETCH_DATA (retval[i], z_val);
			zend_hash_next_index_insert (Z_ARRVAL_P(return_value), &z_val, sizeof(zval *), NULL);
			z_val = NULL;
		}
		free (retval);
	}
}
/* }}} */

/* {{{
 * proto void newt_checkbox_tree_add_item(resource checkboxtree, string text, mixed data, long flags, long index [, long index [,...]])
 */
PHP_FUNCTION(newt_checkbox_tree_add_item)
{
	zval *z_checkboxtree, *z_data, ***args;
	newtComponent checkboxtree;
	char *text;
	int text_len, i;
	long flags;
	void **newt_args = NULL;
	ulong key;

	int argc = ZEND_NUM_ARGS();
	if (argc < 5) { WRONG_PARAM_COUNT; }
	if (zend_parse_parameters (argc TSRMLS_CC, "rszl", &z_checkboxtree, &text, &text_len, &z_data, &flags) == FAILURE) {
		return;
	}
	
	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);

	PHP_NEWT_STORE_DATA (z_data, key);

	newt_args = (void **) safe_emalloc (argc, sizeof(void *), 0);
	newt_args[0] = (void *)checkboxtree;
	newt_args[1] = (void *)text;
	newt_args[2] = (void *)key;
	newt_args[3] = (void *)flags;
	
	for (i=4; i<argc; i++) {
		if (Z_TYPE_PP(args[i]) != IS_LONG) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Arguments starting from fifth must be integers");
			return;
		}
		newt_args[i] = (void *)Z_LVAL_PP(args[i]);
	}

	newt_vcall ((void *)newtCheckboxTreeAddItem, newt_args, argc);

	efree (newt_args);
	efree (args);
}
/* }}} */

/* {{{
 * proto void newt_checkbox_tree_add_item(resource checkboxtree, string text, mixed data, long flags, array indexes)
 * */
PHP_FUNCTION(newt_checkbox_tree_add_array)
{
	zval *z_checkboxtree, *z_data, *z_indexes, *z_index;
	newtComponent checkboxtree;
	char *text;
	int text_len, *indexes, indexes_num, i;
	long flags;
	void **newt_args = NULL;
	ulong key;

	if (5 != ZEND_NUM_ARGS()) { WRONG_PARAM_COUNT; }
	if (zend_parse_parameters (5 TSRMLS_CC, "rszla", &z_checkboxtree, &text, &text_len, &z_data, &flags, &z_indexes) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);

	PHP_NEWT_STORE_DATA (z_data, key);

	indexes_num = zend_hash_num_elements (Z_ARRVAL_P(z_indexes));
	indexes = (int *) safe_emalloc (indexes_num + 1, sizeof(int), 0);

	zend_hash_internal_pointer_reset (Z_ARRVAL_P(z_indexes));
	i=0;
	while (zend_hash_get_current_data (Z_ARRVAL_P(z_indexes), (void **)&z_index) == SUCCESS) {
		if (Z_TYPE_P(z_index) != IS_LONG) {
			efree (indexes);
			efree (newt_args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Array must contain integers only");
			return;
		}
		indexes[i] = Z_LVAL_P(z_index);
		i++;
		zend_hash_move_forward(Z_ARRVAL_P(z_indexes));
	}
	indexes[i] = NEWT_ARG_LAST;

	newt_args = (void **) safe_emalloc (5, sizeof(void *), 0);
	newt_args[0] = (void *)checkboxtree;
	newt_args[1] = (void *)text;
	newt_args[2] = (void *)key;
	newt_args[3] = (void *)flags;
	newt_args[4] = (void *)indexes;
	
	newt_vcall ((void *)newtCheckboxTreeAddArray, newt_args, 5);

	efree (indexes);
	efree (newt_args);
}
/* }}} */

/* {{{
 * proto array newt_checkbox_tree_find_item (resource checkboxtree, mixed data)
 */
PHP_FUNCTION(newt_checkbox_tree_find_item)
{
	zval *z_checkboxtree, *z_data, *z_val;
	newtComponent checkboxtree;
	int *path, i;
	ulong key;

	if (2 != ZEND_NUM_ARGS()) { WRONG_PARAM_COUNT; }
	if (zend_parse_parameters (2 TSRMLS_CC, "rz", &z_checkboxtree, &z_data) != FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);

	PHP_NEWT_STORE_DATA (z_data, key);
	path = newtCheckboxTreeFindItem (checkboxtree, (void *)key);

	array_init (return_value);
	if (path) {
		MAKE_STD_ZVAL (z_val);
		i=0;
		while (path[i] != NEWT_ARG_LAST) {
			ZVAL_LONG (z_val, path[i]);
			zval_add_ref (&z_val);
			zend_hash_next_index_insert (Z_ARRVAL_P(return_value), &z_val, sizeof(zval *), NULL);
			SEPARATE_ZVAL (&z_val);
		}
		free (path);
	}
}
/* }}} */

/* {{{
 * proto void newt_checkbox_tree_set_entry (resource checkboxtree, mixed data, string text)
 */
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY
PHP_FUNCTION(newt_checkbox_tree_set_entry)
{
	zval *z_checkbox_tree;
	newtComponent checkbox_tree;
	zval *z_data;
	char *text = NULL;
	int text_len;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rzs", &z_checkbox_tree, &z_data, &text, &text_len) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkbox_tree, newtComponent, &z_checkbox_tree, -1, le_newt_comp_name, le_newt_comp);

	PHP_NEWT_STORE_DATA (z_data, key);
	newtCheckboxTreeSetEntry (checkbox_tree, (void *)key, text);
}
#endif
/* }}} */

/* {{{
 * proto void newt_checkbox_tree_set_width (resource checkbox_tree, long width)
 */
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_WIDTH
PHP_FUNCTION(newt_checkbox_tree_set_width)
{
	zval *z_checkbox_tree;
	newtComponent checkbox_tree;
	long width;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_checkbox_tree, &width) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(checkbox_tree, newtComponent, &z_checkbox_tree, -1, le_newt_comp_name, le_newt_comp);

	newtCheckboxTreeSetWidth (checkbox_tree, width);
}
#endif
/* }}} */

/* {{{
 * proto string newt_checkbox_tree_get_entry_value (resource checkboxtree, mixed data)
 */
#ifdef HAVE_NEWT_CHECKBOX_TREE_GET_ENTRY_VALUE
PHP_FUNCTION(newt_checkbox_tree_get_entry_value)
{
	zval *z_checkboxtree, *z_data;
	newtComponent checkboxtree;
	char ret_value[2];
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz!", &z_checkboxtree, &z_data) == FAILURE) {
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_data, key);

	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);
	ret_value[0] = newtCheckboxTreeGetEntryValue (checkboxtree, (void *)key);
	ret_value[1] = '\0';
	
	RETURN_STRING (ret_value, 1);
}
#endif
/* }}} */

/* {{{
 * proto void newt_checkbox_tree_set_entry_value (resource checkboxtree, mixed data, string value)
 */
#ifdef HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY_VALUE
PHP_FUNCTION(newt_checkbox_tree_set_entry_value)
{
	zval *z_checkboxtree, *z_data;
	newtComponent checkboxtree;
	char *value;
	int value_len;
	ulong key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz!s", &z_checkboxtree, &z_data, &value, &value_len) == FAILURE) {
		return;
	}

	if (value_len != 1) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Third argument must be a single character");
		return;
	}
	
	PHP_NEWT_STORE_DATA (z_data, key);

	ZEND_FETCH_RESOURCE(checkboxtree, newtComponent, &z_checkboxtree, -1, le_newt_comp_name, le_newt_comp);
	newtCheckboxTreeSetEntryValue (checkboxtree, (void *)key, *value);
}
#endif
/* }}} */

/* {{{
 * proto resource newt_textbox_reflowed (long left, long top, char *text, long width, long flex_down, long flex_up [, long flags])
 */
PHP_FUNCTION(newt_textbox_reflowed)
{
	long left;
	long top;
	char *text = NULL;
	int text_len;
	long width;
	long flex_down;
	long flex_up;
	long flags = 0;
	newtComponent textbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llslll|l", &left, &top, &text, &text_len, 
				&width, &flex_down, &flex_up, &flags) == FAILURE) {
		return;
	}

	textbox = newtTextboxReflowed (left, top, text, width, flex_down, flex_up, flags);
	newtComponentAddCallback (textbox, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, textbox, le_newt_comp);
}
/* }}} */

/* {{{
 * proto resource newt_textbox (long left, long top, long width, long height [, long flags])
 */
PHP_FUNCTION(newt_textbox)
{
	long left;
	long top;
	long width;
	long height;
	long flags = 0;
	newtComponent textbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llll|l", &left, &top, &width, &height, &flags) == FAILURE) {
		return;
	}

	textbox = newtTextbox(left, top, width, height, flags);
	newtComponentAddCallback (textbox, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, textbox, le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_texbox_set_text (resource textbox, string text)
 */
PHP_FUNCTION(newt_textbox_set_text)
{
	zval *z_textbox;
	newtComponent textbox;
	char *text = NULL;
	int text_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_textbox, &text, &text_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(textbox, newtComponent, &z_textbox, -1, le_newt_comp_name, le_newt_comp);
	newtTextboxSetText (textbox, text);
}
/* }}} */

/* {{{
 * proto void newt_textbox_set_height (resource textbox, long height)
 */
PHP_FUNCTION(newt_textbox_set_height)
{
	zval *z_textbox;
	newtComponent textbox;
	long height;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_textbox, &height) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(textbox, newtComponent, &z_textbox, -1, le_newt_comp_name, le_newt_comp);
	newtTextboxSetHeight (textbox, height);
}
/* }}} */

/* {{{
 * proto int newt_textbox_get_num_lines (resource textbox)
 */
PHP_FUNCTION(newt_textbox_get_num_lines)
{
	zval *z_textbox;
	newtComponent textbox;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_textbox) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(textbox, newtComponent, &z_textbox, -1, le_newt_comp_name, le_newt_comp);
	RETURN_LONG (newtTextboxGetNumLines (textbox));
}
/* }}} */

/* {{{
 * proto string newt_reflow_text (string text, long width, long flex_down, long flex_up, long &actual_width, long &actual_height)
 */
PHP_FUNCTION(newt_reflow_text)
{
	zval *z_actual_width = NULL, *z_actual_height = NULL;
	int actual_width, actual_height;
	char *text = NULL;
	int text_len;
	long width;
	long flex_down, flex_up;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "slllzz", &text, &text_len, &width, &flex_down,
				&flex_up, &z_actual_width, &z_actual_height) == FAILURE) {
		return;
	}
	
	text = newtReflowText (text, width, flex_down, flex_up, (int *)&actual_width, (int *)&actual_height);
	
	if (z_actual_width) {
		zval_dtor (z_actual_width);
		ZVAL_LONG (z_actual_width, actual_width);
	}
	if (z_actual_height) {
		zval_dtor (z_actual_height);
		ZVAL_LONG (z_actual_height, actual_height);
	}

	RETURN_STRING (text, 1);
}
/* }}} */

/* {{{
 * proto resource newt_form ([resource vert_bar [, string help [, long flags]]])
 */
PHP_FUNCTION(newt_form)
{
   zval *z_vert_bar = NULL;
   char *help = NULL;
   int help_len;
   long flags = 0;
   newtComponent vert_bar = NULL;
   newtComponent form;

   if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "|z!s!l", &z_vert_bar, &help, &help_len, &flags) == FAILURE) {
	   return;
   }
   
   if (z_vert_bar) {
	   ZEND_FETCH_RESOURCE(vert_bar, newtComponent, &z_vert_bar, -1, le_newt_comp_name, le_newt_comp);
   }
   form = newtForm (vert_bar, help, flags); 
   newtComponentAddCallback (form, NULL, NULL);

   ZEND_REGISTER_RESOURCE (return_value, form, le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_form_set_timer (resource form, long milliseconds)
 */
#ifdef HAVE_NEWT_FORM_SET_TIMER
PHP_FUNCTION(newt_form_set_timer)
{
	zval *z_form;
	newtComponent form;
	long milliseconds;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_form, &milliseconds) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormSetTimer (form, milliseconds);
}
#endif
/* }}} */

/* {{{
 * proto void newt_form_watch_fd (resource form, resource stream [, long flags])
 */
PHP_FUNCTION(newt_form_watch_fd)
{
	zval *z_form, *z_stream;
	newtComponent form;
	php_stream * stream;
	int fd;
	long flags = 0;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rr|l", &z_form, &z_stream, &flags) == FAILURE) {
		return;
	}
	
	php_stream_from_zval (stream, &z_stream);

	if (php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT, (void*)&fd, 1) == SUCCESS && fd >= 0) {
		ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
		newtFormWatchFd (form, fd, flags);	
	}
	else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not cast stream to a file descriptor");
		return;
	}
}
/* }}} */

/* {{{
 * proto void newt_form_set_size (resource form)
 */
PHP_FUNCTION(newt_form_set_size)
{
	zval *z_form;
	newtComponent form;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_form) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormSetSize (form);
}
/* }}} */

/* {{{
 * proto resource newt_form_get_current (resource form)
 */
PHP_FUNCTION(newt_form_get_current)
{
	zval *z_form;
	newtComponent form;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_form) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	php_newt_fetch_resource (return_value, newtFormGetCurrent(form), le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_form_set_background (resource from, long background)
 */
PHP_FUNCTION(newt_form_set_background)
{
	zval *z_form;
	newtComponent form;
	long background;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_form, &background) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);

	newtFormSetBackground (form, background);
}
/* }}} */

/* {{{
 * nwet_form_set_current (resource form, resource current)
 */
PHP_FUNCTION(newt_form_set_current)
{
	zval *z_form, *z_current;
	newtComponent form;
	newtComponent current;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rr", &z_form, &z_current) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	ZEND_FETCH_RESOURCE(current, newtComponent, &z_current, -1, le_newt_comp_name, le_newt_comp);

	newtFormSetCurrent (form, current);
}
/* }}} */

/* {{{
 * proto void newt_form_add_component (resource form, resource component)
 */
PHP_FUNCTION(newt_form_add_component)
{
	zval *z_form, *z_component;
	newtComponent form;
	newtComponent component;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rr", &z_form, &z_component) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	ZEND_FETCH_RESOURCE(component, newtComponent, &z_component, -1, le_newt_comp_name, le_newt_comp);

	newtFormAddComponent (form, component);
}
/* }}} */

/* {{{
 * proto void newt_form_add_components (resource form, array components)
 */
PHP_FUNCTION(newt_form_add_components)
{
	zval *z_form, *z_components;
	zval ** z_component;
	newtComponent form;
	newtComponent component;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "ra", &z_form, &z_components) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);

	zend_hash_internal_pointer_reset (Z_ARRVAL_P(z_components));

	while (zend_hash_get_current_data (Z_ARRVAL_P(z_components), (void **)&z_component) == SUCCESS) {
		ZEND_FETCH_RESOURCE (component, newtComponent, z_component, -1, le_newt_comp_name, le_newt_comp);
		newtFormAddComponent (form, component);
		zend_hash_move_forward(Z_ARRVAL_P(z_components));
	}
}
/* }}} */

/* {{{
 * proto void newt_form_set_height (resource form, long height)
 */
PHP_FUNCTION(newt_form_set_height)
{
	zval *z_form;
	newtComponent form;
	long height;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_form, &height) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormSetHeight (form, height);
}
/* }}} */

/* {{{
 * proto void newt_form_set_width (resource form, long width)
 */
PHP_FUNCTION(newt_form_set_width)
{
	zval *z_form;
	newtComponent form;
	long width;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_form, &width) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormSetWidth (form, width);
}
/* }}} */

/* {{{
 * proto resource newt_run_form (resource form)
 */
PHP_FUNCTION(newt_run_form)
{
	zval *z_form = NULL;
	newtComponent form;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_form) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	php_newt_fetch_resource (return_value, newtRunForm(form), le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_form_run (resource form, array &exit_struct)
 */
PHP_FUNCTION(newt_form_run)
{
	zval *z_form = NULL;
	zval *z_es = NULL;
	newtComponent form;
	struct newtExitStruct es;
	zval *z_reason, *z_watch, *z_key, *z_component;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rz", &z_form, &z_es) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(z_es) != IS_ARRAY) {
		zval_dtor(z_es);
		array_init(z_es);
	}

	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormRun (form, &es);

	MAKE_STD_ZVAL(z_reason);
	ZVAL_LONG(z_reason, es.reason);
	zend_hash_update (HASH_OF(z_es), "reason", sizeof("reason"), (void *)&z_reason, sizeof(zval *), NULL);

#ifndef MISSING_NEWTEXITSTRUCT_U_WATCH
	MAKE_STD_ZVAL(z_watch);
	ZVAL_LONG(z_watch, es.u.watch);
	zend_hash_update (HASH_OF(z_es), "watch", sizeof("watch"), (void *)&z_watch, sizeof(zval *), NULL);
#endif

	MAKE_STD_ZVAL(z_key);
	ZVAL_LONG(z_key, es.u.key);
	zend_hash_update (HASH_OF(z_es), "key", sizeof("key"), (void *)&z_key, sizeof(zval *), NULL);

	MAKE_STD_ZVAL(z_component);
	php_newt_fetch_resource (z_component, es.u.co, le_newt_comp);
	zend_hash_update (HASH_OF(z_es), "component", sizeof("component"), (void *)&z_component, sizeof(zval *), NULL);
}
/* }}} */

/* {{{
 * proto void newt_draw_form (resource form)
 */
PHP_FUNCTION(newt_draw_form)
{
	zval *z_form = NULL;
	newtComponent form;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_form) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtDrawForm (form);
}
/* }}} */

/* {{{
 * proto void newt_form_add_host_key (resource form, long key)
 */
PHP_FUNCTION(newt_form_add_hot_key)
{
	zval *z_form = NULL;
	newtComponent form;
	long key;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_form, &key) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormAddHotKey (form, key);
}
/* }}} */

/* {{{
 * proto resource newt_entry (long left, long top, long width [, string init_value, [, long flags]])
 */
PHP_FUNCTION(newt_entry)
{
	newtComponent entry;
	long left;
	long top;
	long width;
	char *init_value = NULL;
	int init_value_len;
	long flags = 0;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "lll|s!l", &left, &top, &width, 
				&init_value, &init_value_len, &flags) == FAILURE) {
		return;
	}

	entry = newtEntry (left, top, init_value, width, NULL, flags);
	newtComponentAddCallback (entry, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, entry, le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_entry_set (resource entry, string value [, bool cursor_at_end])
 */
PHP_FUNCTION(newt_entry_set)
{
	zval *z_entry;
	newtComponent entry;
	char *value = NULL;
	int value_len;
	zend_bool cursor_at_end = 0;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs|b", &z_entry, &value, &value_len, &cursor_at_end) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE (entry, newtComponent, &z_entry, -1, le_newt_comp_name, le_newt_comp);
	newtEntrySet (entry, value, cursor_at_end);
}
/* }}} */

/* {{{
 * proto void newt_entry_set_filter (resource entry, callback filter, mixed data)
 */
PHP_FUNCTION(newt_entry_set_filter)
{
	zval *z_callback, *z_data, *z_entry;
	newtComponent entry;
	php_newt_cb *cb = NULL;

	cb = emalloc(sizeof(php_newt_cb));
	memset (cb, 0, sizeof(php_newt_cb));
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &z_entry, &z_callback, &z_data) == FAILURE) {
		efree (cb);
		return;
	}

	if (Z_TYPE_P(z_callback) != IS_STRING && Z_TYPE_P(z_callback) != IS_ARRAY) {
		SEPARATE_ZVAL (&z_callback);
		convert_to_string_ex (&z_callback);
	}

	if (!zend_is_callable(z_callback, 0, &cb->func_name TSRMLS_CC)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s: not valid callback", cb->func_name);
		efree (cb->func_name);
		efree (cb);
		return;
	}

	MAKE_STD_ZVAL(cb->callback);
	*cb->callback = *z_callback;
	zval_copy_ctor(cb->callback);

	MAKE_STD_ZVAL(cb->data);
	*cb->data = *z_data;
	zval_copy_ctor(cb->data);

	PHP_NEWT_STORE_CALLBACK (cb);

	ZEND_FETCH_RESOURCE(entry, newtComponent, &z_entry, -1, le_newt_comp_name, le_newt_comp);
	newtEntrySetFilter (entry, (newtEntryFilter) newt_entry_filter_callback_wrapper, cb->key);
}
/* }}} */

/* {{{
 * proto string newt_entry_get_value (resource entry)
 */
PHP_FUNCTION(newt_entry_get_value)
{
	zval *z_entry;
	newtComponent entry;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_entry) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(entry, newtComponent, &z_entry, -1, le_newt_comp_name, le_newt_comp);

	RETURN_STRING(newtEntryGetValue(entry), 1);
}
/* }}} */

/* {{{
 * proto void newt_entry_set_flags (resource entry, long flags, long sense)
 */
PHP_FUNCTION(newt_entry_set_flags)
{
	zval *z_entry;
	newtComponent entry;
	long flags;
	long sense;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rll", &z_entry, &flags, &sense) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(entry, newtComponent, &z_entry, -1, le_newt_comp_name, le_newt_comp);
	newtEntrySetFlags (entry, flags, sense);
}
/* }}} */

/* {{{
 * proto resource newt_scale (long left, long top, long width, long full_value)
 */
PHP_FUNCTION(newt_scale)
{
	newtComponent scale;
	long left;
	long top;
	long width;
	long full_value;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "llll", &left, &top, &width, &full_value) == FAILURE) {
		return;
	}

	scale = newtScale (left, top, width, full_value);
	newtComponentAddCallback (scale, NULL, NULL);

	ZEND_REGISTER_RESOURCE (return_value, scale, le_newt_comp);
}
/* }}} */

/* {{{
 * proto void newt_scale_set (resource scale, long amount)
 */
PHP_FUNCTION(newt_scale_set)
{
	zval *z_scale;
	newtComponent scale;
	long amount;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_scale, &amount) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE (scale, newtComponent, &z_scale, -1, le_newt_comp_name, le_newt_comp);
	newtScaleSet (scale, amount);
}
/* }}} */

/* {{{
 * proto void newt_component_add_callback (resource component, mixed func_name, mixed data)
 */
PHP_FUNCTION(newt_component_add_callback)
{
	zval *z_callback, *z_data, *z_component;
	newtComponent component;
	php_newt_cb *cb = NULL;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &z_component, &z_callback, &z_data) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(z_callback) != IS_STRING && Z_TYPE_P(z_callback) != IS_ARRAY) {
		SEPARATE_ZVAL (&z_callback);
		convert_to_string_ex (&z_callback);
	}

	cb = emalloc(sizeof(php_newt_cb));
	memset (cb, 0, sizeof(php_newt_cb));

	if (!zend_is_callable(z_callback, 0, &cb->func_name TSRMLS_CC)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s: not valid callback", cb->func_name);
		efree (cb->func_name);
		efree (cb);
		return;
	}

	MAKE_STD_ZVAL(cb->callback);
	*cb->callback = *z_callback;
	zval_copy_ctor(cb->callback);

	MAKE_STD_ZVAL(cb->data);
	*cb->data = *z_data;
	zval_copy_ctor(cb->data);

	PHP_NEWT_STORE_CALLBACK (cb);

	ZEND_FETCH_RESOURCE(component, newtComponent, &z_component, -1, le_newt_comp_name, le_newt_comp);
	newtComponentAddCallback (component, (newtCallback) newt_comp_callback_wrapper, cb->key);
}
/* }}} */

/* {{{
 * proto void newt_component_takes_focus (resource component, bool takes_focus)
 */
PHP_FUNCTION(newt_component_takes_focus)
{
	zval *z_component;
	newtComponent component;
	zend_bool takes_focus;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rb", &z_component, &takes_focus) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(component, newtComponent, &z_component, -1, le_newt_comp_name, le_newt_comp);
	newtComponentTakesFocus (component, takes_focus);
}
/* }}} */

/* {{{
 */
PHP_FUNCTION(newt_form_destroy)
{
	zval *z_form;
	newtComponent form;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_form) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);
	newtFormDestroy (form);

	zend_hash_clean (&NEWT_G(data));
	zend_hash_clean (&NEWT_G(callbacks));
}
/* }}} */

/* {{{
 * proto resource newt_create_grid (long cols, long rows)
 */
PHP_FUNCTION(newt_create_grid)
{
	long cols;
	long rows;
	newtGrid grid;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "ll", &cols, &rows) == FAILURE) {
		return;
	}
	
	grid = newtCreateGrid (cols, rows);
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto resource newt_grid_v_stacked(long element1_type, resource element1 [, ...])
 */
PHP_FUNCTION(newt_grid_v_stacked)
{
	zval ***args;
	void **newt_args = NULL;
	int i;
	newtGrid grid;
	newtComponent element;

	int argc = ZEND_NUM_ARGS();
	if (argc < 2) { WRONG_PARAM_COUNT; }

	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_args = (void **) safe_emalloc(argc, sizeof(void *), 0);
	for (i=0; i<argc; i+=2) {
		if(Z_TYPE_PP(args[i]) != IS_LONG) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Odd arguments must be integers");
			return;
		}
		newt_args[i] = (void *)Z_LVAL_PP(args[i]);
		if(Z_TYPE_PP(args[i+1]) != IS_RESOURCE) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Even arguments must be resources");
			return;
		}
		ZEND_FETCH_RESOURCE(element, newtComponent, args[i+1], -1, le_newt_comp_name, le_newt_comp);
		newt_args[i+1] = (void *)element;
	}

	grid = (newtGrid) newt_vcall((void *)newtGridVStacked, newt_args, argc);

	efree (args);
	efree (newt_args);
	
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto resource newt_grid_v_close_stacked(long element1_type, resource element1 [, ...])
 */
PHP_FUNCTION(newt_grid_v_close_stacked)
{
	zval ***args;
	void **newt_args = NULL;
	int i;
	newtGrid grid;
	newtComponent element;

	int argc = ZEND_NUM_ARGS();
	if (argc < 2) { WRONG_PARAM_COUNT; }

	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_args = (void **) safe_emalloc(argc, sizeof(void *), 0);
	for (i=0; i<argc; i+=2) {
		if(Z_TYPE_PP(args[i]) != IS_LONG) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Odd arguments must be integers");
			return;
		}
		newt_args[i] = (void *)Z_LVAL_PP(args[i]);
		if(Z_TYPE_PP(args[i+1]) != IS_RESOURCE) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Even arguments must be resources");
			return;
		}
		ZEND_FETCH_RESOURCE(element, newtComponent, args[i+1], -1, le_newt_comp_name, le_newt_comp);
		newt_args[i+1] = (void *)element;
	}

	grid = (newtGrid) newt_vcall((void *)newtGridVCloseStacked, newt_args, argc);

	efree (args);
	efree (newt_args);
	
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto resource newt_grid_h_stacked(long element1_type, resource element1 [, ...])
 */
PHP_FUNCTION(newt_grid_h_stacked)
{
	zval ***args;
	void **newt_args = NULL;
	int i;
	newtGrid grid;
	newtComponent element;

	int argc = ZEND_NUM_ARGS();
	if (argc < 2) { WRONG_PARAM_COUNT; }

	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_args = (void **) safe_emalloc (argc, sizeof(void *), 0);
	for (i=0; i<argc; i+=2) {
		if(Z_TYPE_PP(args[i]) != IS_LONG) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Odd arguments must be integers");
			return;
		}
		newt_args[i] = (void *)Z_LVAL_PP(args[i]);
		if(Z_TYPE_PP(args[i+1]) != IS_RESOURCE) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Even arguments must be resources");
			return;
		}
		ZEND_FETCH_RESOURCE(element, newtComponent, args[i+1], -1, le_newt_comp_name, le_newt_comp);
		newt_args[i+1] = (void *)element;
	}

	grid = (newtGrid) newt_vcall((void *)newtGridHStacked, newt_args, argc);

	efree (args);
	efree (newt_args);
	
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto resource newt_grid_h_close_stacked(long element1_type, resource element1 [, ...])
 */
PHP_FUNCTION(newt_grid_h_close_stacked)
{
	zval ***args;
	void **newt_args = NULL;
	int i;
	newtGrid grid;
	newtComponent element;

	int argc = ZEND_NUM_ARGS();
	if (argc < 2) { WRONG_PARAM_COUNT; }

	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_args = (void **) safe_emalloc (argc, sizeof(void *), 0);
	for (i=0; i<argc; i+=2) {
		if(Z_TYPE_PP(args[i]) != IS_LONG) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Odd arguments must be integers");
			return;
		}
		newt_args[i] = (void *)Z_LVAL_PP(args[i]);
		if(Z_TYPE_PP(args[i+1]) != IS_RESOURCE) {
			efree (newt_args);
			efree (args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Even arguments must be resources");
			return;
		}
		ZEND_FETCH_RESOURCE(element, newtComponent, args[i+1], -1, le_newt_comp_name, le_newt_comp);
		newt_args[i+1] = (void *)element;
	}

	grid = (newtGrid) newt_vcall((void *)newtGridHCloseStacked, newt_args, argc);

	efree (args);
	efree (newt_args);
	
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto resource newt_grid_basic_window (resource text, resource middle, resource buttons)
 */
PHP_FUNCTION(newt_grid_basic_window)
{
	zval *z_text, *z_middle, *z_buttons;
	newtComponent text;
	newtGrid middle, buttons, grid;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rrr", &z_text, &z_middle, &z_buttons) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(text, newtComponent, &z_text, -1, le_newt_comp_name, le_newt_comp);
	ZEND_FETCH_RESOURCE(middle, newtGrid, &z_middle, -1, le_newt_grid_name, le_newt_grid);
	ZEND_FETCH_RESOURCE(buttons, newtGrid, &z_buttons, -1, le_newt_grid_name, le_newt_grid);

	grid = newtGridBasicWindow (text, middle, buttons);
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto resource newt_grid_simple_window (resource text, resource middle, resource buttons)
 */
PHP_FUNCTION(newt_grid_simple_window)
{
	zval *z_text, *z_middle, *z_buttons;
	newtComponent text, middle;
	newtGrid buttons, grid;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rrr", &z_text, &z_middle, &z_buttons) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(text, newtComponent, &z_text, -1, le_newt_comp_name, le_newt_comp);
	ZEND_FETCH_RESOURCE(middle, newtComponent, &z_middle, -1, le_newt_comp_name, le_newt_comp);
	ZEND_FETCH_RESOURCE(buttons, newtGrid, &z_buttons, -1, le_newt_grid_name, le_newt_grid);

	grid = newtGridSimpleWindow (text, middle, buttons);
	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto void newt_grid_set_field (resource grid, long col, long row, long type, resource val, long pad_left, long pad_top, long pad_right, long pad_bottom, long anchor [, long flags])
 */
PHP_FUNCTION(newt_grid_set_field)
{
	zval *z_grid;
	newtGrid grid;
	long col, row, type;
	zval *z_val;
	newtComponent val;
	long pad_left, pad_top, pad_right, pad_bottom, anchor, flags = 0;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rlllrlllll|l", &z_grid, &col, &row, &z_val, &pad_left,
				&pad_top, &pad_right, &pad_bottom, &anchor, &flags) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	ZEND_FETCH_RESOURCE(val, newtComponent, &z_val, -1, le_newt_comp_name, le_newt_comp);
	newtGridSetField (grid, col, row, type, val, pad_left, pad_top, pad_right, pad_bottom, anchor, flags);
}
/* }}} */

/* {{{
 * proto void newt_grid_place (resource grid, long left, long top)
 */
PHP_FUNCTION(newt_grid_place)
{
	zval *z_grid;
	newtGrid grid;
	long left, top;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rll", &z_grid, &left, &top) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	newtGridPlace (grid, left, top);
}
/* }}} */

/* {{{
 * proto void newt_grid_free (resource grid, bool recurse)
 */
PHP_FUNCTION(newt_grid_free)
{
	zval *z_grid;
	newtGrid grid;
	zend_bool recurse;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rb", &z_grid, &recurse) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	newtGridFree (grid, recurse);
}
/* }}} */

/* {{{
 * proto void newt_grid_get_size (resouce grid, long &width, long &height)
 */
PHP_FUNCTION(newt_grid_get_size)
{
	zval *z_grid;
	newtGrid grid;
	zval *z_width = NULL, *z_height = NULL;
	int width, height;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &z_grid, &z_width, &z_height) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	newtGridGetSize (grid, (int *)&width, (int *)&height);
	
	if (z_width) {
		zval_dtor (z_width);
		ZVAL_LONG (z_width, width);
	}
	if (z_height) {
		zval_dtor (z_height);
		ZVAL_LONG (z_height, height);
	}
}
/* }}} */

/* {{{
 * proto void newt_grid_wrapped_window (resource grid, string title)
 */
PHP_FUNCTION(newt_grid_wrapped_window)
{
	zval *z_grid;
	newtGrid grid;
	char *title = NULL;
	int title_len;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_grid, &title, &title_len) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	newtGridWrappedWindow (grid, title);
}
/* }}} */

/* {{{
 * proto void newt_grid_wrapped_window_at (resource grid, string title, long left, long top)
 */
PHP_FUNCTION(newt_grid_wrapped_window_at)
{
	zval *z_grid;
	newtGrid grid;
	char *title = NULL;
	int title_len;
	long left, top;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rsll", &z_grid, &title, &title_len, &left, &top) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	newtGridWrappedWindowAt (grid, title, left, top);
}
/* }}} */

/* {{{
 * proto void newt_grid_add_components_to_form (resource grid, resource form, bool recurse)
 */
PHP_FUNCTION(newt_grid_add_components_to_form)
{
	zval *z_grid, *z_form;
	newtGrid grid;
	newtComponent form;
	zend_bool recurse;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "rrb", &z_grid, &z_form, &recurse) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(grid, newtGrid, &z_grid, -1, le_newt_grid_name, le_newt_grid);
	ZEND_FETCH_RESOURCE(form, newtComponent, &z_form, -1, le_newt_comp_name, le_newt_comp);

	newtGridAddComponentsToForm (grid, form, recurse);
}
/* }}} */

/* {{{
 * proto resource newt_button_bar(array &buttons)
 */
PHP_FUNCTION(newt_button_bar)
{
	newtComponent *buttons = NULL;
	void **newt_args = NULL;
	zval *z_buttons, **z_button;
	newtGrid grid;
	char *button_text;
	ulong button_i;
	int buttons_num, i, bi;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a/", &z_buttons) == FAILURE) {
		return;
	}
	
	buttons_num = zend_hash_num_elements (Z_ARRVAL_P(z_buttons));

	newt_args = (void **) safe_emalloc (buttons_num * 2, sizeof(void *), 0);
	buttons = (newtComponent *) safe_emalloc (buttons_num, sizeof(newtComponent), 0);

	i=bi=0;
	zend_hash_internal_pointer_reset (Z_ARRVAL_P(z_buttons));

	while (zend_hash_get_current_data (Z_ARRVAL_P(z_buttons), (void **)&z_button) == SUCCESS) {
		if (zend_hash_get_current_key(Z_ARRVAL_P(z_buttons), &button_text, &button_i, 0) == HASH_KEY_IS_STRING) {
			newt_args[i] = (void *)button_text;
		} else {
			efree (newt_args);
			efree (buttons);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Array index must be a string value");
			return;
		}
		buttons[bi] = NULL;
		newt_args[i+1] = (void *)&buttons[bi];
		bi++;
		i+=2;
		zend_hash_move_forward(Z_ARRVAL_P(z_buttons));
	}

	grid = (newtGrid) newt_vcall ((void *)newtButtonBar, newt_args, buttons_num*2);

	for (bi=0, i=0; bi<buttons_num; i+=2, bi++) {
		if (buttons[bi]) {
			zval *z_tmp;
			MAKE_STD_ZVAL (z_tmp);
			ZEND_REGISTER_RESOURCE (z_tmp, buttons[bi], le_newt_comp);
			zval_add_ref (&z_tmp);
			zend_hash_update (Z_ARRVAL_P(z_buttons), newt_args[i], strlen(newt_args[i])+1, (void **)&z_tmp, sizeof(zval *), NULL);
		}
	}
	zend_hash_internal_pointer_reset (Z_ARRVAL_P(z_buttons));

	efree (newt_args);
	efree (buttons);

	ZEND_REGISTER_RESOURCE (return_value, grid, le_newt_grid);
}
/* }}} */

/* {{{
 * proto void newt_win_message (string title, string button_text, string format [, mixed args [, mixed...]])
 */
PHP_FUNCTION(newt_win_message)
{
	char *title, *button_text, *text;
	int title_len, button_text_len;
	zval ***args, *z_text = NULL;
	
	int argc = ZEND_NUM_ARGS();
	if (argc < 3) { WRONG_PARAM_COUNT; }

	if (zend_parse_parameters (2 TSRMLS_CC, "ss", &title, &title_len,
				&button_text, &button_text_len) == FAILURE) {
		return;
	}
	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_call_php_function (INTERNAL_FUNCTION_PARAM_PASSTHRU, "sprintf", &z_text, argc-2, args+2);
	text = Z_STRVAL_P(z_text);

	if (text) {
	  newtWinMessage (title, button_text, text);
	}
	efree (args);
}
/* }}} */

/* {{{
 * proto void newt_win_messagev (string title, string button_text, string format, array args)
 */
PHP_FUNCTION(newt_win_messagev)
{
	char *title, *button_text, *text;
	int title_len, button_text_len;
	zval **args[4], *z_text = NULL;
	int argc = 4;
	
	if (argc != ZEND_NUM_ARGS()) { WRONG_PARAM_COUNT; }
	
	if (zend_parse_parameters (2 TSRMLS_CC, "ss", &title, &title_len,
				&button_text, &button_text_len) == FAILURE) {
		return;
	}
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		return;
	}

	newt_call_php_function (INTERNAL_FUNCTION_PARAM_PASSTHRU, "vsprintf", &z_text, argc-2, args+2);
	text = Z_STRVAL_P(z_text);

	if (text) {
	  newtWinMessage (title, button_text, text);
	}
}
/* }}} */

/* {{{
 * proto int newt_win_choice (string title, string button1_text, string button2_text, string format [, mixed args [, mixed...]])
 */
PHP_FUNCTION(newt_win_choice)
{
	char *title, *button1_text, *button2_text, *text;
	int title_len, button1_text_len, button2_text_len;
	zval ***args, *z_text = NULL;
	int button_id = 0;
	
	int argc = ZEND_NUM_ARGS();
	if (argc < 4) { WRONG_PARAM_COUNT; }

	if (zend_parse_parameters (3 TSRMLS_CC, "sss", &title, &title_len, &button1_text, &button1_text_len,
				&button2_text, &button2_text_len) == FAILURE) {
		return;
	}
	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_call_php_function (INTERNAL_FUNCTION_PARAM_PASSTHRU, "sprintf", &z_text, argc-3, args+3);
	text = Z_STRVAL_P(z_text);
		
	if (text) {
	  button_id = newtWinChoice (title, button1_text, button2_text, text);
	}
	efree (args);

	RETURN_LONG (button_id);
}
/* }}} */

/* {{{
 * proto int newt_win_ternary (string title, string button1_text, string button2_text, string button3_text, string format [, mixed args [, mixed...]])
 */
PHP_FUNCTION(newt_win_ternary)
{
	char *title, *button1_text, *button2_text, *button3_text, *text;
	int title_len, button1_text_len, button2_text_len, button3_text_len;
	zval ***args, *z_text = NULL;
	int button_id = 0;
	
	int argc = ZEND_NUM_ARGS();
	if (argc < 5) { WRONG_PARAM_COUNT; }

	if (zend_parse_parameters (4 TSRMLS_CC, "ssss", &title, &title_len, &button1_text, &button1_text_len,
				&button2_text, &button2_text_len, &button3_text, &button3_text_len) == FAILURE) {
		return;
	}
	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	newt_call_php_function (INTERNAL_FUNCTION_PARAM_PASSTHRU, "sprintf", &z_text, argc-4, args+4);
	text = Z_STRVAL_P(z_text);
		
	if (text) {
	  button_id = newtWinTernary (title, button1_text, button2_text, button3_text, text);
	}
	efree (args);

	RETURN_LONG (button_id);
}
/* }}} */

/* {{{
 * proto int newt_win_menu (string title, string text, long suggestedWidth, long flexDown, long flexUp, long maxListHeight, array items, long &listItem [, string button1, string...])
 */
PHP_FUNCTION(newt_win_menu)
{
	char *title, *text;
	int title_len, text_len;
	long suggested_width, flex_down, flex_up, max_list_height;
	zval *z_items, **z_item, *z_listitem, ***args;
	char **items = NULL;
	int num_items, i, listitem;
	long rc;
	void **newt_args = NULL;
	
	int argc = ZEND_NUM_ARGS();
	if (argc < 8) { WRONG_PARAM_COUNT; }

	if (zend_parse_parameters (8 TSRMLS_CC, "ssllllaz", &title, &title_len, &text, &text_len,
				&suggested_width, &flex_down, &flex_up, &max_list_height, &z_items, &z_listitem) == FAILURE) {
		return;
	}
	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	zend_hash_internal_pointer_reset (Z_ARRVAL_P(z_items));
	num_items = zend_hash_num_elements (Z_ARRVAL_P(z_items));

	items = (char **) safe_emalloc (num_items+1, sizeof(char *), 0);
	i = 0;
	
	while (zend_hash_get_current_data (Z_ARRVAL_P(z_items), (void **)&z_item) == SUCCESS) {
		if (Z_TYPE_PP(z_item) != IS_STRING) {
			efree (args);
			efree (items);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Seventh argument must be array of strings");
			return;
		}
		items[i++] = Z_STRVAL_PP(z_item);
		zend_hash_move_forward(Z_ARRVAL_P(z_items));
	}
	items[i] = NULL;

	if(z_listitem) {
		listitem = Z_LVAL_P(z_listitem);
	}

	newt_args = (void **) safe_emalloc (argc, sizeof(void *), 0);
	newt_args[0] = (void *)title;
	newt_args[1] = (void *)text;
	newt_args[2] = (void *)suggested_width;
	newt_args[3] = (void *)flex_down;
	newt_args[4] = (void *)flex_up;
	newt_args[5] = (void *)max_list_height;
	newt_args[6] = (void *)items;
	newt_args[7] = (void *)&listitem;

	for (i=8; i<argc; i++) {
		if (Z_TYPE_PP(args[i]) != IS_STRING) {
			efree (args);
			efree (items);
			efree (newt_args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "All arguments starting from ninth must be strings");
			return;
		}
		newt_args[i] = (void *)Z_STRVAL_PP(args[i]);
	}

	rc = (long) newt_vcall ((void *)newtWinMenu, newt_args, argc);

	if (z_listitem) {
		zval_dtor (z_listitem);
		ZVAL_LONG (z_listitem, listitem);
	}
	efree (args);
	efree (newt_args);
	efree (items);

	RETURN_LONG (rc);
}
/* }}} */

/* {{{
 * proto int newt_win_entries (string title, string text, long suggested_width, long flex_down, long flex_up, long data_width, array &items, string button1 [, string button2 [,...]])
 */
PHP_FUNCTION(newt_win_entries)
{
	char *title, *text;
	int title_len, text_len;
	long suggested_width, flex_down, flex_up, data_width;
	zval ***args, *z_items, **z_item;
	void **newt_args = NULL;
	struct newtWinEntry *items = NULL;
	int num_items, i;
	long rc;
	char **strs;

	int argc = ZEND_NUM_ARGS();
	if (argc < 8) { WRONG_PARAM_COUNT; }
	if (zend_parse_parameters (7 TSRMLS_CC, "sslllla/", &title, &title_len, &text, &text_len,
				&suggested_width, &flex_down, &flex_up, &data_width, &z_items) == FAILURE) {
		return;
	}

	args = (zval ***) safe_emalloc (argc, sizeof(zval **), 0);
	if (zend_get_parameters_array_ex (argc, args) == FAILURE) {
		efree (args);
		return;
	}

	zend_hash_internal_pointer_reset (Z_ARRVAL_P(z_items));
	num_items = zend_hash_num_elements (Z_ARRVAL_P(z_items));
	items = (struct newtWinEntry *) safe_emalloc (num_items+1, sizeof(struct newtWinEntry), 0);

	strs = safe_emalloc (num_items, sizeof(char *), 0);

	i=0;
	while (zend_hash_index_find (Z_ARRVAL_P(z_items), i, (void **)&z_item) == SUCCESS) {
		zval **z_item_text = NULL, **z_item_value = NULL, **z_item_flags = NULL;

		if(Z_TYPE_PP(z_item) != IS_ARRAY) {
			efree (args);
			efree (items);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Seventh argument must be array containing arrays");
			return;
		}
		if(zend_hash_find (Z_ARRVAL_PP(z_item), "text", sizeof("text"), (void **)&z_item_text) == FAILURE) {
			efree (args);
			efree (items);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Sub-array must have key: \"text\"");
			return;
		}
		if(zend_hash_find (Z_ARRVAL_PP(z_item), "value", sizeof("value"), (void **)&z_item_value) == FAILURE) {
			efree (args);
			efree (items);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Sub-array must have key: \"value\"");
			return;
		}
		zend_hash_find (Z_ARRVAL_PP(z_item), "flags", sizeof("flags"), (void **)&z_item_flags);

		if(Z_TYPE_PP(z_item_text) != IS_STRING) {
			efree (args);
			efree (items);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Value associated with key: \"text\" must be string");
			return;
		}
		if(z_item_flags) {
			if (Z_TYPE_PP(z_item_flags) != IS_LONG) {
				efree (args);
				efree (items);
				php_error_docref (NULL TSRMLS_CC, E_ERROR, "Value associated with key: \"flags\" must be integer");
				return;
			}
		}

		items[i].text = Z_STRVAL_PP(z_item_text);
		strs[i] = Z_STRVAL_PP(z_item_value);
		items[i].value = (const char **)&strs[i];
		items[i].flags = (z_item_flags ? Z_LVAL_PP(z_item_flags) : 0);

		i++;
	}
	if (i == 0) {
		php_error_docref (NULL TSRMLS_CC, E_ERROR, "Seventh argument must be non-empty array (containing arrays)");
		return;
	}
	items[i].text = NULL; // original function finishes processing items on null text entry
	
	newt_args = (void **) safe_emalloc (argc, sizeof(void *), 0);
	newt_args[0] = (void *)title;
	newt_args[1] = (void *)text;
	newt_args[2] = (void *)suggested_width;
	newt_args[3] = (void *)flex_down;
	newt_args[4] = (void *)flex_up;
	newt_args[5] = (void *)data_width;
	newt_args[6] = (void *)items;

	for(i=7; i<argc; i++) {
		if(Z_TYPE_PP(args[i]) != IS_STRING) {
			efree (args);
			efree (items);
			efree (newt_args);
			php_error_docref (NULL TSRMLS_CC, E_ERROR, "Arguments starting from eighth must be strings");
			return;
		}
		newt_args[i] = (void *)Z_STRVAL_PP(args[i]);
	}

	rc = (long) newt_vcall ((void *)newtWinEntries, newt_args, argc);

	for(i=0; i<num_items; i++) {
		if (items[i].text && zend_hash_index_find (Z_ARRVAL_P(z_items), i, (void **)&z_item) == SUCCESS)
		{
			zval *z_item_value = NULL, **z_item_value_in = NULL;

			if(zend_hash_find (Z_ARRVAL_PP(z_item), "value", sizeof("value"), (void **)&z_item_value_in) == SUCCESS && PZVAL_IS_REF (*z_item_value_in)) {
				zval_dtor(*z_item_value_in);
				ZVAL_STRING (*z_item_value_in, (char *)*(items[i].value), 1);
			} else {
				MAKE_STD_ZVAL (z_item_value);
				ZVAL_STRING (z_item_value, (char *)*(items[i].value), 1);
				zval_add_ref (&z_item_value);
				zend_hash_update (Z_ARRVAL_PP(z_item), "value", sizeof("value"), (void *)&z_item_value, sizeof(zval *), NULL);
			}
		}
	}
	
	efree(strs);
	efree (args);
	efree (items);
	efree (newt_args);

	RETURN_LONG (rc);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
