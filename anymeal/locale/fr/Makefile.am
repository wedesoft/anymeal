SUFFIXES = .ts .qm

frdir = $(localedir)/fr/LC_MESSAGES
fr_DATA = anymeal_qt.qm

EXTRA_DIST = anymeal_qt.ts

%.ts: $(wildcard ../../*.cc) $(wildcard ../../*.ui)
	lupdate $^ -ts $@

.ts.qm:
	lconvert -o $@ $<
