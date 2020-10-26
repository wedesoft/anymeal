SUFFIXES = .ts .qm

nldir = $(localedir)/nl/LC_MESSAGES
nl_DATA = anymeal_qt.qm

EXTRA_DIST = anymeal_qt.ts

%.ts: $(wildcard ../../*.cc) $(wildcard ../../*.ui)
	lupdate $^ -ts $@

.ts.qm:
	lconvert -o $@ $<
