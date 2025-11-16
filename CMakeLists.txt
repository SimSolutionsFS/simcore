cmake_minimum_required(VERSION 3.21)
project(simcore C)

set(CMAKE_C_STANDARD 17)

# CMake Settings
set(CMAKE_C_FLAGS "-O3 -fPIC -pedantic -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

# Definitions for the X-Plane SDK
add_definitions(
	-DXPLM200
	-DXPLM210
	-DXPLM300
	-DXPLM301
	-DXPLM302
	-DXPLM303
)

# Include source code
add_library(simcore
	${CMAKE_CURRENT_SOURCE_DIR}/src/manips.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/simcore/manips.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/persist.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/simcore/persist.h
)

# Include directories for both SimCore & its dependencies
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_include_directories(simcore
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/deps/libacfutils/src
	${CMAKE_CURRENT_SOURCE_DIR}/deps/xpdraw/src
	${CMAKE_CURRENT_SOURCE_DIR}/deps/XPSDK/CHeaders/XPLM
	${CMAKE_CURRENT_SOURCE_DIR}/deps/XPSDK/CHeaders/Widgets
	${CMAKE_CURRENT_SOURCE_DIR}/deps/libacfutils-redist/include
)

if (WIN32)
	set(ARCH "mingw64")
elseif (APPLE)
	set(ARCH "mac64")
else ()
	set(ARCH "lin64")
endif ()

# Adapted from https://discord.com/channels/397379810067742721/529085059324444672/1029105813631029308
set(LACF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/libacfutils-redist/${ARCH}/lib")
set(LACF_LIBS
	"${LACF_ROOT}/libacfutils.a"
	"${LACF_ROOT}/libiconv.a"
	"${LACF_ROOT}/libcairo.a"
	"${LACF_ROOT}/libpixman-1.a"
	"${LACF_ROOT}/libfreetype.a"
	"${LACF_ROOT}/libopusurl.a"
	"${LACF_ROOT}/libopusfile.a"
	"${LACF_ROOT}/libopus.a"
	"${LACF_ROOT}/libogg.a"
	"${LACF_ROOT}/libpcre2-8.a"
	"${LACF_ROOT}/libproj.a"
	"${LACF_ROOT}/libxml2.a"
	"${LACF_ROOT}/libcurl.a"
	"${LACF_ROOT}/libshp.a"
	"${LACF_ROOT}/libcharset.a"
	"${LACF_ROOT}/libpng16.a"
	"${LACF_ROOT}/liblzma.a"
	"${LACF_ROOT}/libz.a"
	"${LACF_ROOT}/libssl.a"
	"${LACF_ROOT}/libcrypto.a"
)

if (WIN32)
	list(APPEND LACF_LIBS "${LACF_ROOT}/libglew32mx.a" crypt32 ws2_32 gdi32 dbghelp psapi winmm)
else ()
	list(APPEND LACF_LIBS "${LACF_ROOT}/libGLEWmx.a" pthread)
endif ()

# OS-specific options
if (WIN32)
	include_directories("deps/libacfutils-redist/mingw64/include")

	# Compiler Definitions
	add_definitions(
		-DIBM
		-D_USE_MATH_DEFINES
	)

	# Libraries to link
	target_link_libraries(simcore
		${LINK_LIBRARIES}
		${CMAKE_CURRENT_SOURCE_DIR}/deps/XPSDK/Libraries/Win/XPLM_64.lib
		${CMAKE_CURRENT_SOURCE_DIR}/deps/XPSDK/Libraries/Win/XPWidgets_64.lib
		${CMAKE_CURRENT_SOURCE_DIR}/deps/libacfutils-redist/win64/lib/libacfutils37.a
		opengl32
		imagehlp
		Psapi
		stdc++
	)

	# Output Directory
	set_target_properties(simcore PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ./build/win
	)
elseif (APPLE)
	include_directories("deps/libacfutils-redist/mac64/include")

	# Compiler Definitions
	add_definitions(-DAPL)

	# Libraries to link
	target_link_libraries(simcore
		${LINK_LIBRARIES}
		${CMAKE_CURRENT_SOURCE_DIR}/deps/XPSDK/Libraries/Mac/XPLM.framework
		${CMAKE_CURRENT_SOURCE_DIR}/deps/XPSDK/Libraries/Mac/XPWidgets.framework
		${LACF_LIBS}
		"-framework CoreFoundation"
	)

	# Output Directory
	set_target_properties(simcore PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ./build/mac
	)
else ()
	include_directories("deps/libacfutils-redist/lin64/include")

	# Compiler Definitions
	add_definitions(-DLIN)
	target_link_libraries(simcore
		${LACF_LIBS}
	)

	# Output Directory
	set_target_properties(simcore PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ./build/lin
	)
endif ()