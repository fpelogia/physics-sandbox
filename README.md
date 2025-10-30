# Physics Sandbox

## Instructions
### Linux
* CD into the build folder
* run `./premake5 gmake`
* CD back to the root
* run `make`

### MacOS Users
* CD into the build folder
* run `./premake5.osx gmake`
* CD back to the root
* run `make`

### Build for Web
* make sure you have emscripten
* compile raylib to get the lib file:
  `cd build/external/raylib-master`
  `mkdir -p build_web`
  `cd build_web`
  `emcmake cmake .. -DPLATFORM=Web -DBUILD_SHARED_LIBS=OFF`
  `emmake make`
* run `./run_build_web.sh`

## Output files
The built code will be in the bin dir

This project was created from the Raylib-Quickstart Template, by Jeffery Myers.

## License
Copyright (c) 2025 Frederico José Ribeiro Pelogia
GPLv3 - Details in LICENSE File

## License (RayLib + Quickstart Template)
Copyright (c) 2020-2025 Jeffery Myers

This software is provided "as-is", without any express or implied warranty. In no event 
will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not claim that you 
  wrote the original software. If you use this software in a product, an acknowledgment 
  in the product documentation would be appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be misrepresented
  as being the original software.

  3. This notice may not be removed or altered from any source distribution.
