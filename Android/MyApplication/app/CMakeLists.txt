# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror  -D_ANDROID_SDK -D_MS_WIN_")

add_library( bmelib
            SHARED
             src/main/cpp/bmelib.cpp
                                 src/main/cpp/sci_beep_old.cpp
                                 src/main/cpp/sci_bitcorrect.cpp
                                 src/main/cpp/sci_dpskmod.cpp
                                 src/main/cpp/sci_fft.cpp
                                 src/main/cpp/sci_preamble.cpp
                                 src/main/cpp/sci_rs.cpp
                                 src/main/cpp/sci_transform.cpp
                                 src/main/cpp/sci_transmitter.cpp
                                 src/main/cpp/sci_transmitter_helper.cpp

             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( log-lib
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( bmelib
                       ${log-lib} )