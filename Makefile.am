SUFFIXES = .ui .cc .hh .cc .hh .ll

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = anymeal

noinst_HEADERS = main_window.hh partition.hh mealmaster.hh recipe.hh

EXTRA_DIST = main_window.ui test.mmf test2.mmf

BUILT_SOURCES = ui_main_window.hh moc_main_window.cc gtest-all.cc gmock-all.cc

anymeal_SOURCES = anymeal.cc main_window.cc moc_main_window.cc partition.cc recipe.cc mealmaster.ll
anymeal_CXXFLAGS = $(QT_CXXFLAGS)
anymeal_LDFLAGS =
anymeal_LDADD = $(QT_LIBS)

check_PROGRAMS = suite

suite_SOURCES = suite.cc gtest-all.cc gmock-all.cc test_partition.cc partition.cc test_recipe.cc recipe.cc \
								test_mealmaster.cc mealmaster.ll
suite_CXXFLAGS = -I$(GMOCK)/include -I$(GTEST)/include -I$(GTEST) -I$(GMOCK)
suite_LDFLAGS =
suite_LDADD = -lpthread

gtest-all.cc: $(GTEST)/src/gtest-all.cc
	$(LN_S) $< $@

gmock-all.cc: $(GMOCK)/src/gmock-all.cc
	$(LN_S) $< $@

check-local:
	$(top_srcdir)/suite

ui_%.hh: %.ui
	$(QT_UIC) -o $@ $<

moc_%.cc: %.hh
	$(QT_MOC) -o $@ $<
