
alias :_original_rand_ :rand
def rand(n)
  if n.is_a? Range
    return n.min if n.min == n.max
    n.min + _original_rand_(n.max-n.min)
  else
    _original_rand_(n)
  end
end

class Bi
  @@bi = nil
  attr_accessor :timers, :layers
  def self.init(w,h,fps,title)
    unless @bi
      @@bi = Bi.new(w,h,fps,title)
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
  def self.fps
    @@bi.fps
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
  def self.title=(title)
    @@bi.set_title title
  end
  def self.add_layer(layer)
    @@bi.layers << layer
    @@bi.add_layer layer
    layer
  end
  def self.remove_layer(layer)
    @@bi.layers.delete layer
    @@bi.remove_layer layer
  end
  def self.add_timer(timer)
    @@bi.timers << timer
    @@bi.add_timer timer
    timer
  end
  def self.remove_timer(timer)
    @@bi.timers.delete timer
    @@bi.remove_timer timer
  end
  def root=(node)
    @root = node
    set_root_node node
  end
end

class Bi::Texture
  attr_reader :texture_image
end

class Bi::Node
  attr_reader :texture
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
  def texture=(texture)
    @texture = texture
    self.set_texture @texture
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
  def add_timer(interval,repeat,&block)
    timer = Bi::Timer.new(self,interval,repeat,&block)
    Bi.add_timer( timer )
  end
  def remove_timer(timer)
    Bi.remove_timer timer
  end
  def on_update(callback=nil,&blk)
    self._on_update_( callback || blk )
  end
end

class Bi::Sprite < Bi::Node
  def initialize(texture)
    super
    self.texture = texture
    self.set_bound 0, 0, texture.w, texture.h
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
