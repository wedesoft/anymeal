SUFFIXES = .ui .cpp .hpp .cc .hh

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = anymeal

noinst_HEADERS = main_window.hpp partition.hpp

BUILT_SOURCES = ui_main_window.hpp moc_main_window.cpp gtest-all.cc gmock-all.cc

anymeal_SOURCES = anymeal.cpp main_window.cpp moc_main_window.cpp partition.cpp
anymeal_CXXFLAGS = $(QT_CXXFLAGS)
anymeal_LDFLAGS =
anymeal_LDADD = $(QT_LIBS)

check_PROGRAMS = suite

suite_SOURCES = suite.cpp gtest-all.cc gmock-all.cc test_partition.cpp partition.cpp
suite_CXXFLAGS = -I$(GMOCK)/include -I$(GTEST)/include -I$(GTEST) -I$(GMOCK)
suite_LDFLAGS =
suite_LDADD = -lpthread

gtest-all.cc: $(GTEST)/src/gtest-all.cc
	$(LN_S) $< $@

gmock-all.cc: $(GMOCK)/src/gmock-all.cc
	$(LN_S) $< $@

check-local:
	$(top_srcdir)/suite

ui_%.hpp: %.ui
	$(QT_UIC) -o $@ $<

moc_%.cpp: %.hpp
	$(QT_MOC) -o $@ $<
