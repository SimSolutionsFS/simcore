# SimCore
SimCore is a C library that includes various common functions that are used across SimSolutions's X-Plane projects.

It also includes the [X-Plane SDK](https://developer.x-plane.com/sdk/plugin-sdk-downloads/) and [libacfutils](https://codeberg.org/skiselkov/libacfutils) redistributable, so they don't have to be included or linked separately.

> [!IMPORTANT]
> To be able to compile & use SimCore, you must provide your own copy of the libacfutils redistributable at `/deps/libacfutils-redist`. 

The project currently consists of the following:
* [simcore/manips.h](src/simcore/manips.h), handler for switch manipulators and animation datarefs.
* [simcore/persist.h](src/simcore/persist.h), a continuation of the persistence system first introduced in the DA40.
* [xpdraw/xpdraw.h](src/xpdraw/xpdraw.h), simplified OpenGL functions
  * [xpdraw/fonts.h](src/xpdraw/fonts.h), support for text rendering in xpdraw
  * [xpdraw/tools.h](src/xpdraw/tools.h), misc helper functions
  * [xpdraw/windows.h](src/xpdraw/windows.h), support for X-Plane window creation in xpdraw

## Open Source Licenses
This project uses or includes the following open source projects:
* [skiselkov/libacfutils](https://codeberg.org/skiselkov/libacfutils), available under the [CDDL 1.0](https://spdx.org/licenses/CDDL-1.0.html) license.
* [nothings/stb](https://github.com/nothings/stb), available both under the [MIT](https://spdx.org/licenses/MIT.html) license.

While not necessarily open-source, the following are included in the project and are made available under their own licenses:
* The X-Plane SDK (using [this mirror](https://github.com/slimit75/XPSDK)) available under a [custom license](https://github.com/slimit75/XPSDK/blob/main/license.txt).

## Usage
To include SimCore, the X-Plane SDK, and the libacfutils redistributable in your project, you'll need to include it in your project's CMakeLists.txt:
```cmake
add_subdirectory("simcore")
add_dependencies(MyProject simcore)

# If you're using xpdraw/fonts.h, you may need to include FreeType separately:
find_package(Freetype REQUIRED)
target_include_directories(MyProject
        PRIVATE
        ${FREETYPE_INCLUDE_DIRS}
)

# Also, don't forget this:
target_link_libraries(MyProject simcore)
```