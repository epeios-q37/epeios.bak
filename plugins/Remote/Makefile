#	Copyright (C)  Claude SIMON (http://q37.info/contact/).
#
#	This file is part of Remote.
#
#	Remote is free software: you can redistribute it and/or
#	modify it under the terms of the GNU Affero General Public License
#	published by the Free Software Foundation, either version 3 of the
#	License, or (at your option) any later version.
#
#	Remote is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#	Affero General Public License for more details.
#
#	You should have received a copy of the GNU Affero General Public License
#	along with Remote. If not, see <http://www.gnu.org/licenses/>.


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
  

##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","$(Cygwin)")
	exe=.exe
	libp=
	libs=.dll
endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")
	exe=.exe
	libp=
	libs=.dll
endif

#########################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(GNULinux)")
	exe=
	libp=lib
	libs=.so
endif

#############################
		

#########################
# For Linux environment #
#########################

ifeq ("$(os)","$(Linux)")
	exe=
	libp=lib
	libs=.so
endif

#############################
		

##########################
# For MacOS environment #
##########################

ifeq ("$(os)","$(MacOS)")
	exe=
	libp=lib
	libs=.dylib
endif

##########################
		
all : common
ifndef EPEIOS_SRC
	mkdir -p out
	echo \<xpp:expand href=\"../Proxy/proxy.xcfg\"/\> >out/proxy.xcfg
	echo \<xpp:expand href=\"../Proxy/proxy.xlcl\"/\> >out/proxy.xlcl
	cp Proxy/$(libp)proxy$(libs) out
	echo \<xpp:expand href=\"../Straight/straight.xcfg\"/\> >out/straight.xcfg
	echo \<xpp:expand href=\"../Straight/straight.xlcl\"/\> >out/straight.xlcl
	cp Straight/$(libp)straight$(libs) out
endif

clean : common

common:
ifndef EPEIOS_SRC
endif
	$(MAKE) $(MAKECMDGOALS) -C Proxy/
	$(MAKE) $(MAKECMDGOALS) -C Straight/
