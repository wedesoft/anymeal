AC_PREREQ([2.69])
AC_INIT([anymeal], [1.19], [jan@wedesoft.de])
AM_INIT_AUTOMAKE([--warnings=no-portability foreign])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_LANG([C++])
AC_PROG_LN_S

dnl Check for flex.
AC_PROG_LEX
if test "x$LEX" != xflex; then
  AC_MSG_ERROR([Could not find flex])
fi

dnl Check for windres.
AC_CHECK_PROG(WINDRES,windres,windres,no)
AM_CONDITIONAL([HAVE_WINDRES], [test "x$WINDRES" = "xwindres"])

dnl Check for Googletest framework.
if test "x$GTEST" = "x"; then
  GTEST="/usr/src/googletest/googletest"
fi
AS_IF([test -e "$GTEST/src/gtest-all.cc"],
      AC_SUBST(GTEST),
      AC_MSG_ERROR([Could not find $GTEST/src/gtest-all.cc]))

dnl Check for recode-library.
AC_MSG_CHECKING([for recode-library])
old_LIBS="$LIBS"
LIBS="$LIBS -lrecode"
AC_TRY_LINK([#include <stdbool.h>
             #include <stdlib.h>
             #include <stdio.h>
             #include <recode.h>],
            [recode_new_outer( false )],
            [AC_MSG_RESULT([found])],
            [AC_MSG_ERROR([Check for recode-library failed.])]);
LIBS="$old_LIBS"

dnl Check for SQLite 3 library.
AX_LIB_SQLITE3([3.6.19])
if test "x$SQLITE3_VERSION" = "x"; then
  AC_MSG_ERROR([Could not find SQLite 3 library])
fi

dnl Check for Qt5 library.
AX_HAVE_QT
if test "x$have_qt" = "xno"; then
  AC_MSG_ERROR([Could not locate Qt5 installation])
fi

dnl Check for Qt5 resource compiler.
QT_RCC=`(which rcc || which rcc-qt5)`
if test "x$QT_RCC" = "x"; then
  AC_MSG_ERROR([Could not locate Qt5 resource compiler])
fi
AC_SUBST(QT_RCC)

dnl Check for Qt5 lconvert.
QT_LCONVERT=`(which lconvert || which lconvert-qt5)`
if test "x$QT_LCONVERT" = "x"; then
        AC_MSG_ERROR([Could not find Lconvert from qt5])
fi
AC_SUBST(QT_LCONVERT)

dnl Check for lupdate.
QT_LUPDATE=`(which lupdate || which lupdate-qt5)`
if test "x$QT_LUPDATE" = "x"; then
        AC_MSG_ERROR([Could not find lupdate from qt5])
fi
AC_SUBST(QT_LUPDATE)

AM_CONFIG_HEADER(config.h)
AC_CHECK_HEADERS([cassert ios sstream fstream string])
AC_OUTPUT(Makefile
          anymeal/Makefile
          anymeal/locale/Makefile
          anymeal/locale/de/Makefile
          anymeal/locale/it/Makefile
          anymeal/locale/nl/Makefile
          anymeal/locale/fr/Makefile
          tests/Makefile
          tests/fixtures/Makefile)
