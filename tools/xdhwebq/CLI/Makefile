#	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).
#
#	This file is part of XDHWebQ.
#
#	XDHWebQ is free software: you can redistribute it and/or
#	modify it under the terms of the GNU Affero General Public License
#	published by the Free Software Foundation, either version 3 of the
#	License, or (at your option) any later version.
#
#	XDHWebQ is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#	Affero General Public License for more details.
#
#	You should have received a copy of the GNU Affero General Public License
#	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.

name = xdhwebq


# Epeios general debugging features
copt += -DE_DEBUG
# Required for multitasking
#copt += -DE_MT
# For using <setjmp.h> instead of C++ exceptions.
#copt += -DERR_JMPUSE
	
mods += ags aem bch bitbch bomhdl 
mods += bso cdgb64 cio cpe crptgr 
mods += cslio crt ctn dir dte 
mods += dtfbsc dtfptb epsmsc err fdr 
mods += fil flf flsq flw flx 
mods += fnm ias idsq iof iop 
mods += lcl lck lst lstbch lstcrt 
mods += lstctn mns mtk mtx ntvstr 
mods += osd que rgstry sdr stkbse 
mods += stkbch stkcrt stkctn str strng 
mods += stsfsm tagsbs tht thtsub tol 
mods += txf tys uys utf xml 
mods += xpp xtf llio dlbrry plgn 
mods += plgncore btr idxbtq idxbtr idxque 
mods += sck sha1 strmrg websck 
mods += csdbns csdcmn csdscb 
mods += scla scli sclm scle scll 
mods += sclr sclt 
mods += xdhbrd xdhcdc xdhcmn xdhcuc xdhups 
mods += xdhutl 
mods += xdwsessn xdwrgstry 
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
	
copt += -DE_ST

win=win
win32=win32
win64=win64
IA_32=x86
AMD64=x64
Cygwin=Cygwin
MinGW=Msys
GNULinux=GNU/Linux
FreeBSD=FreeBSD
Linux=Linux
MacOS=Darwin
Android=Android
os=$(shell uname -o 2>/dev/null || uname -s)
mach=$(shell uname -m)
ifdef Q37_EPEIOS
 src += :../
endif


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
# By default, 'CXX' is set to 'g++'.
# 	CXX = clang++
endif

#############################
	

###########################
# For FreeBSD environment #
###########################

ifeq ("$(os)","$(FreeBSD)")
# By default, 'CXX' is set to 'clang++'.
# 	CXX = g++
endif

###########################
	

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
		

####################################
# For Android (Termux) environment #
####################################

ifeq ("$(os)","$(Android)")

endif

#############################
	
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
	
	ifneq ("$(target)","$(Android)")
		libs += -lws2_32
		LDFLAGS += -static
	endif
	
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
			ifeq ("$(target)","$(Android)")
				CXX = C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe
				mods += $(pmods)
				binary=lib$(name).so
				
				lo += -target i686-none-linux-android -gcc-toolchain C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/toolchains/x86-4.9/prebuilt/windows-x86_64 --sysroot=C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/platforms/android-14/arch-x86 -fPIC -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wa,--noexecstack -Wformat -Werror=format-security -fno-exceptions -fno-rtti -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wa,--noexecstack -Wformat -Werror=format-security -fno-exceptions -fno-rtti -fexceptions -std=c++11 -O0 -fno-limit-debug-info -O0 -fno-limit-debug-info  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now -shared
				
				co += -target i686-none-linux-android -gcc-toolchain C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/toolchains/x86-4.9/prebuilt/windows-x86_64 --sysroot=C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/platforms/android-14/arch-x86  -DXXX_DEBUG -Ih:/hg/epeios/stable -isystem C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/include -isystem C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/libs/x86/include -isystem C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/include/backward -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wa,--noexecstack -Wformat -Werror=format-security -fno-rtti -fexceptions -std=c++11 -O0 -fno-limit-debug-info  -fPIC -MD
				
				libs += -ldl -landroid -llog -lm "C:/Users/csimon/AppData/Local/Android/sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/libs/x86/libgnustl_static.a"
			else
				mods += $(pmods)
			endif
		endif
    
	endif
	
	binary=$(name).exe

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

	dest=/home/csimon/bin/
endif

#############################
		

###########################
# For FreeBSD environment #
###########################

ifeq ("$(os)","$(FreeBSD)")
 
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

	dest=/home/csimon/bin/
endif

###########################
		

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

	dest=/Users/csimon/bin/
endif

##########################
		

####################################
# For Android (Termux) environment #
####################################

ifeq ("$(os)","$(Android)")
 
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
		
all: $(binary)
	rm -rf *.o
ifeq ("$(target)","$(Android)")
	rm -rf *.d
endif

copt += -DVERSION=\""20220625"\"
copt += -DCOPYRIGHT_YEARS=\""2017"\"
copt += -DIDENTIFIER=\""effd756a-a901-46e8-939f-3f2a5e7986b0"\"

ifndef Q37_EPEIOS
	src += :src:src/epeios
	out = ./
else
	src += :$(Q37_EPEIOS)/stable
	out = $(dest)
endif

    
ox = o
ds = :

vpath %.cpp $(subst $(ds),:,$(src))
vpath %.mm $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co := -c $(co) $(copt) $(subst :, -I,$(src))
lo += -o $(binary) $(objs)

objs = $(name).$(ox) $(mods:=.$(ox))

$(binary): $(objs)
	$(CXX) $(lo) $(libs) $(LDFLAGS)
ifdef Q37_EPEIOS			
	mkdir -p $(out)
	cp $(binary) $(out)
endif

%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<

clean:
	rm -rf *.o
ifeq ("$(target)","$(Android)")
	rm -rf *.d
endif


		