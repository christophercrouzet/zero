ifndef outdir
OUTDIR := build
else
OUTDIR := $(outdir)
endif

ifndef compiler
COMPILER := c
else ifneq "$(filter-out c cc,$(compiler))" ""
$(error the 'compiler' option is not valid)
else
COMPILER := $(compiler)
endif

ifndef env
ENV := 64
else ifneq (1,$(words [$(strip $(env))]))
$(error the 'env' option should contain a single value)
else ifneq "$(filter-out 16 32 x32 64,$(env))" ""
$(error the 'env' option is not valid)
else
ENV := $(env)
endif

DEFAULT_ARCH := $(subst _,-,$(shell arch))
ifndef arch
ARCH := $(DEFAULT_ARCH)
else
ARCH := $(strip $(arch))
endif

ifndef config
CONFIG := release
else ifneq "$(filter-out debug release all,$(config))" ""
$(error the 'config' option is not valid)
else ifeq "$(filter all,$(config))" "all"
CONFIG := debug release
else
CONFIG := $(config)
endif

# ------------------------------------------------------------------------------

PROJECT := zero

OBJECTDIR := $(OUTDIR)/obj
BINARYDIR := $(OUTDIR)/bin

CFLAGS := -std=c99
CXXFLAGS := -std=c++11
CPPFLAGS := -Iinclude -fPIC \
            -Wpedantic -Wall -Wextra -Waggregate-return -Wcast-align \
            -Wcast-qual -Wconversion -Wfloat-equal -Wpointer-arith -Wshadow \
            -Wstrict-overflow=5 -Wswitch -Wswitch-default -Wundef \
            -Wunreachable-code -Wwrite-strings
PREREQFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS :=

