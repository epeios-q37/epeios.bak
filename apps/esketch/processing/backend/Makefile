# 'esketchbkd' Project
#    by Claude SIMON (http://zeusw.org/intl/contact.html).
#
# Requires GNU make (http://www.gnu.org/software/make/).
#
#	Copyright (C) 2013 Claude SIMON.
#
#	This file is part of 'eSketch'.
#
#    'eSketch' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'eSketch' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.

name = esketchbkd


# Epeios general debugging features
copt += -DE_DEBUG
# Required for multitasking
#copt += -DE_MT
# For using <setjmp.h> instead of C++ exceptions.
#copt += -DERR_JMPUSE
	
mods += ags bso cpe epsmsc err 
mods += flw ias str tol ttr 
mods += uys flx cio dtfptb bom 
mods += cdgb64 fil fnm lcl rgstry 
mods += stsfsm xml xpp 

mods += sclmisc sclerror scllocale sclrgstry scldaemon 
mods += sclbacknd 

mods += fblber fblbkd fblbrd fblbrr fblcmd 
mods += fblcst fbltyp 

mods += csdleo csdles 

mods += registry wrpexample wrpunbound 



##############################
# Common to all environment. #
##############################

# Generates debugging data.
#CXXFLAGS += -g

# General optimization.
CXXFLAGS += -O

# To use with gprof
#CXXFLAGS += -pg

###############################
	
IA_32=x86
x86_64=x64
Cygwin=Cygwin
MinGW=Msys
GNULinux=GNU/Linux
Linux=Linux
MacOS=Darwin
os=$(shell uname -o 2>/dev/null || uname -s)
  

##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","$(Cygwin)")

# By default, vanilla Cygwin compiler ('g++') is used.
# Resulting application uses the Cygwin POSIX Layer.
# Currently, only 32 bits application can be made.
# For 64 bits and/or application using
# directly the MS CRT, see below.

# Use of the MinGW compiler under Cygwin. 
# To build 64 bits application.
# Uses directly the MS CRT.
	#arch=$(X86_64)

# Use of the MinGW compiler under Cygwin.
# For 32 bits application using directly the MS CRT.
	#arch=$(IA_32)

endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")

endif

#########################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(GNULinux)")

endif

#############################
	

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(Linux)")

endif

#############################
	

##########################
# For MacOS environment #
##########################

ifeq ("$(os)","$(MacOS)")

endif

##########################
		
###################################
###################################
##### DON'T MODIFY BELOW !!! ######
###################################
###################################
			

##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","$(Cygwin)")

	co += -DCYGWIN

	LDFLAGS += -static

	ifeq ("$(arch)","$(IA_32)")
		CXX = i686-w64-mingw32-g++
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900)
		ifeq ("$(arch)","$(x86_64)")
			CXX = x86_64-w64-mingw32-g++
		endif
    
	endif
	
	target=$(name).dll
	lo += -Wl,--kill-at -shared

endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")

	co += -DMSYS

	LDFLAGS += -static

	ifeq ("$(arch)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(arch)","$(x86_64)")
			co += -m64
			lo += -m64
		endif
	endif

	target=$(name).dll
	lo += -Wl,--kill-at -shared
endif

#########################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(GNULinux)")

	co += -DGNULINUX

	ifeq ("$(arch)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(arch)","$(x86_64)")
			co += -m64
			lo += -m64
		endif
	endif
	target=lib$(name).so
	lo += -shared
	co += -fPIC
endif

#############################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(Linux)")

	co += -DLINUX

	ifeq ("$(arch)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(arch)","$(x86_64)")
			co += -m64
			lo += -m64
		endif
	endif
	target=lib$(name).so
	lo += -shared
	co += -fPIC
endif

#############################
		

##########################
# For MacOS environment #
##########################

ifeq ("$(os)","$(MacOS)")

	co += -DDARWIN

	#LDFLAGS += -static	// Doesn't work.

	ifeq ("$(arch)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(arch)","$(x86_64)")
			co += -m64
			lo += -m64
		endif
	endif
	target=lib$(name).dylib
	lo += -shared
	co += -fPIC

endif

##########################
		
src += :..
src += :../..

include Details.mk

copt += -DVERSION=\"$(version)\"
copt += -DCOPYRIGHT_YEARS=\"$(copyright_years)\"
	
ifndef EPEIOS_SRC
	src += :../epeios:epeios
else
	src += :$(EPEIOS_SRC)
endif
    
ox = o
ds = :

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co := -c $(co) $(copt) $(subst :, -I,$(src))
lo += -o $(target) $(objs)

objs = $(name).$(ox) $(mods:=.$(ox))

$(target): $(objs)
	$(CXX) $(LDFLAGS) $(lo) $(libs)

%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<
		