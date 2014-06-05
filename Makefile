# List of packages
PACKAGES = \
	core \
	board \
	system \
	test


VIRTUAL_PACKAGES = $(addsuffix /.virtual.Makefile,${PACKAGES})

FLAGS= -$(MAKEFLAGS)

ifeq ($(MAKEFLAGS), "")
  FLAGS= ""
endif

# Makefile targets declared as phony
TARGETS = build rpm clean 
.PHONY = $(TARGETS)

default: build

# Each target in TARGET will be built using the targets in VIRTUAL_PACKAGES
$(TARGETS): $(VIRTUAL_PACKAGES)

$(VIRTUAL_PACKAGES):
	$(MAKE) $(FLAGS) -C $(@D) $(MAKECMDGOALS)