COMPILE.c = $(CC) $(PREREQFLAGS) $(CPPFLAGS) $(CFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(PREREQFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

COMPILE = $(COMPILE.$(COMPILER))

# ------------------------------------------------------------------------------

release_CFLAGS :=
release_CXXFLAGS :=
release_CPPFLAGS := -DNDEBUG -O3
release_LDFLAGS :=

debug_CFLAGS :=
debug_CXXFLAGS :=
debug_CPPFLAGS := -DDEBUG -O0 -g
debug_LDFLAGS :=

# ------------------------------------------------------------------------------

# Expand a single local object dependency.
# $(1): variable to save the output to.
# $(2): dependency.
# $(3): configuration, e.g.: debug, release.
# $(4): architecture, e.g. x86-64, i386.
define EXPAND_LOCALDEP =
$(1) := $$($1) $$($(4)_$(3)_$(2)_OBJECTS)
endef

# Expand a bunch of local object dependencies.
# $(1): variable to save the output to.
# $(2): dependencies.
# $(3): configuration, e.g.: debug, release.
# $(4): architecture, e.g. x86-64, i386.
define EXPAND_LOCALDEPS =
$$(foreach _i,$(2),$$(eval $$(call \
    EXPAND_LOCALDEP,$(1),$$(_i),$(3),$(4))))
endef

# Create build rules for object targets.
# $(1): source files.
# $(2): source path, e.g.: src, demos/test.
# $(3): target name.
# $(4): target path, e.g.: dekoi, demos/test.
# $(5): prefix for variable names.
# $(6): configuration, e.g.: debug, release.
# $(7): architecture, e.g. x86-64, i386.
define CREATE_OBJECT_RULES =
$(7)_$(6)_$(5)_OBJECTS := \
    $$($(1):$(2)/%.c=$$(OBJECTDIR)/$(7)/$(6)/$(4)/$(3)/%.o)
$(7)_$(6)_$(5)_PREREQS := $$($(7)_$(6)_$(5)_OBJECTS:.o=.d)

$$($(7)_$(6)_$(5)_OBJECTS): CFLAGS += \
    $$($(5)_CFLAGS) $$($(6)_CFLAGS) $$($(7)_CFLAGS) \
    $$($(7)_$(6)_CFLAGS)
$$($(7)_$(6)_$(5)_OBJECTS): CXXFLAGS += \
    $$($(5)_CXXFLAGS) $$($(6)_CXXFLAGS) $$($(7)_CXXFLAGS) \
    $$($(7)_$(6)_CXXFLAGS)
$$($(7)_$(6)_$(5)_OBJECTS): CPPFLAGS += \
    $$($(5)_CPPFLAGS) $$($(6)_CPPFLAGS) $$($(7)_CPPFLAGS) \
    $$($(7)_$(6)_CPPFLAGS)

$$($(7)_$(6)_$(5)_OBJECTS): TARGET_ARCH := -march=$(7) -m$$(ENV)

$$($(7)_$(6)_$(5)_OBJECTS): $$(OBJECTDIR)/$(7)/$(6)/$(4)/$(3)/%.o: $(2)/%.c
	@ mkdir -p $$(@D)
	@ $$(COMPILE) -o $$@ $$<

-include $$($(7)_$(6)_$(5)_PREREQS)

ALL_SOURCES += $$($(7)_$(6)_$(5)_SOURCES)
ALL_HEADERS += $$($(7)_$(6)_$(5)_HEADERS)
endef

# Create build rules for binary targets.
# $(1): source files.
# $(2): source path, e.g.: src, demos/test.
# $(3): target name.
# $(4): target path, e.g.: dekoi, demos.
# $(5): prefix for variable names.
# $(6): configuration, e.g.: debug, release.
# $(7): architecture, e.g. x86-64, i386.
define CREATE_BINARY_RULES =
$(7)_$(6)_$(5)_OBJECTS := \
    $$($(1):$(2)/%.c=$$(OBJECTDIR)/$(7)/$(6)/$(4)/$(3)/%.o)
$(7)_$(6)_$(5)_TARGET := $$(BINARYDIR)/$(7)/$(6)/$(4)/$(3)
$(7)_$(6)_$(5)_DEPS :=
$(7)_$(6)_$(5)_LDLIBS := \
    $$($(5)_LDLIBS) $$($(6)_$(5)_LDLIBS) $$($(7)_$(5)_LDLIBS)

$$(eval $$(call \
    EXPAND_LOCALDEPS,$(7)_$(6)_$(5)_DEPS,$$($(5)_LOCALDEPS),$(6),$(7)))

$$(eval $$(call \
    CREATE_OBJECT_RULES,$(1),$(2),$(3),$(4),$(5),$(6),$(7)))

$$($(7)_$(6)_$(5)_TARGET): LDFLAGS += \
    $$($(6)_LDFLAGS) $$($(7)_LDFLAGS) $$($(7)_$(6)_LDFLAGS)

$$($(7)_$(6)_$(5)_TARGET): TARGET_ARCH := -march=$(7) -m$$(ENV)

$$($(7)_$(6)_$(5)_TARGET): $$($(7)_$(6)_$(5)_DEPS) $$($(7)_$(6)_$(5)_OBJECTS)
	@ mkdir -p $$(@D)
	@ $$(LINK.o) $$^ $$($(7)_$(6)_$(5)_LDLIBS) -o $$@
endef

# Create architecture specific build rules.
# $(1): build target, e.g.: BINARY.
# $(2): source files.
# $(3): source path, e.g.: src, demos/test.
# $(4): target name.
# $(5): target path, e.g.: dekoi, demos.
# $(6): prefix for variable names.
# $(7): configuration, e.g.: debug, release.
# $(8): architecture, e.g. x86-64, i386.
define CREATE_ARCH_RULES =
$$(eval $$(call \
    CREATE_$(1)_RULES,$(2),$(3),$(4),$(5),$(6),$(7),$(8)))

ifeq "$$(strip $(1))" "BINARY"
$(7)_$(6)_TARGETS += $$($(8)_$(7)_$(6)_TARGET)
endif
endef

# Create configuration specific build rules.
# $(1): build target, e.g.: BINARY.
# $(2): source files.
# $(3): source path, e.g.: src, demos/test.
# $(4): target name.
# $(5): target path, e.g.: dekoi, demos.
# $(6): prefix for variable names.
# $(7): configuration, e.g.: debug, release.
define CREATE_CONFIG_RULES =
$$(foreach _i,$$(ARCH),$$(eval $$(call \
    CREATE_ARCH_RULES,$(1),$(2),$(3),$(4),$(5),$(6),$(7),$$(_i))))

ifeq "$$(strip $(1))" "BINARY"
$(6)_TARGETS += $$($(7)_$(6)_TARGETS)
endif
endef

# Create all rule variations for a build.
# $(1): build target, e.g.: BINARY.
# $(2): source files.
# $(3): source path, e.g.: src, demos/test.
# $(4): target name.
# $(5): target path, e.g.: dekoi, demos.
# $(6): prefix for variable names.
define CREATE_RULES =
$$(foreach _i,$$(CONFIG),$$(eval $$(call \
    CREATE_CONFIG_RULES,$(1),$(2),$(3),$(4),$(5),$(6),$$(_i))))
endef

# ------------------------------------------------------------------------------

ALL_SOURCES :=
ALL_HEADERS :=

# ------------------------------------------------------------------------------

TOOLS_TARGETS :=
TOOLS_PHONYTARGETS :=

# Create the rules to build a tool target.
# $(1): target name.
# $(2): path.
# $(3): prefix for variable names.
define CREATE_TOOL_RULES =
$(3)_$(1)_SOURCES := \
    $$(wildcard $(2)/$(1)/*.c) $$(wildcard $(2)/$(1)/private/*.c)

$$(eval $$(call \
    CREATE_BINARY_RULES,$(3)_$(1)_SOURCES,$(2),$(1),$(2),$(3)_$(1),debug,$$(DEFAULT_ARCH)))

$(3)_$(1)_TARGET := $$($$(DEFAULT_ARCH)_debug_$(3)_$(1)_TARGET)

$(3)-$(1): $$($(3)_$(1)_TARGET)

.PHONY: $(3)-$(1)

TOOLS_TARGETS += $$($(3)_$(1)_TARGET)
TOOLS_PHONYTARGETS += $(3)-$(1)
ALL_SOURCES += $$($(3)_$(1)_SOURCES)
ALL_HEADERS += $$($(3)_$(1)_HEADERS)
endef

# Create the rules for all the tool targets.
# $(1): path.
# $(2): prefix for variable names.
define CREATE_TOOLS_RULES =
TOOLS := $$(notdir $$(wildcard $(1)/*))
$$(foreach _i,$$(TOOLS),$$(eval $$(call \
    CREATE_TOOL_RULES,$$(_i),$(1),$(2))))

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
