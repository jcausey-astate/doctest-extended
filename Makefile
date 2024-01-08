# path macros
DOCTEST_PATH=doctest
SRC_PATH=src
UPSTREAM_PATH=upstream

project_name := doctest-extended
target_name  := doctest-extended.h

doctest_name := doctest.h
extensions_name := doctest-grading-extensions.h
preamble_name := doctest-extended-preamble.h

sep_start := "// *****"
sep_end   := "******************************************************************************"

# default rule
default: all

# phony rules
.PHONY: all
all: update src/doctest-grading-extensions.h upstream/doctest.h
	@cat $(SRC_PATH)/$(preamble_name) > $(target_name)
	@echo "" >> $(target_name)
	@echo $(sep_start) $(extensions_name) $(sep_end) | head -c 80 >> $(target_name)
	@echo "" >> $(target_name) ; echo "" >> $(target_name)
	@cat $(SRC_PATH)/$(extensions_name) >> $(target_name)
	@echo "" >> $(target_name)
	@echo $(sep_start) $(doctest_name) $(sep_end) | head -c 80 >> $(target_name)
	@echo "" >> $(target_name) ; echo "" >> $(target_name)
	@cat $(UPSTREAM_PATH)/$(doctest_name) >> $(target_name)

.PHONY: update
update:
	@./update-doctest.sh >/dev/null || { echo "Update step failed." ; false; }
