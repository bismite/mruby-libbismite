# mruby-libbismite

for [libbismite 2.0.2](https://github.com/bismite/libbismite/releases/tag/2.0.2)

# Changelog

## 1.1.1 - 2022/01/13
- remove unused emscripten code
## 1.1.0 - 2021/12/22
- for libbismite 1.2
- BiCanvas: add blend factor setter/getter.
- Blend factor constants moved to under the kernel module.
- BiTexture receives straight-alpha flag.
## 1.0.2 - 2021/12/18
- for libbismite 1.1.8
  - remove on_update from Node
  - add Canvas class
  - on_update callback removed
  - add opacity to node
- node angle getter/setter use radian
## 0.14.2
- modifications to the rand function have been moved to [mrubi-bi-misc](https://github.com/bismite/mruby-bi-misc).
## 0.14.1
- small fix for src/layer.c.
## 0.14.0
- post-process class has been removed.
- updated initializers for textures and texture mapping.
- layer class can act as a post-process.

# License

Copyright 2018-2022 kbys <work4kbys@gmail.com>

Apache License Version 2.0
