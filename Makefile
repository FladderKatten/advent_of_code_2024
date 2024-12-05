DAYS := $(wildcard day-*/)
all: $(DAYS)
	@echo "success"

# Rule to run "make run" in each day directory
$(DAYS):
	@$(MAKE) -C $@

.PHONY: all $(DAYS)
