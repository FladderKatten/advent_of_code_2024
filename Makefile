DIRS := $(wildcard day-*/)
all: $(DIRS)
	@echo "success"

$(DIRS):
	@$(MAKE) -C $@

.PHONY: all $(DIRS)

clean:
	@for dir in $(DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	@echo "Clean complete."