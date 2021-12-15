
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
    unless @@bi.layers.layers.include? layer
      @@bi.layers.add_layer layer
    end
    layer
  end
  def self.remove_layer(layer)
    @@bi.layers.remove_layer layer
  end
  def self.remove_all_layers
    @@bi.layers.layers.each{|l| @@bi.remove_layer l }
    @@bi.layers.clear
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

class Bi::LayerGroup
  include Bi::TimerRunner
  def layers
    @layers
  end
end

class Bi::Layer
  include Bi::TimerRunner
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
  include Bi::TimerRunner
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
  def remove_from_parent
    self.parent&.remove self
  end

  #
  # callbacks
  #
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
end

class Bi::Sprite < Bi::Node
  def initialize(texture_mapping)
    super
    self.texture_mapping = texture_mapping
    self.set_position 0, 0
    self.set_size texture_mapping.w, texture_mapping.h
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
