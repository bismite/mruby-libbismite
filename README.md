# mruby-libbismite

[libbismite](https://github.com/bismite/libbismite) binding for [mruby](https://github.com/mruby/mruby)

# Changelog
## 7.1.0
- update libbismite 10.1.0
- src/bismite.c: stop run loop when mrb exception occured in main loop.
## 7.0.0
- update libbismite 10.0.0
- Color update
  - add Bi::Color class.
  - Bi::Node: add `color` and `tint` accessor.
  - add `color` accessor to Bi class.
- Postprocess and Framebuffer update
  - add FrameBuffer class and PostProcessLayer class.
  - postprocess related attributes removed from Layer class.
  - TransitionLayer removed.
- Scene graph related update
  - remove root node from layer, and add `add` and `remove` function.
  - Bi::LayerGroup : add `add` and `remove` function. `add_layer`,`remove_layer`,`add_layer_group` and `remove_layer_group` removed.
  - `add_layer`, `remove_layer` and `remove_all_layers` removed from Bi class.
  - add `parent` accessor to LayerGroup and Layer.
- other
  - timer.c : callback receives context.
  - Bi::Node : rename anchor aliases. (top_left,bottom_left,...)
  - overhaul Bi::Label class.
## 6.0.0 - 2023/04/10
- update libbismite 8.0.2
- remove Repeat action.
- add actions (FadeOut,FadeIn,ScaleTo,ScaleBy,MoveBy)
## 5.0.0 - 2023/03/23
- license changed : MIT License
## 4.1.0 - 2022/11/12
- add autoremove flag to actions
## 4.0.1 - 2022/11/7
- fix transition callback
## 4.0.0 - 2022/11/7
- add w/h getter to canvas
- update BiTimer callback
## 3.0.0 - 2022/11/2
- update libbismite 5.0.0
## 2.0.0 - 2022/05/10
- update libbismite 4.0.0
- TextureMapping removed.
- Cropped Texture supported.
## 1.4.0 - 2022/04/26
- add flip_vertical and flip_horizontal to TextureMapping
## 1.3.0 - 2022/04/15
- add line_x_to_index to Bi::Font
- add set_text_color_with_range to Bi::Label
- Changed alpha value of set_color function to optional. (canvas,label,node)
## 1.2.1 - 2022/02/11
- add x,y accsessors Bi::TextureMapping
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
Copyright 2018-2023 kbys <work4kbys@gmail.com>

MIT License
