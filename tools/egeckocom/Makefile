# 'egeckocom' Project
#    by Claude SIMON (http://zeusw.org/intl/contact.html).
#
# Requires GNU make (http://www.gnu.org/software/make/).
#
#	Copyright (C) 2012 - 2004 Claude SIMON.
#
#	This file is part of 'eGeckoCOM'.
#
#    'eGeckoCOM' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'eGeckoCOM' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.

name = egeckocom


# Epeios general debugging features
copt += -DE_DEBUG
# Required for multitasking
#copt += -DE_MT
# For using <setjmp.h> instead of C++ exceptions.
#copt += -DERR_JMPUSE
	
mods += ags aem bch bitbch bso 
mods += cpe crptgr ctn dir dte 
mods += dtfbsc dtfptb epsmsc err fdr 
mods += flf fls flw flx ias 
mods += ids iof iop lst lstbch 
mods += lstctn mns que sdr stk 
mods += str strng tol ttr txf 
mods += tys uys utf xtf cio 
mods += dlbrry mtk nsxpcm sck tht 
mods += xulwdg bom cdgb64 fil fnm 
mods += geckof lcl rgstry stsfsm xml 
mods += xpp 
mods += sclmisc sclerror scllocale sclrgstry 
mods += atcplsrch registry 



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
	
# Required for Gecko.

copt += -DE_GECKO

# Multitasking.

copt += -DE_MT

IA_32=x86
x86_64=x64
Cygwin=Cygwin
MinGW=Msys
GNULinux=GNU/Linux
Linux=Linux
MacOS=Darwin
os=$(shell uname -o 2>/dev/null || uname -s)
  
gdk = $(GDK)
      

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
	

#########################
# For Linux environment #
#########################

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
		mods += $(wmods)
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900)
		ifeq ("$(arch)","$(x86_64)")
			CXX = x86_64-w64-mingw32-g++
			mods += $(wmods)
		else
			mods += $(pmods)
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
	
	mods += $(wmods)

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
	
	mods += $(pmods)
	
	libs += -lpthread -ldl -lrt

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
	co += -std=c++0x
	co += -D__STDC_LIMIT_MACROS
    
endif

#############################
		

#########################
# For Linux environment #
#########################

ifeq ("$(os)","$(Linux)")

	co += -DLINUX
	
	mods += $(pmods)

	libs += -lpthread -ldl -lrt
	
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
	
	mods += $(pmods)

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
	co += -std=c++0x
	co += -D__STDC_LIMIT_MACROS
    

endif

##########################
		
all: $(target)
	rm -f *.o

copt += -DVERSION=\""20140120"\"
copt += -DCOPYRIGHT_YEARS=\""2012 - 2004"\"

ifndef EPEIOS_SRC
	src += :src
else
	src += :$(EPEIOS_SRC)
endif

    
src += :"$(gdk)/include":"$(gdk)/include/nspr"
      
ox = o
ds = :

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co := -c $(co) $(copt) $(subst :, -I,$(src))
lo += -o $(target) $(objs)

objs = $(name).$(ox) $(mods:=.$(ox))

$(target): $(objs)
	$(CXX) $(lo) $(libs) $(LDFLAGS)

%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<

clean:
	rm -rf *.o

		