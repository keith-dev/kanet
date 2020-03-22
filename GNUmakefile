PROJECTS = src

.PHONY: $(PROJECTS)

all: $(PROJECTS)

clean:
	for P in $(PROJECTS); do $(MAKE) --directory=$$P clean; done

$(PROJECTS):
	$(MAKE) -C $@
