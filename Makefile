# Default target
for = linux

debugMaxLevel = 3
debugFlag = -g -DDEBUG0 -DDEBUG1 -DDEBUG2 -DDEBUG3

include targets/$(for)/config

all: 

$(binariesDirectory):
	mkdir -p $@

$(librariesDirectory):
	mkdir -p $@

.PRECIOUS: $(binariesDirectory)/% $(librariesDirectory)/%.o
.PHONY: run clean cleanlib all libraries

%: $(binariesDirectory)/%
	$(eval application = $<)

$(binariesDirectory)/%: $(binariesDirectory) $(librariesDirectory)/%.o
	$(linker) -o $@$(applicationSuffix) `find $(librariesDirectory) -name '*.o' -type f` $(linkerOptions)

$(librariesDirectory)/%.o: $(applicationDirectory)/%.cpp libraries
	$(compiler) $(compilerOptions) $< -o $@
	
libraries: $(librariesDirectory)
	@( for source in `cd $(sourcesDirectory); find . -name '*.cpp' -type f`; \
	do \
		mkdir -p $(librariesDirectory)/`dirname $$source` ; \
		if [ ! -e $(librariesDirectory)/$$source.o -o $(sourcesDirectory)/$$source -nt $(librariesDirectory)/$$source.o ] ; then \
			echo "$(compiler) $(compilerOptions) $(sourcesDirectory)/$$source -o $(librariesDirectory)/$$source.o" ; \
			$(compiler) $(compilerOptions) $(sourcesDirectory)/$$source -o $(librariesDirectory)/$$source.o ; \
			if [ $$? -eq 1 ] ; then \
				exit 1 ; \
			fi ; \
		fi ; \
	done )

run: $(application)
	@( if [ ! -z "$(application)" ] ; then \
		$(application) ; \
	fi )

debug: 
	@( make --no-print-directory debug0 )

debug%:
	@( target="$@"; \
	target=$${target#debug}; \
	for level in `seq $$target $(debugMaxLevel)`; \
	do \
		flags="-DDEBUG$$level $$flags"; \
	done ; \
	make --no-print-directory run debugFlag="$$flags -g" )

cleanlib:
	rm -rf $(librariesDirectory)/*
	
clean:
	find . -name '*~' | xargs rm -f
	rm -f $(binariesDirectory)/*

