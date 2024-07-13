AC_PREREQ([2.69])
AC_INIT([anymeal], [1.28], [jan@wedesoft.de])
AM_INIT_AUTOMAKE([--warnings=no-portability foreign])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG([C++])
AC_PROG_LN_S
PKG_PROG_PKG_CONFIG

dnl Check for flex.
AC_PROG_LEX(noyywrap)
if test "x$LEX" != xflex; then
  AC_MSG_ERROR([Could not find flex])
fi

dnl Check for windres.
AC_CHECK_PROG(WINDRES,windres,windres,no)
AM_CONDITIONAL([HAVE_WINDRES], [test "x$WINDRES" = "xwindres"])

dnl Check for Googletest source file.
AC_ARG_WITH(gtest,
            AC_HELP_STRING([--with-gtest=<dir>],[directory of Google Test framework]),
            [GTESTSRC="$withval"],
            [GTESTSRC="/usr/src/googletest/googletest"])
AC_MSG_CHECKING([Google Test source code])
if test -e "$GTESTSRC/src/gtest-all.cc"; then
  AC_MSG_RESULT(found)
else
  AC_MSG_RESULT(not found)
fi
AC_SUBST(GTESTSRC)

dnl Check for Googletest library.
PKG_CHECK_MODULES(GTEST, gtest >= 1.12.0,
                  [GTESTLIB='yes'],
                  [GTESTLIB='no'])
AC_SUBST(GTEST_CFLAGS)
AC_SUBST(GTEST_LIBS)

dnl Enable unit tests if Googletest source or library was found.
AM_CONDITIONAL([ENABLE_TESTS], [test -e "$GTESTSRC/src/gtest-all.cc" -o "x$GTESTLIB" == xyes])
AM_CONDITIONAL([GOOGLE_TEST_SRC], [test -e "$GTESTSRC/src/gtest-all.cc"])

dnl Check for iconv-library.
AC_MSG_CHECKING([for iconv-library])
AC_TRY_LINK([#include <stdbool.h>
             #include <stdlib.h>
             #include <stdio.h>
             #include <iconv.h>],
            [iconv_open("US-ASCII", "UTF-8")],
            [AC_MSG_RESULT([found])],
            [AC_MSG_ERROR([Check for iconv-library failed.])]);

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
QT_RCC=`(which rcc-qt5 || which rcc)`
if test "x$QT_RCC" = "x"; then
  AC_MSG_ERROR([Could not locate Qt5 resource compiler])
fi
AC_SUBST(QT_RCC)

dnl Check for Qt5 lconvert.
QT_LCONVERT=`(which lconvert-qt5 || which lconvert)`
if test "x$QT_LCONVERT" = "x"; then
        AC_MSG_ERROR([Could not find Lconvert from qt5])
fi
AC_SUBST(QT_LCONVERT)

dnl Check for lupdate.
QT_LUPDATE=`(which lupdate-qt5 || which lupdate)`
if test "x$QT_LUPDATE" = "x"; then
        AC_MSG_ERROR([Could not find lupdate from qt5])
fi
AC_SUBST(QT_LUPDATE)

AC_CONFIG_HEADERS(config.h)
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
