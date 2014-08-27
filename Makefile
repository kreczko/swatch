PACKAGES = \
	core \
	core/test \
	processor \
	processor/test \
	system \
	system/test \
	test 

VIRTUAL_PACKAGES = $(addsuffix /.virtual.Makefile,${PACKAGES})

ifeq ($(MAKEFLAGS), "")
  FLAGS= ""
else
  FLAGS= $(MAKEFLAGS)
endif

# Makefile targets declared as phony
TARGETS = build rpm clean cleanrpm
.PHONY = $(TARGETS)

default: build

# Each target in TARGET will be built using the targets in VIRTUAL_PACKAGES
$(TARGETS): $(VIRTUAL_PACKAGES)

$(VIRTUAL_PACKAGES):
	$(MAKE) $(FLAGS) -C $(@D) $(MAKECMDGOALS)
