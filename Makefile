# Default target
for = linux

include targets/$(for)/config

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

$(librariesDirectory)/%.o: $(applicationDirectory)/%.c
	$(compiler) -x c $(compilerOptions) $< -o $@

$(librariesDirectory)/%.o: $(applicationDirectory)/%.cpp
	$(compiler) $(compilerOptions) $< -o $@
	
$(librariesDirectory)/%.o: $(sourceDirectory)/%.cpp
	mkdir -p $(@D)
	$(compiler) $(compilerOptions) $< -o $@

libraries_%: $(librariesDirectory) getdependencies_%
	@( echo "C libraries compilation..." ; \
	for source in `cd $(sourcesDirectory); find . -type f -name '*.c'`; \
	do \
		library=$$(echo $${source%.c}.o | sed "s|^\.|$(librariesDirectory)|g") ; \
		if [ $$(echo -n "$(libraries)" | grep "$${library}" | wc -l) = "1" ] ; then \
			mkdir -p $(librariesDirectory)/`dirname $$source` ; \
			if [ ! -e $(librariesDirectory)/$${source%.c}.o -o $(sourcesDirectory)/$$source -nt $(librariesDirectory)/$${source%.c}.o ] ; then \
				echo "$(compiler) -x c $(compilerOptions) $(sourcesDirectory)/$$source -o $(librariesDirectory)/$${source%.c}.o" ; \
				$(compiler) -x c $(compilerOptions) $(sourcesDirectory)/$$source -o $(librariesDirectory)/$${source%.c}.o ; \
				if [ $$? -eq 1 ] ; then \
					exit 1 ; \
				fi ; \
			fi ; \
		fi ; \
	done )
	@( echo "C++ libraries compilation..." ; \
	for source in `cd $(sourcesDirectory); find . -type f -name '*.cpp'`; \
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

debug: $(application)
	@( if [ ! -z "$(application)" ] ; then \
		ddd $(application) ; \
	fi )

run: $(application)
	@( if [ ! -z "$(application)" ] ; then \
		$(application) ; \
	fi )
	
run_%: $(application)
	@( if [ ! -z "$(application)" ] ; then \
		remoteName="$@" ; \
		remoteName=$${remoteName#run_} ; \
		if [ -e "remote-hosts.conf" ] ; then \
			if [ "`grep "^$${remoteName}\s\+" remote-hosts.conf | wc -l`" != "0" ] ; then \
				address=`grep "^$${remoteName}\s\+" remote-hosts.conf | awk '{print $$2}'` ; \
				applicationName=`basename "$(application)"` ; \
				scp $(application) $${address}:arcade/$${applicationName} ; \
				ssh -t $${address} "cd arcade ; ./$${applicationName}" ; \
			else \
				echo "Remote host \"$${remoteName}\" could not be found in \"remote-hosts.conf\"." ; \
			fi ; \
		else \
			echo "File \"remote-hosts.conf\" is absent." ; \
			echo "Please create it with name-ip association like:" ; \
			echo "example   1.2.3.4" ; \
			echo "example2  example.com" ; \
			echo "" ; \
			echo "Then call \"run_example\" target to remotely execute specified application." ; \
			exit 1 ; \
		fi ; \
	fi )

cleanlib:
	rm -rf $(librariesDirectory)/*
	
clean:
	find . -name '*~' | xargs rm -f
	rm -f $(binariesDirectory)/*

