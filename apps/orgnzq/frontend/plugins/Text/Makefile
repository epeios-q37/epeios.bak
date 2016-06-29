#	Copyright (C) 2016 Claude SIMON (http://q37.info/contact/).
#
#	This file is part of orgnzq.
#
#	orgnzq is free software: you can redistribute it and/or
#	modify it under the terms of the GNU Affero General Public License
#	published by the Free Software Foundation, either version 3 of the
#	License, or (at your option) any later version.
#
#	orgnzq is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#	Affero General Public License for more details.
#
#	You should have received a copy of the GNU Affero General Public License
#	along with orgnzq. If not, see <http://www.gnu.org/licenses/>.

name = text


# Epeios general debugging features
copt += -DE_DEBUG
# Required for multitasking
#copt += -DE_MT
# For using <setjmp.h> instead of C++ exceptions.
#copt += -DERR_JMPUSE
	
mods += ags aem bch bitbch bso 
mods += cio cpe crptgr cslio crt 
mods += ctn dir dte dtfbsc dtfptb 
mods += epsmsc err fdr flf flsq 
mods += flw flx ias idsq iof 
mods += iop lst lstbch lstcrt lstctn 
mods += mns mtx ntvstr que sdr 
mods += stkbse stkbch stkctn str strng 
mods += tagsbs tol txf tys uys 
mods += utf xtf llio bomhdl cdgb64 
mods += dlbrry fil fnm lcl plgn 
mods += plgncore rgstry stsfsm xml xpp 
mods += tht 
mods += sclmisc sclerror scllocale sclrgstry sclargmnt 
mods += sclplugin 
mods += frdcllbck frdplgn 
mods += cmnplgn cmnrgstry 
mods += registry 

pmods += pllio 

wmods += wllio 

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
	
copt += -DQ37_LIBRARY

win=win
win32=win32
win64=win64
IA_32=x86
AMD64=x64
Cygwin=Cygwin
MinGW=Msys
GNULinux=GNU/Linux
Linux=Linux
MacOS=Darwin
os=$(shell uname -o 2>/dev/null || uname -s)
mach=$(shell uname -m)
  
src += :../

src += :../../


##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","$(Cygwin)")

# By default, vanilla Cygwin compiler ('g++') is used.
# Resulting application uses the Cygwin POSIX Layer.
# directly the MS CRT, see below.

# Use of the MinGW compiler under Cygwin. 
# To build native binary, i.e. with the MS CRT,
# without using the Cygwin/POSIX layer,
# Uncomment follwing line :
#target=$(win)

endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")
	co += -fpermissive
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

	co += -std=gnu++11 -DUNICODE -D_FILE_OFFSET_BITS=64
	libs += -lws2_32

	LDFLAGS += -static
	
	ifeq ("$(target)","$(win)")
		ifeq ("$(mach)","i686")
			override target=$(win32)
		else  # 'ifeq' on other line due to GNU 3.80 (Maemo on N900)
			ifeq ("$(mach)","x86_64")
				override target=$(win64)
			endif
		endif
	endif
	
	ifeq ("$(target)","$(IA_32)")
		override target=$(win32)
	else  # 'ifeq' on other line due to GNU 3.80 (Maemo on N900)
		ifeq ("$(target)","$(AMD64)")
			override target=$(win64)
		endif
	endif
	
	ifeq ("$(target)","$(win32)")
		CXX = i686-w64-mingw32-g++
		mods += $(wmods)
		lo += -municode -m32
		co += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900)
		ifeq ("$(target)","$(win64)")
			CXX = x86_64-w64-mingw32-g++
			mods += $(wmods)
			lo += -municode -m64
			co += -m64
		else
			mods += $(pmods)
		endif
    
	endif
	
	binary=$(name).dll
	lo += -Wl,--kill-at -shared

	dest=/cygdrive/h/bin/

endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")

	co += -DMSYS -std=gnu++11 -DUNICODE
	lo += -municode
	
	mods += $(wmods)

	LDFLAGS += -static

	ifeq ("$(target)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(arch)","$(AMD64)")
			co += -m64
			lo += -m64
		endif
	endif

	binary=$(name).dll
	lo += -Wl,--kill-at -shared

	dest=/h/bin/
endif

#########################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(GNULinux)")

	co += -std=gnu++11 -DUNICODE -D_FILE_OFFSET_BITS=64
	
	mods += $(pmods)
	
	libs += -lpthread -ldl -lrt

	ifeq ("$(target)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(target)","$(AMD64)")
			co += -m64
			lo += -m64
		endif
	endif
	binary=lib$(name).so
	lo += -shared
	co += -fPIC

	dest=/home/csimon/bin/
endif

#############################
		

#########################
# For Linux environment #
#########################

ifeq ("$(os)","$(Linux)")

	co += -std=gnu++11 -DUNICODE -D_FILE_OFFSET_BITS=64
	
	mods += $(pmods)

	libs += -lpthread -ldl -lrt
	
	ifeq ("$(target)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(target)","$(AMD64)")
			co += -m64
			lo += -m64
		endif
	endif
	binary=lib$(name).so
	lo += -shared
	co += -fPIC
endif

#############################
		

##########################
# For MacOS environment #
##########################

ifeq ("$(os)","$(MacOS)")

	co += -std=gnu++11 -DUNICODE -D_FILE_OFFSET_BITS=64
	
	mods += $(pmods)

	ifeq ("$(target)","$(IA_32)")
		co += -m32
		lo += -m32
	else # 'ifeq' on other line due to GNU 3.80 (Maemo on N900).
		ifeq ("$(target)","$(AMD64)")
			co += -m64
			lo += -m64
		endif
	endif
	binary=lib$(name).dylib
	lo += -dynamiclib

	dest=/Users/csimon/bin/
endif

##########################
		
all: $(binary)
	rm -f *.o

copt += -DVERSION=\""20160622"\"
copt += -DCOPYRIGHT_YEARS=\""2016"\"
copt += -DIDENTIFIER=\""ea42259d-2d9d-454e-9f0e-fe45302613ea"\"

ifndef EPEIOS_SRC
	src = :src:src/epeios
else
	src += :$(EPEIOS_SRC)
endif

ifdef EPEIOS_OUT
	out = $(EPEIOS_OUT)
else
	out = $(dest)
endif

    
ox = o
ds = :

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co := -c $(co) $(copt) $(subst :, -I,$(src))
lo += -o $(binary) $(objs)

objs = $(name).$(ox) $(mods:=.$(ox))

$(binary): $(objs)
	$(CXX) $(lo) $(libs) $(LDFLAGS)
	mkdir -p $(out)plugins/ogz/Frontend/
	cp $(binary) $(out)plugins/ogz/Frontend/

%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<

clean:
	rm -rf *.o

		