SUFFIXES = .ts .qm

sldir = $(localedir)/sl/LC_MESSAGES
sl_DATA = anymeal_qt.qm

EXTRA_DIST = anymeal_qt.ts

CLEANFILES = anymeal_qt.qm

%.ts: $(wildcard ../../*.cc) $(wildcard ../../*.ui)
	$(QT_LUPDATE) $^ -ts $@

.ts.qm:
	$(QT_LCONVERT) -o $@ $<
