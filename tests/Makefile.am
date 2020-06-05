SUFFIXES = .cc .hh

SUBDIRS = fixtures

BUILT_SOURCES = gtest-all.cc gmock-all.cc

check_PROGRAMS = suite

suite_SOURCES = suite.cc gtest-all.cc gmock-all.cc test_partition.cc test_recipe.cc test_ingredient.cc test_mealmaster.cc \
								test_recode.cc test_database.cc test_html.cc
suite_CXXFLAGS = -I$(top_srcdir)/anymeal $(SQLITE3_CFLAGS) -I$(GMOCK)/include -I$(GTEST)/include -I$(GTEST) -I$(GMOCK)
suite_LDFLAGS =
suite_LDADD = ../anymeal/libanymeal.a -lrecode $(SQLITE3_LDFLAGS) -lpthread

gtest-all.cc: $(GTEST)/src/gtest-all.cc
	rm -f $@
	$(LN_S) $< $@

gmock-all.cc: $(GMOCK)/src/gmock-all.cc
	rm -f $@
	$(LN_S) $< $@

check-local:
	$(top_srcdir)/tests/suite
