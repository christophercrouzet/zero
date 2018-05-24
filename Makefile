PROJECT_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# ------------------------------------------------------------------------------

ifndef outdir
OUT_DIR := build
else
OUT_DIR := $(outdir)
endif

ifndef config
CONFIG := release
else ifneq "$(filter-out debug release all,$(config))" ""
$(error the 'config' option is not valid)
else ifneq "$(filter all,$(config))" ""
CONFIG := debug release
else
CONFIG := $(config)
endif

# ------------------------------------------------------------------------------

PROJECT := zero

# ------------------------------------------------------------------------------

FORMAT_FILES :=
TIDY_FILES :=

# ------------------------------------------------------------------------------

# $(1): build directory.
# $(2): rule.
define zr_forward_rule_impl =
$(MAKE) -C $(1) -s $(2)
endef

# Forward a rule to the generated Makefiles.
# $(1): rule.
define zr_forward_rule =
$(foreach _x,$(BUILD_DIRS), $(call \
	zr_forward_rule_impl,$(_x),$(1)))
endef

# ------------------------------------------------------------------------------

# Create a Makefile rule.
# # $(1): configuration.
define zr_create_makefile =
$$(OUT_DIR)/$(1)/Makefile:
	@ mkdir -p $$(OUT_DIR)/$(1)
	@ cd $$(OUT_DIR)/$(1) && cmake \
		-DCMAKE_BUILD_TYPE=$(1) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		$(PROJECT_DIR)

BUILD_DIRS += $$(OUT_DIR)/$(1)
MAKE_FILES += $$(OUT_DIR)/$(1)/Makefile
endef

$(foreach _config,$(CONFIG),$(eval $(call \
	zr_create_makefile,$(_config))))

# ------------------------------------------------------------------------------

tool-build: $(MAKE_FILES)
	@ $(call zr_forward_rule,tool-build)

tools: $(MAKE_FILES)
	@ $(call zr_forward_rule,tools)

.PHONY: tool-build tools

TOOLS := $(notdir $(wildcard tools/*))
FORMAT_FILES += $(foreach _x,$(TOOLS),$(wildcard tools/$(_x)/*.[ch]))
TIDY_FILES += $(foreach _x,$(TOOLS),$(wildcard tools/$(_x)/*.[ch]))

# ------------------------------------------------------------------------------

TEMPLATES := $(wildcard src/*.tpl)
INCLUDES := $(TEMPLATES:src/%.h.tpl=include/$(PROJECT)/%.h)

$(INCLUDES): include/$(PROJECT)/%.h: src/%.h.tpl
	@ mkdir -p $(@D)
	@ $(firstword $(BUILD_DIRS))/bin/tools/build $< $@
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

tidy: $(firstword $(MAKE_FILES))
	@ clang-tidy $(TIDY_FILES) \
		-p $(firstword $(BUILD_DIRS))/compile_commands.json \
		-- -I$(CLANG_INCLUDE_DIR)

.PHONY: format tidy

# ------------------------------------------------------------------------------

install: $(MAKE_FILES)
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
