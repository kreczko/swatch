Set?=devel
SWATCH_PACKAGES = \
	logger \
	core \
	xml \
	processor \
	system
SWATCH_TEST_PACKAGES = \
	logger/test \
	core/test \
	xml/test \
	processor/test \
	system/test \
	dummy \
	test
ifeq ($(Set), devel)
PACKAGES = \
	$(SWATCH_PACKAGES) \
	$(SWATCH_TEST_PACKAGES)
else ifeq ($(Set), pro)
PACKAGES = \
	$(SWATCH_PACKAGES) \
	pyswatch
else ifeq ($(Set), amc13)
PACKAGES = \
    $(SWATCH_PACKAGES) \
    $(SWATCH_TEST_PACKAGES) \
	amc13
else ifeq ($(Set), hwdevel)
PACKAGES = \
	$(SWATCH_PACKAGES) \
    $(SWATCH_TEST_PACKAGES) \
	amc13 \
	mp7
endif

VIRTUAL_PACKAGES = $(addsuffix /.virtual.Makefile,${PACKAGES})

FLAGS = $(ifeq $(MAKEFLAGS) "","",-$(MAKEFLAGS))

ifdef jobs
FLAGS+=-j ${jobs}
endif

# Makefile targets declared as phony
TARGETS = build rpm clean cleanrpm
.PHONY = $(TARGETS)

default: build

# Each target in TARGET will be built using the targets in VIRTUAL_PACKAGES
$(TARGETS): $(VIRTUAL_PACKAGES)

$(VIRTUAL_PACKAGES):
	$(MAKE) $(FLAGS) -C $(@D) $(MAKECMDGOALS)
