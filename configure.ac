AC_PREREQ([2.69])
AC_INIT([anymeal], [1.0.0], [jan@wedesoft.de])
AM_INIT_AUTOMAKE([--warnings=no-portability foreign])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG([C++])
AC_PROG_LN_S
AM_PROG_LIBTOOL
AC_PROG_LEX
if test "x$GTEST" == "x"; then
  GTEST="/usr/src/googletest/googletest"
fi
AC_CHECK_FILE("$GTEST/src/gtest-all.cc",
              AC_SUBST(GTEST),
              AC_MSG_ERROR([Could not find $GTEST/src/gtest-all.cc]))
if test "x$GMOCK" == "x"; then
  GMOCK="/usr/src/googletest/googlemock"
fi
AC_CHECK_FILE("$GMOCK/src/gmock-all.cc",
              AC_SUBST(GMOCK),
              AC_MSG_ERROR([Could not find $GMOCK/src/gmock-all.cc]))
AX_HAVE_QT
if test "x$have_qt" == "xno"; then
  AC_MSG_ERROR([Could not locate Qt5 installation])
fi
AM_CONFIG_HEADER(config.h)
AC_CHECK_HEADERS([ctype.h stdbool.h stdlib.h string.h stdio.h])
AC_OUTPUT(Makefile
          anymeal/Makefile
          tests/Makefile
          tests/fixtures/Makefile)
