ifndef outdir
OUTDIR := build
else
OUTDIR := $(outdir)
endif

# ------------------------------------------------------------------------------

PROJECT := zero

OBJECTDIR := $(OUTDIR)/obj
BINARYDIR := $(OUTDIR)/bin

CFLAGS := -std=c99
CPPFLAGS := -DNDEBUG -O3 \
            -Wpedantic -Wall -Wextra -Waggregate-return -Wcast-align \
            -Wcast-qual -Wconversion -Wfloat-equal -Wpointer-arith -Wshadow \
            -Wstrict-overflow=5 -Wswitch -Wswitch-default -Wundef \
            -Wunreachable-code -Wwrite-strings
PREREQFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS := 

COMPILE.c = $(CC) $(PREREQFLAGS) $(CPPFLAGS) $(CFLAGS) $(TARGET_ARCH) -c
LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

# ------------------------------------------------------------------------------

ALL_SOURCES :=
ALL_HEADERS :=

# Create build rules for object targets.
# $(1): source path, e.g.: src, tools/build.
# $(2): target path, e.g.: zero, tools/build.
# $(3): prefix for variable names.
define CREATE_OBJECT_RULES =
$(3)_SOURCES := $$(wildcard $(1)/*.c)
$(3)_HEADERS := $$(wildcard $(1)/*.h)
$(3)_OBJECTS := $$($(3)_SOURCES:$(1)/%.c=$$(OBJECTDIR)/$(2)/%.o)
$(3)_PREREQS := $$($(3)_OBJECTS:.o=.d)

$$($(3)_OBJECTS): $$(OBJECTDIR)/$(2)/%.o: $(1)/%.c
	@ mkdir -p $$(@D)
	@ $$(COMPILE.c) -o $$@ $$<

-include $$($(3)_PREREQS)

ALL_SOURCES += $$($(3)_SOURCES)
ALL_HEADERS += $$($(3)_HEADERS)
endef

# Create build rules for binary targets.
# $(1): source path, e.g.: src, tools/build.
# $(2): target path, e.g.: zero, tools/build.
# $(3): prefix for variable names.
define CREATE_BINARY_RULES =
$(3)_SOURCES := $$(wildcard $(1)/*.c)
$(3)_OBJECTS := $$($(3)_SOURCES:$(1)/%.c=$$(OBJECTDIR)/$(2)/%.o)
$(3)_TARGET := $$(BINARYDIR)/$(2)

$$(eval $$(call \
    CREATE_OBJECT_RULES,$(1),$(2),$(3)))

$$($(3)_TARGET): $$($(3)_OBJECTS)
	@ mkdir -p $$(@D)
	@ $$(LINK.o) $$^ $$(LDLIBS) -o $$@
endef

# ------------------------------------------------------------------------------

TOOLS_TARGETS :=
TOOLS_PHONYTARGETS :=

# Create the rules to build a tool target.
# $(1): target name.
# $(2): path.
# $(3): prefix for variable names.
define CREATE_TOOL_RULES =
$$(eval $$(call \
    CREATE_BINARY_RULES,$(2),$(2),$(3)_$(1)))

$(3)-$(1): $$($(3)_$(1)_TARGET)

.PHONY: $(3)-$(1)

TOOLS_TARGETS += $$($(3)_$(1)_TARGET)
TOOLS_PHONYTARGETS += $(3)-$(1)
endef

# Create the rules for all the tool targets.
# $(1): path.
# $(2): prefix for variable names.
define CREATE_TOOLS_RULES =
TOOLS := $$(notdir $$(wildcard $(1)/*))
$$(foreach _i,$$(TOOLS),$$(eval $$(call \
    CREATE_TOOL_RULES,$$(_i),$(1)/$$(_i),$(2))))

tools: $$(TOOLS_PHONYTARGETS)

.PHONY: tools
endef

$(eval $(call \
    CREATE_TOOLS_RULES,tools,tool))

# ------------------------------------------------------------------------------

TEMPLATES := $(wildcard src/*.tpl)
INCLUDES := $(TEMPLATES:src/%.h.tpl=include/$(PROJECT)/%.h)

$(INCLUDES): include/$(PROJECT)/%.h: src/%.h.tpl
	@ mkdir -p $(@D)
	@ $(tool_build_TARGET) $< $@
	@ clang-format -i -style=file $@

$(INCLUDES): tool-build

includes: $(INCLUDES)

.PHONY: includes

# ------------------------------------------------------------------------------

ALL_HEADERS += $(TEMPLATES) $(wildcard src/partials/*.h)

CLANGVERSION := $(shell clang --version \
                        | grep version \
                        | sed 's/^.*version \([0-9]*\.[0-9]*\.[0-9]*\).*$$/\1/')
CLANGDIR := $(shell dirname $(shell which clang))
CLANGINCLUDE := $(CLANGDIR)/../lib/clang/$(CLANGVERSION)/include

format:
	@ clang-format -i -style=file $(ALL_SOURCES) $(ALL_HEADERS)

tidy:
	clang-tidy -fix $(INCLUDES) -- $(CPPFLAGS) $(CFLAGS) -I$(CLANGINCLUDE)

.PHONY: format tidy

# ------------------------------------------------------------------------------

clean:
	@- rm -rf $(OUTDIR)

.PHONY: clean

# ------------------------------------------------------------------------------

all: includes

.PHONY: all

.DEFAULT_GOAL := all
