# Default target
for = linux

include targets/$(for)/config

# Are we on a Pidora Raspberry Pi ?
ifneq (,$(filter $(for),pi pi-prod))
isRaspberryPi := $(shell if [ $(shell grep "^Pidora" /etc/redhat-release | wc -l) = "1" ] ; then echo true ; else echo false; fi)
else
isRasberryPi = true
endif

# If we are not on a Raspberry Pi, we launch QEMU
ifeq ($(isRaspberryPi),false)

all:
	sudo TARGET=$(for) ./tools/compile.sh $(shell ls -1 $(applicationDirectory) | grep '\.cpp$$' | sed 's/\.cpp$$//g')

%:
	sudo TARGET=$(for) ./tools/compile.sh $@
	
run:
	

else

all:

$(binariesDirectory):
	mkdir -p $@

$(librariesDirectory):
	mkdir -p $@

.PRECIOUS: $(binariesDirectory)/%
.PHONY: run clean cleanlib all
.SECONDEXPANSION:

%: $(binariesDirectory)/%
	$(eval application = $<)
	
getdependencies_%: get-dependencies.sh
	$(eval libraries := $(shell app="$@" ; app=$${app#getdependencies_} ; ./get-dependencies.sh $(applicationDirectory)/$${app}.cpp | sed 's|^.*$$|$(librariesDirectory)/\0|g'))

$(binariesDirectory)/%: $(librariesDirectory)/%.o $(binariesDirectory) libraries_%
	$(linker) -o $@$(applicationSuffix) $< $(libraries) $(linkerOptions)

$(librariesDirectory)/%.o: $(applicationDirectory)/%.cpp
	$(compiler) $(compilerOptions) $< -o $@
	
$(librariesDirectory)/%.o: $(sourceDirectory)/%.cpp
	mkdir -p $(@D)
	$(compiler) $(compilerOptions) $< -o $@

libraries_%: $(librariesDirectory) getdependencies_%
	@( for source in `cd $(sourcesDirectory); find . -name '*.cpp' -type f`; \
	do \
		library=$$(echo $${source%.cpp}.o | sed "s|^\.|$(librariesDirectory)|g") ; \
		if [ $$(echo -n "$(libraries)" | grep "$${library}" | wc -l) = "1" ] ; then \
			mkdir -p $(librariesDirectory)/`dirname $$source` ; \
			if [ ! -e $(librariesDirectory)/$${source%.cpp}.o -o $(sourcesDirectory)/$$source -nt $(librariesDirectory)/$${source%.cpp}.o ] ; then \
				echo "$(compiler) $(compilerOptions) $(sourcesDirectory)/$$source -o $(librariesDirectory)/$${source%.cpp}.o" ; \
				$(compiler) $(compilerOptions) $(sourcesDirectory)/$$source -o $(librariesDirectory)/$${source%.cpp}.o ; \
				if [ $$? -eq 1 ] ; then \
					exit 1 ; \
				fi ; \
			fi ; \
		fi ; \
	done )

run: $(application)
	@( if [ ! -z "$(application)" ] ; then \
		$(application) ; \
	fi )

endif

cleanlib:
	rm -rf $(librariesDirectory)/*
	
clean:
	find . -name '*~' | xargs rm -f
	rm -f $(binariesDirectory)/*

