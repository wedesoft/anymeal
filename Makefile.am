SUFFIXES = .ui .cpp .hpp

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = anymeal

noinst_HEADERS = main_window.hpp

BUILT_SOURCES = ui_main_window.hpp moc_main_window.cpp

anymeal_SOURCES = anymeal.cpp main_window.cpp moc_main_window.cpp
anymeal_CXXFLAGS = $(QT_CXXFLAGS)
anymeal_LDFLAGS =
anymeal_LDADD = $(QT_LIBS)

ui_%.hpp: %.ui
	$(QT_UIC) -o $@ $<

moc_%.cpp: %.hpp
	$(QT_MOC) -o $@ $<
