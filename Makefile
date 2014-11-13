Set?=devel
ifeq ($(Set), devel)
PACKAGES = \
	logger\
	core \
	core/test \
	processor \
	processor/test \
	system \
	system/test \
	test \
	pyswatch
else ifeq ($(Set), pro)
PACKAGES = \
	logger\
	core \
	processor \
	system \
	test \
	pyswatch
else ifeq ($(Set), hwdevel)
PACKAGES = \
	logger\
	core \
	core/test \
	processor \
	processor/test \
	system \
	system/test \
	hardware \
	hardware/test \
	test \
	pyswatch
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
