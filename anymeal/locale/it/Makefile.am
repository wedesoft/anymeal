SUFFIXES = .ts .qm

itdir = $(localedir)/it/LC_MESSAGES
it_DATA = anymeal_qt.qm

EXTRA_DIST = anymeal_qt.ts

%.ts: $(wildcard ../../*.cc) $(wildcard ../../*.ui)
	@QT_LUPDATE@ $^ -ts $@

.ts.qm:
	@QT_LCONVERT@ -o $@ $<
