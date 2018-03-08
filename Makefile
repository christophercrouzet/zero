PROJECT_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# ------------------------------------------------------------------------------

ifndef outdir
OUT_DIR := build
else
OUT_DIR := $(outdir)
endif

# ------------------------------------------------------------------------------

PROJECT := zero

# ------------------------------------------------------------------------------

FORMAT_FILES :=
TIDY_FILES :=

# ------------------------------------------------------------------------------

# Forward a rule to the generated Makefile.
# $(1): rule.
define zr_forward_rule =
$(MAKE) -C $(OUT_DIR) -s $(1)
endef

# ------------------------------------------------------------------------------

$(OUT_DIR)/Makefile:
	@ mkdir -p $(OUT_DIR)
	@ cd $(OUT_DIR) && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		$(PROJECT_DIR)

# ------------------------------------------------------------------------------

tool-build: $(OUT_DIR)/Makefile
	@ $(call zr_forward_rule,tool-build)

tools: $(OUT_DIR)/Makefile
	@ $(call zr_forward_rule,tools)

.PHONY: tool-build tools

TOOLS := $(notdir $(wildcard tools/*))
FORMAT_FILES += $(foreach _x,tools/$(TOOLS),$(wildcard $(_x)/*.[ch]))
TIDY_FILES += $(foreach _x,tools/$(TOOLS),$(wildcard $(_x)/*.[ch]))

# ------------------------------------------------------------------------------

TEMPLATES := $(wildcard src/*.tpl)
INCLUDES := $(TEMPLATES:src/%.h.tpl=include/$(PROJECT)/%.h)

$(INCLUDES): include/$(PROJECT)/%.h: src/%.h.tpl
	@ mkdir -p $(@D)
	@ $(OUT_DIR)/bin/tools/build $< $@
	@ clang-format -i -style=file $@

$(INCLUDES): tool-build

includes: $(INCLUDES)

.PHONY: includes

FORMAT_FILES += $(TEMPLATES) $(wildcard src/partials/*.h)
TIDY_FILES += $(INCLUDES)

# ------------------------------------------------------------------------------

CLANG_VERSION := $(shell \
	clang --version \
	| grep version \
	| sed 's/^.*version \([0-9]*\.[0-9]*\.[0-9]*\).*$$/\1/')
CLANG_DIR := $(shell dirname $(shell which clang))
CLANG_INCLUDE_DIR := $(CLANG_DIR)/../lib/clang/$(CLANG_VERSION)/include

format:
	@ clang-format -i -style=file $(FORMAT_FILES)

tidy: $(OUT_DIR)/Makefile
	@ clang-tidy $(TIDY_FILES) \
		-p $(OUT_DIR)/compile_commands.json \
		-- -I$(CLANG_INCLUDE_DIR)

.PHONY: format tidy

# ------------------------------------------------------------------------------

install: $(OUT_DIR)/Makefile
	@ $(call zr_forward_rule,install)

.PHONY: install

# ------------------------------------------------------------------------------

clean:
	@ rm -rf $(OUT_DIR)

.PHONY: clean

# ------------------------------------------------------------------------------

all: includes

.PHONY: all

.DEFAULT_GOAL := all
