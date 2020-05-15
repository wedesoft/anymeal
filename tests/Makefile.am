SUFFIXES = .cc .hh

EXTRA_DIST = test_header.mmf test_header2.mmf test_ingredient.mmf test_preparation.mmf test_fraction.mmf \
						 test_float.mmf test_mixed.mmf test_noamount.mmf test_continuation.mmf test_two_ingredients.mmf test_instructions1.mmf \
						 test_instructions2.mmf test_instructions3.mmf test_instructions4.mmf test_instructions5.mmf test_instructions6.mmf \
						 test_instructions7.mmf test_instructions8.mmf test_instructions9.mmf test_instructions10.mmf test_two_instructions.mmf \
						 test_no_servings.mmf test_ingredient_section.mmf test_section_hyphen.mmf test_section_spaces.mmf

BUILT_SOURCES = gtest-all.cc gmock-all.cc

check_PROGRAMS = suite

suite_SOURCES = suite.cc gtest-all.cc gmock-all.cc test_partition.cc test_recipe.cc test_ingredient.cc test_mealmaster.cc
suite_CXXFLAGS = -I$(top_srcdir)/anymeal -I$(GMOCK)/include -I$(GTEST)/include -I$(GTEST) -I$(GMOCK)
suite_LDFLAGS =
suite_LDADD = ../anymeal/libanymeal.a -lpthread

gtest-all.cc: $(GTEST)/src/gtest-all.cc
	$(LN_S) $< $@

gmock-all.cc: $(GMOCK)/src/gmock-all.cc
	$(LN_S) $< $@

check-local:
	$(top_srcdir)/tests/suite
