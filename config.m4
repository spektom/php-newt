dnl $Id: config.m4 331808 2013-10-13 17:54:46Z remi $

PHP_ARG_WITH(newt, for newt support,
[  --with-newt             Include newt support (CLI/CGI only)], no)

PHP_ARG_WITH(curses-dir, for the location of libcurses,
[  --with-curses-dir[=DIR]     NEWT: Set the path to libcurses install prefix.], no, no)

PHP_ARG_WITH(slang-dir, for the location of libslang,
[  --with-slang-dir[=DIR]      NEWT: Set the path to libslang install prefix.], no, no)

if test "$PHP_NEWT" != "no"; then

  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/newt.h"
  if test -r $PHP_NEWT/$SEARCH_FOR; then
     NEWT_DIR=$PHP_NEWT
  else
     AC_MSG_CHECKING([for newt files in default path])
     for i in $SEARCH_PATH ; do
       if test -r $i/$SEARCH_FOR; then
         NEWT_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
  fi
  
  if test -z "$NEWT_DIR"; then
     AC_MSG_RESULT([not found])
     AC_MSG_ERROR([Please reinstall the newt distribution])
  fi

  if test "$PHP_CURSES_DIR" != "no"; then
    for i in $PHP_CURSES_DIR /usr/local /usr; do
      if test -f $i/$PHP_LIBDIR/libncurses.$SHLIB_SUFFIX_NAME -o -f $i/$PHP_LIBDIR/libncurses.a; then
				PHP_ADD_LIBRARY_WITH_PATH(ncurses, $i/$PHP_LIBDIR, NEWT_SHARED_LIBADD)
				break
			fi
      if test -f $i/$PHP_LIBDIR/libcurses.$SHLIB_SUFFIX_NAME -o -f $i/$PHP_LIBDIR/libcurses.a; then
				PHP_ADD_LIBRARY_WITH_PATH(curses, $i/$PHP_LIBDIR, NEWT_SHARED_LIBADD)
				break
			fi
    done
	fi

  if test "$PHP_SLANG_DIR" != "no"; then
    for i in $PHP_SLANG_DIR /usr/local /usr; do
      if test -f $i/$PHP_LIBDIR/libslang.$SHLIB_SUFFIX_NAME -o -f $i/$PHP_LIBDIR/libslang.a; then
				PHP_ADD_LIBRARY_WITH_PATH(slang, $i/$PHP_LIBDIR, NEWT_SHARED_LIBADD)
				break
			fi
    done
	fi

  PHP_ADD_INCLUDE($NEWT_DIR/include)

  PHP_CHECK_LIBRARY(newt,newtInit,
  [
    PHP_ADD_INCLUDE($NEWT_DIR/include)
    PHP_ADD_LIBRARY_WITH_PATH(newt, $NEWT_DIR/$PHP_LIBDIR, NEWT_SHARED_LIBADD)
    AC_DEFINE(HAVE_NEWTLIB,1,[ ])
  ],[
    AC_MSG_ERROR([newt extension requires libnewt])
  ],[
    -L$NEWT_DIR/$PHP_LIBDIR
  ])

  PHP_SUBST(NEWT_SHARED_LIBADD)

  PHP_CHECK_LIBRARY(newt, newtListitem, [AC_DEFINE(HAVE_NEWT_LISTITEM,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtListitemSet, [AC_DEFINE(HAVE_NEWT_LISTITEM_SET,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtListitemGetData, [AC_DEFINE(HAVE_NEWT_LISTITEM_GET_DATA,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtResizeScreen, [AC_DEFINE(HAVE_NEWT_RESIZE_SCREEN,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtListboxItemCount, [AC_DEFINE(HAVE_NEWT_LISTBOX_ITEM_COUNT,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtSetHelpCallback, [AC_DEFINE(HAVE_NEWT_SET_HELP_CALLBACK,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtCursorOff, [AC_DEFINE(HAVE_NEWT_CURSOR_OFF,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])
  PHP_CHECK_LIBRARY(newt, newtCursorOn, [AC_DEFINE(HAVE_NEWT_CURSOR_ON,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtCheckboxTreeGetCurrent, [
  	AC_DEFINE(HAVE_NEWT_CHECKBOX_TREE_GET_CURRENT,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtCheckboxTreeSetCurrent, [
  	AC_DEFINE(HAVE_NEWT_CHECKBOX_TREE_SET_CURRENT,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtCheckboxTreeSetEntry, [
  	AC_DEFINE(HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtCheckboxTreeSetWidth, [
  	AC_DEFINE(HAVE_NEWT_CHECKBOX_TREE_SET_WIDTH,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtCheckboxTreeGetEntryValue, [
  	AC_DEFINE(HAVE_NEWT_CHECKBOX_TREE_GET_ENTRY_VALUE,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtCheckboxTreeSetEntryValue, [
  	AC_DEFINE(HAVE_NEWT_CHECKBOX_TREE_SET_ENTRY_VALUE,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  PHP_CHECK_LIBRARY(newt, newtFormSetTimer, [AC_DEFINE(HAVE_NEWT_FORM_SET_TIMER,1,[ ])],, [-L$NEWT_DIR/$PHP_LIBDIR])

  AC_MSG_CHECKING([if newtExistStruct has u.watch])

  old_CFLAGS=$CFLAGS
  CLFAGS=-I$NEWT_DIR/include
  AC_TRY_COMPILE([
#include <newt.h>
  ], [static struct newtExitStruct es;  es.u.watch = 0; return 0],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])
     AC_DEFINE(MISSING_NEWTEXITSTRUCT_U_WATCH, 1, [ ])]
  )
  CFLAGS=$old_CFLAGS

  PHP_NEW_EXTENSION(newt, newt.c newt_vcall.c, $ext_shared, cli)
fi
