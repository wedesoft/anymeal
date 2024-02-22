SUFFIXES = .cc .hh

SUBDIRS = fixtures

if ENABLE_TESTS
if GOOGLE_TEST_SRC
BUILT_SOURCES = gtest-all.cc
endif
check_PROGRAMS = suite
suite_LDFLAGS =
if GOOGLE_TEST_SRC
suite_SOURCES = suite.cc gtest-all.cc test_partition.cc test_recipe.cc test_ingredient.cc test_mealmaster.cc \
								test_recode.cc test_database.cc test_html.cc test_export.cc
suite_CXXFLAGS = -I$(top_srcdir)/anymeal $(SQLITE3_CFLAGS) -I$(GTESTSRC)/include -I$(GTESTSRC)
suite_LDADD = ../anymeal/libanymeal.a $(SQLITE3_LDFLAGS) -lpthread
else
suite_SOURCES = suite.cc test_partition.cc test_recipe.cc test_ingredient.cc test_mealmaster.cc \
								test_recode.cc test_database.cc test_html.cc test_export.cc
suite_CXXFLAGS = -I$(top_srcdir)/anymeal $(SQLITE3_CFLAGS) $(GTEST_CFLAGS)
suite_LDADD = ../anymeal/libanymeal.a $(GTEST_LIBS) $(SQLITE3_LDFLAGS) -lpthread
endif
else
BUILT_SOURCES =
check_PROGRAMS =
endif

CLEANFILES = $(BUILT_SOURCES)

if GOOGLE_TEST_SRC
gtest-all.cc: $(GTESTSRC)/src/gtest-all.cc
	rm -f $@
	$(LN_S) $< $@
endif

if ENABLE_TESTS
check-local:
	./suite
endif
