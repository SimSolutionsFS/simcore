# simcore
Simcore is a C library that includes various common functions that are used across SimSolutions's X-Plane projects.

It also includes the [X-Plane SDK](https://developer.x-plane.com/sdk/plugin-sdk-downloads/) and [libacfutils](https://codeberg.org/skiselkov/libacfutils) redistributable, so they don't have to be included or linked separately.

> [!IMPORTANT]
> To be able to compile & use Simcore, you must provide your own copy of the libacfutils redistributable at `/deps/libacfutils-redist`. 

The project currently consists of the following:
* `simcore/`
  * [draw_tools.h](src/simcore/draw_tools.h), helper functions to be used with XPLM440's panel graphics API
  * [manips.h](src/simcore/manips.h), handler for switch manipulators and animation datarefs.
  * [persist.h](src/simcore/persist.h), a continuation of the persistence system first introduced in the DA40.
  * [tools.h](src/simcore/tools.h), misc helper functions
  * [win.h](src/simcore/win.h), support for X-Plane window creation using the panel graphics API for rendering
* `xpdraw/` [DEPRECIATED]
  * [xpdraw.h](src/xpdraw/xpdraw.h), simplified OpenGL functions
  * [fonts.h](src/xpdraw/fonts.h), support for text rendering in xpdraw
  * [windows.h](src/xpdraw/windows.h), support for X-Plane window creation in xpdraw

## Other Open Source Licenses
This project uses or includes the following open source projects:
* [skiselkov/libacfutils](https://codeberg.org/skiselkov/libacfutils), available under the [CDDL 1.0](https://spdx.org/licenses/CDDL-1.0.html) license.
* [nothings/stb](https://github.com/nothings/stb), available both under the [MIT](https://spdx.org/licenses/MIT.html) license.

While not necessarily open-source, the following are included in the project and are made available under their own licenses:
* The X-Plane SDK (using [this mirror](https://github.com/slimit75/XPSDK)) available under [this license](https://github.com/slimit75/XPSDK/blob/main/license.txt).

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