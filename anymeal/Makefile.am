SUFFIXES = .ui .qrc .cc .hh .ll .rc .o

SUBDIRS = locale

bin_PROGRAMS = anymeal

noinst_LIBRARIES = libanymeal.a

noinst_HEADERS = main_window.hh partition.hh mealmaster.hh recipe.hh ingredient.hh recode.hh database.hh titles_model.hh \
								 categories_model.hh html.hh export.hh import_dialog.hh export_dialog.hh edit_dialog.hh ingredient_model.hh \
								 instructions_model.hh category_dialog.hh

EXTRA_DIST = main_window.ui import_dialog.ui export_dialog.ui edit_dialog.ui category_dialog.ui anymeal.qrc anymeal.png anymeal.ico \
						 anymeal.desktop anymeal.nsi anymeal.man categoryadd.svg categoryremove.svg delete.svg down.svg edit.svg export.svg \
						 import.svg preview.svg print.svg quit.svg search.svg trash.svg up.svg splash.png

BUILT_SOURCES = ui_main_window.hh ui_import_dialog.hh ui_export_dialog.hh ui_edit_dialog.hh ui_category_dialog.hh \
								moc_main_window.cc moc_import_dialog.cc moc_export_dialog.cc moc_edit_dialog.cc moc_ingredient_model.cc \
								moc_titles_model.cc moc_categories_model.cc moc_instructions_model.cc moc_category_dialog.cc qrc_anymeal.cc

anymeal_SOURCES = anymeal.cc main_window.cc import_dialog.cc export_dialog.cc edit_dialog.cc moc_main_window.cc ingredient_model.cc \
									titles_model.cc categories_model.cc instructions_model.cc category_dialog.cc moc_import_dialog.cc \
									moc_export_dialog.cc moc_edit_dialog.cc moc_ingredient_model.cc moc_titles_model.cc moc_categories_model.cc \
									moc_instructions_model.cc moc_category_dialog.cc qrc_anymeal.cc
if HAVE_WINDRES
anymeal_SOURCES += icon.rc
endif
anymeal_CXXFLAGS = $(SQLITE3_CFLAGS) $(QT_CXXFLAGS)
anymeal_LDFLAGS =
anymeal_LDADD = libanymeal.a -lrecode $(SQLITE3_LDFLAGS) $(QT_LIBS)

libanymeal_a_SOURCES = partition.cc recipe.cc ingredient.cc mealmaster.ll recode.cc database.cc html.cc export.cc
libanymeal_a_CXXFLAGS =
libanymeal_a_LIBADD =

CLEANFILES = $(BUILT_SOURCES)

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
