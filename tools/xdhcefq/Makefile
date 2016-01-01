#	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).
#
#	This file is part of xdhcefq.
#
#	xdhcefq is free software: you can redistribute it and/or
#	modify it under the terms of the GNU Affero General Public License
#	published by the Free Software Foundation, either version 3 of the
#	License, or (at your option) any later version.
#
#	xdhcefq is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#	Affero General Public License for more details.
#
#	You should have received a copy of the GNU Affero General Public License
#	along with xdhcefq. If not, see <http://www.gnu.org/licenses/>.

name = xdhcefq


# Epeios general debugging features
copt += -DE_DEBUG
# Required for multitasking
#copt += -DE_MT
# For using <setjmp.h> instead of C++ exceptions.
#copt += -DERR_JMPUSE
	
mods += ags aem bch bitbch bso 
mods += cio cpe crptgr cslio ctn 
mods += dir dte dtfbsc dtfptb epsmsc 
mods += err fdr flf flsq flw 
mods += flx ias ids iof iop 
mods += lst lstbch lstctn mns mtx 
mods += ntvstr que sdr stk str 
mods += strng tagsbs tol txf tys 
mods += uys utf xtf llio tht 
mods += bomhdl cdgb64 fil fnm lcl 
mods += rgstry stsfsm xml xpp dlbrry 
mods += mtk strmrg 
mods += sclmisc sclerror scllocale sclrgstry sclargmnt 
mods += scltool 
mods += xdhujp xdhujr xdhujs 
mods += xdhcmn xdhups xdhutl 
mods += agent browser files misc registry 
mods += render 

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
	
copt += -DE_MT

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
  
cef = $(CEF)
      

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
	
	binary=$(name).exe
	co += -fpermissive
	libs += -L$(cef)/Release -llibcef

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

	binary=$(name).exe
	libs += -L$(cef)/Release -llibcef

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
	binary=$(name)
	 libs += -L$(cef)/Release -lcef -ludev
	 lo += -Wl,-rpath=$(cef)/Release/
     

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
	binary=$(name)
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
	binary=$(name)
	lo += -Wl,-search_paths_first -Wl,-ObjC -Wl,-pie -Wl,-search_paths_first -Wl,-headerpad_max_install_names -lpthread -framework Cocoa -framework AppKit
	libs += "$(cef)/Debug/Chromium Embedded Framework.framework/Chromium Embedded Framework" # /Users/csimon/cef_binary_3.2454.1344.g2782fb8_macosx64/build/libcef_dll/Debug/libcef_dll_wrapper.a
	
	ifdef EPEIOS_SRC
		xcfgb=<xpp:bloc><xpp:define name=\"ProcessType\"><xpp:bloc>
		xcfga=</xpp:bloc></xpp:define><xpp:expand href=\"/Users/csimon/epeios/tools/xdhcefq/xdhcefq.xcfg\"/></xpp:bloc>
		xlcl=<xpp:expand href=\"/Users/csimon/epeios/tools/xdhcefq/xdhcefq.xlcl\"/>
	endif
	
	mods += cefosx
	

	dest=/Users/csimon/bin/
endif

##########################
		
all: $(binary)
	rm -f *.o

copt += -DVERSION=\""20151030"\"
copt += -DCOPYRIGHT_YEARS=\""2015"\"

src += :"$(cef)"	# Had to be here (before the 'src += ...' below),
					# otherwise 'MinGW' under 'MSYS' does not find some header files...
      
ifndef EPEIOS_SRC
	src += :src:src/epeios
else
	src += :$(EPEIOS_SRC)
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
ifdef EPEIOS_SRC
		
# No tabulation, otherwise it's passed to 'bash', until to be handled by 'make.
ifeq ("$(os)","$(MacOS)")
		cp $(binary) "$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/MacOS/cefsimple"
		install_name_tool -change "@executable_path/Chromium Embedded Framework" "@executable_path/../Frameworks/Chromium Embedded Framework.framework/Chromium Embedded Framework" "$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/MacOS/cefsimple"
		install_name_tool -change "@executable_path/Chromium Embedded Framework" "@executable_path/../../../Chromium Embedded Framework.framework/Chromium Embedded Framework" $(binary)
		cp $(binary) "$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper.app/Contents/MacOS/cefsimple Helper"
# 		cp $(binary) "$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper EH.app/Contents/MacOS/cefsimple Helper EH"
# 		cp $(binary) "$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper NP.app/Contents/MacOS/cefsimple Helper NP"
		echo "$(xcfgb)Main$(xcfga)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/MacOS/xdhcefq.xcfg"
		echo "$(xlcl)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/MacOS/xdhcefq.xlcl"
		echo "$(xcfgb)Sub$(xcfga)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper.app/Contents/MacOS/xdhcefq.xcfg"
		echo "$(xlcl)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper.app/Contents/MacOS/xdhcefq.xlcl"
# 		echo "$(xcfgb)Sub$(xcfga)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper EH.app/Contents/MacOS/xdhcefq.xcfg"
# 		echo "$(xlcl)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper EH.app/Contents/MacOS/xdhcefq.xlcl"
# 		echo "$(xcfgb)Sub$(xcfga)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper NP.app/Contents/MacOS/xdhcefq.xcfg"
# 		echo "$(xlcl)" | xppq --#Output="$(cef)/build/cefsimple/Debug/cefsimple.app/Contents/Frameworks/cefsimple Helper NP.app/Contents/MacOS/xdhcefq.xlcl"
endif
				
	mkdir -p $(dest)
			
	cp $(binary) $(dest)
endif
		
cefosx.o : cefosx.mm
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<
			

%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<

clean:
	rm -rf *.o

		