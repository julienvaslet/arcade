#!/bin/bash
basedir=$(cd `dirname $0`; pwd)
includeDirectory="${basedir}/include"
sourceDirectory="${basedir}/source"
verbose=0

file=$1

if [ -z "${file}" ]
then
	echo "Find all #include dependencies from the specified file."
	echo "Usage: $(basename $0) <file>"
	exit 1
fi

if [ ! -e "${file}" ]
then
	echo "The specified file does not exist."
	exit 2
fi

# Read files are stored to avoid cycles
readFiles=""
includedFiles=""

function log ()
{
	if [ ${verbose} -eq 1 ]
	then
		echo $*
	fi
}

function getIncludedFiles ()
{
	if [ $(echo -e "${readFiles}" | grep "$1" | wc -l) = "0" ]
	then
		log "Parsing files: $*"
		
		includes=$(grep -o "^#include\s*<[^>]\+" $* | sed 's/^[^<]\+<//g')
	
		files=""
		for file in $*
		do
			if [ -z "${file}" ]
			then
				files="${file}"
			else
				files="${files}\n${file}"
			fi
		done
	
		if [ -z "${readFiles}" ]
		then
			readFiles="${files}"
		else
			readFiles="${readFiles}\n${files}"
		fi
	
		OLDIFS=${IFS}
		IFS=$'\n'
		for include in ${includes}
		do
			IFS=${OLDIFS}
			
			if [ -e "${includeDirectory}/${include}" ]
			then
				if [ $(echo -n "${includedFiles}" | grep "${include}" | wc -l) = "0" ]
				then
					if [ -z "${includedFiles}" ]
					then
						includedFiles="${include}"
					else
						includedFiles="${includedFiles}\n${include}"
					fi

					getIncludedFiles ${includeDirectory}/${include} ${sourceDirectory}/${include%.h}.cpp
				fi
			fi
			
			IFS=$'\n'
		done
		IFS=${OLDIFS}
	else
		log "File already parsed: ${file}"
	fi
}

# Parsing includes
getIncludedFiles ${file}

echo -e ${includedFiles} | sed 's/.h$/.o/g'
