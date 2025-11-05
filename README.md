# SimCore
SimCore is an internal solution with common functions across SimSolution's X-Plane projects.

It also includes the X-Plane SDK, [xpdraw](https://github.com/slimit75/xpdraw), and [libacfutils](https://codeberg.org/skiselkov/libacfutils), so they don't have to be included or linked separately.

The project currently consists of the following:
* [manips.h](src/simcore/manips.h), formerly the open-source **libswitch**, which handles callbacks for switch manipulators and animation datarefs.
* [persist.h](src/simcore/persist.h), a continuation of the persistence system first introduced in the DA40.

In the future, the project plans to include:
* [gtx327.h](src/simcore/gtx327.h), a simulation of the Garmin GTX327 transponder for use in the DA20 & Alarus.