DAYS := $(wildcard day-*/)
all: $(DAYS)
	@echo "success"

$(DAYS):
	@$(MAKE) -C $@

.PHONY: all $(DAYS)
