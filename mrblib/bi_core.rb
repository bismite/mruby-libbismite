
alias :_original_rand_ :rand
def rand(n=nil)
  if n.is_a? Range
    return n.min if n.min == n.max
    n.min + _original_rand_(1+n.max-n.min)
  else
    _original_rand_(n)
  end
end

class Bi
  @@bi = nil
  attr_accessor :title
  attr_accessor :timers, :layers

  def self.init(w,h,opts={})
    unless @@bi
      fps = opts[:fps] || 0
      title = opts[:title] || ""
      highdpi = opts[:highdpi] || false
      @@bi = Bi.new(w,h,fps,highdpi,title)
      @@bi.title = title
      @@bi.layers = []
      @@bi.timers = []
    end
    @@bi
  end
  def self.w
    @@bi.w
  end
  def self.h
    @@bi.h
  end
  def self.now
    @@bi.now
  end
  def self.start_run_loop
    @@bi.start_run_loop
  end
  def self.debug=(debug)
    @@bi.debug = debug
  end
  def self.debug
    @@bi.debug
  end
  def self.title
    @@bi.title
  end
  def self.title=(title)
    @@bi.title = title
    @@bi.set_title title
  end

  # Layer
  def self.layers
    @@bi.layers
  end
  def self.add_layer(layer)
    unless @@bi.layers.include? layer
      @@bi.layers << layer
      @@bi.add_layer layer
    end
    layer
  end
  def self.remove_layer(layer)
    if @@bi.layers.delete layer
      @@bi.remove_layer layer
    end
  end
  def self.remove_all_layers
    @@bi.layers.each{|l| @@bi.remove_layer l }
    @@bi.layers.clear
  end

  # shader
  def self.shader
    @@bi.shader
  end
  def self.shader=(shader)
    @@bi.shader=shader
  end
  def self.set_optional_shader_attributes(index,value)
    @@bi.set_optional_shader_attributes(index,value)
  end

  # Timer
  def self.add_timer(duration,repeat,&callback)
    @@bi.timers ||= []
    timer = Bi::Timer.new @@bi, duration, repeat, &callback
    @@bi.timers << timer
    @@bi._add_timer timer
    timer
  end
  def self.remove_timer(timer)
    @@bi.timers ||= []
    @@bi.timers.delete timer
    @@bi._remove_timer timer
    timer
  end

  def self.messagebox(title,message,dialog_type=:information)
    @@bi.messagebox title, message,dialog_type
  end
  def self.profile
    @@bi.profile
  end
  def profile
    @profile ||= Bi::Profile.new self
  end
end

class Bi::Texture
  def to_sprite(x=nil,y=nil,w=nil,h=nil)
    if x and y and w and h
      Bi::Sprite.new Bi::TextureMapping.new(self,x,y,w,h)
    else
      Bi::Sprite.new Bi::TextureMapping.new(self,0,0,self.w,self.h)
    end
  end
end

class Bi::TextureMapping
  attr_reader :texture
  def to_sprite
    Bi::Sprite.new self
  end
end

class Bi::Node
  attr_reader :texture_mapping
  attr_accessor :parent

  # NW   N  NE
  #  W   C   E
  # SW   S  SE
  ANCHOR_ALIAS = {
    center:     [0.5, 0.5],
    north:      [0.5, 1.0],
    north_west: [0.0, 1.0],
    west:       [0.0, 0.5],
    south_west: [0.0, 0.0],
    south:      [0.5, 0.0],
    south_east: [1.0, 0.0],
    east:       [1.0, 0.5],
    north_east: [1.0, 1.0]
  }

  def anchor=(anchor)
    self.anchor_x, self.anchor_y = ANCHOR_ALIAS[anchor]
  end
  def texture_mapping=(texture_mapping)
    @texture_mapping = texture_mapping
    self.set_texture_mapping @texture_mapping
  end
  def add(node)
    @children ||= []
    @children << node
    node.parent = self
    self.add_child(node)
  end
  def remove(node)
    @children.delete node
    node.parent = nil
    self.remove_child node
  end
  def remove_from_parent
    self.parent&.remove self
  end

  #
  # callbacks
  #
  def on_update(callback=nil,&blk)
    self._on_update_( callback || blk )
  end
  def on_click(callback=nil,&blk)
    self._on_click_( callback || blk )
  end
  def on_move_cursor(callback=nil,&blk)
    self._on_move_cursor_( callback || blk )
  end
  def on_key_input(callback=nil,&blk)
    self._on_key_input_( callback || blk )
  end
  def on_move_finger(callback=nil,&blk)
    self._on_move_finger_( callback || blk )
  end
  def on_touch(callback=nil,&blk)
    self._on_touch_( callback || blk )
  end
  def on_text_input(callback=nil,&blk)
    self._on_text_input_( callback || blk )
  end

  # Timer
  def add_timer(duration,repeat,&callback)
    @timers ||= []
    timer = Bi::Timer.new self, duration, repeat, &callback
    @timers << timer
    self._add_timer timer
    timer
  end
  def remove_timer(timer)
    @timers ||= []
    @timers.delete timer
    self._remove_timer timer
    timer
  end
end

class Bi::Timer
  attr_reader :callback
  attr_reader :owner
end

class Bi::Sprite < Bi::Node
  def initialize(texture_mapping)
    super
    self.texture_mapping = texture_mapping
    self.set_position 0, 0
    self.set_size texture_mapping.w, texture_mapping.h
    self.set_color 0xff,0xff,0xff,0xff
  end
end

module Bi::KeyCode
  def self.name(code)
    Bi::KeyCode.constants.each{|c|
      return c if Bi::KeyCode.const_get(c) == code
    }
    return nil
  end
end

module Bi::ScanCode
  def self.name(code)
    Bi::ScanCode.constants.each{|c|
      return c if Bi::ScanCode.const_get(c) == code
    }
    return nil
  end
end

module Bi::Version
  def self.bicore
    "#{BI_CORE_MAJOR}.#{BI_CORE_MINOR}.#{BI_CORE_PATCH}"
  end

  def self.mruby_bicore
    "0.10.0"
  end

  def self.emscripten
    EMSCRIPTEN_MAJOR ? "#{EMSCRIPTEN_MAJOR}.#{EMSCRIPTEN_MINOR}.#{EMSCRIPTEN_PATCH}" : nil
  end

  def self.clang
    CLANG_MAJOR ? "#{CLANG_MAJOR}.#{CLANG_MINOR}.#{CLANG_PATCH}" : nil
  end

  def self.gnuc
    GNUC_MAJOR ? "#{GNUC_MAJOR}.#{GNUC_MINOR}.#{GNUC_PATCH}" : nil
  end

  def self.sdl_compiled
    "#{SDL_COMPILED_MAJOR}.#{SDL_COMPILED_MINOR}.#{SDL_COMPILED_PATCH}"
  end

  def self.sdl
    "#{SDL_LINKED_MAJOR}.#{SDL_LINKED_MINOR}.#{SDL_LINKED_PATCH}"
  end
end
