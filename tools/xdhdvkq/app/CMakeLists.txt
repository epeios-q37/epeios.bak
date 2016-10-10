# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(
    h:/hg/epeios/stable
)

set (EPEIOS h:/hg/epeios/stable/ )

add_library( # Sets the name of the library.
            xdhdvkq

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/xdhdvkq.cpp
             ${EPEIOS}/ags.cpp
             ${EPEIOS}/bomhdl.cpp
             ${EPEIOS}/bso.cpp
             ${EPEIOS}/cdgb64.cpp
             ${EPEIOS}/cio.cpp
             ${EPEIOS}/ctn.cpp
             ${EPEIOS}/dir.cpp
             ${EPEIOS}/dlbrry.cpp
             ${EPEIOS}/dtfptb.cpp
             ${EPEIOS}/err.cpp
             ${EPEIOS}/fil.cpp
             ${EPEIOS}/flw.cpp
             ${EPEIOS}/fnm.cpp
             ${EPEIOS}/ias.cpp
             ${EPEIOS}/lcl.cpp
             ${EPEIOS}/lst.cpp
             ${EPEIOS}/mtx.cpp
             ${EPEIOS}/ntvstr.cpp
             ${EPEIOS}/plgn.cpp
             ${EPEIOS}/rgstry.cpp
             ${EPEIOS}/sclargmnt.cpp
             ${EPEIOS}/sclerror.cpp
             ${EPEIOS}/scllocale.cpp
             ${EPEIOS}/sclmisc.cpp
             ${EPEIOS}/sclrgstry.cpp
             ${EPEIOS}/stsfsm.cpp
             ${EPEIOS}/str.cpp
             ${EPEIOS}/tagsbs.cpp
             ${EPEIOS}/tht.cpp
             ${EPEIOS}/tol.cpp
             ${EPEIOS}/uys.cpp
             ${EPEIOS}/xml.cpp
             ${EPEIOS}/xpp.cpp

#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
#             ${EPEIOS}/.cpp
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       xdhdvkq

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

 add_definitions(-DXXX_DEBUG)
