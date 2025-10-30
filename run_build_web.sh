#!/bin/bash
cd build_web
emcc ../src/main.c -o index.html \
  -I../include \
  -I../build/external/raylib-master/src \
  ../build/external/raylib-master/build_web/raylib/libraylib.a \
  --preload-file ../resources@resources \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s TOTAL_MEMORY=256MB \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s USE_WEBGL2=1 \
  -s NO_EXIT_RUNTIME=1 \
  -s ASSERTIONS=0 \
  -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
  -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1