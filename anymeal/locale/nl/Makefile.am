SUFFIXES = .ts .qm

dedir = $(localedir)/de/LC_MESSAGES
de_DATA = anymeal_qt.qm

EXTRA_DIST = anymeal_qt.ts

%.ts: $(wildcard ../../*.cc) $(wildcard ../../*.ui)
	lupdate $^ -ts $@

.ts.qm:
	lconvert -o $@ $<
