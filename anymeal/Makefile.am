SUFFIXES = .ui .qrc .cc .hh .ll .rc .o .ts .qm

bin_PROGRAMS = anymeal

noinst_LIBRARIES = libanymeal.a

noinst_HEADERS = main_window.hh partition.hh mealmaster.hh recipe.hh ingredient.hh recode.hh database.hh titles_model.hh \
								 categories_model.hh html.hh import_dialog.hh

EXTRA_DIST = main_window.ui import_dialog.ui anymeal.qrc anymeal.png anymeal.ico anymeal.desktop anymeal.nsi anymeal.man

BUILT_SOURCES = ui_main_window.hh ui_import_dialog.hh moc_main_window.cc moc_import_dialog.cc qrc_anymeal.cc

anymeal_SOURCES = anymeal.cc main_window.cc import_dialog.cc moc_main_window.cc moc_import_dialog.cc qrc_anymeal.cc \
									titles_model.cc categories_model.cc
if HAVE_WINDRES
anymeal_SOURCES += icon.rc
endif
anymeal_CXXFLAGS = $(SQLITE3_CFLAGS) $(QT_CXXFLAGS)
anymeal_LDFLAGS =
anymeal_LDADD = libanymeal.a -lrecode $(SQLITE3_LDFLAGS) $(QT_LIBS)

libanymeal_a_SOURCES = partition.cc recipe.cc ingredient.cc mealmaster.ll recode.cc database.cc html.cc
libanymeal_a_CXXFLAGS =
libanymeal_a_LIBADD =

appdir = $(datadir)/applications
app_DATA = anymeal.desktop

icondir = $(datadir)/icons/hicolor/64x64/apps
icon_DATA = anymeal.png

man1_MANS = anymeal.man

ui_%.hh: %.ui
	$(QT_UIC) -o $@ $<

moc_%.cc: %.hh
	$(QT_MOC) -o $@ $<

qrc_%.cc: %.qrc
	$(QT_RCC) -o $@ $<

.rc.o:
	$(WINDRES) $< -O coff -o $@

%.ts: $(wildcard *.cc) $(wildcard *.ui)
	lupdate $^ -ts $@

.ts.qm:
	lconvert -o $@ $<
