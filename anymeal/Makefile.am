SUFFIXES = .ui .qrc .cc .hh .ll

bin_PROGRAMS = anymeal

noinst_LIBRARIES = libanymeal.a

noinst_PROGRAMS = parser

noinst_HEADERS = main_window.hh partition.hh mealmaster.hh recipe.hh ingredient.hh recode.hh database.hh titles_model.hh \
								 categories_model.hh html.hh import_dialog.hh

EXTRA_DIST = main_window.ui import_dialog.ui anymeal.qrc anymeal.png

BUILT_SOURCES = ui_main_window.hh ui_import_dialog.hh moc_main_window.cc moc_import_dialog.cc qrc_anymeal.cc

anymeal_SOURCES = anymeal.cc main_window.cc import_dialog.cc moc_main_window.cc moc_import_dialog.cc qrc_anymeal.cc \
									titles_model.cc categories_model.cc
anymeal_CXXFLAGS = $(SQLITE3_CFLAGS) $(QT_CXXFLAGS)
anymeal_LDFLAGS =
anymeal_LDADD = libanymeal.a -lrecode $(SQLITE3_LDFLAGS) $(QT_LIBS)

parser_SOURCES = parser.cc
parser_CXXFLAGS = $(SQLITE3_CFLAGS)
parser_LDFLAGS =
parser_LDADD = libanymeal.a -lrecode $(SQLITE3_LDFLAGS)

libanymeal_a_SOURCES = partition.cc recipe.cc ingredient.cc mealmaster.ll recode.cc database.cc html.cc
libanymeal_a_CXXFLAGS =
libanymeal_a_LIBADD =

ui_%.hh: %.ui
	$(QT_UIC) -o $@ $<

moc_%.cc: %.hh
	$(QT_MOC) -o $@ $<

qrc_%.cc: %.qrc
	$(QT_RCC) -o $@ $<
