
module Bi::NodeBase
  include Bi::TimerRunner
  attr_reader :parent

  #  TopLeft     Top     TopRight
  #   Left      Center    Right
  # BottomLeft  Bottom  BottomRight
  ANCHOR_ALIAS = {
    center:      [0.5, 0.5],
    top:         [0.5, 1.0],
    top_left:    [0.0, 1.0],
    left:        [0.0, 0.5],
    bottom_left: [0.0, 0.0],
    bottom:      [0.5, 0.0],
    bottom_right:[1.0, 0.0],
    right:       [1.0, 0.5],
    top_right:   [1.0, 1.0]
  }

  #
  # Scene Graph
  #
  def add(node,x=nil,y=nil,z=nil)
    return nil unless node.is_a?(Bi::Node) || node.is_a?(Bi::ShaderNode)
    if x==:center
      node.x = (self.w - node.w*node.scale_x)/2 + node.w*node.scale_x*node.anchor_x
    elsif x
      node.x = x.to_i
    end
    if y==:center
      node.y = (self.h - node.h*node.scale_y)/2 + node.h*node.scale_y*node.anchor_y
    elsif y
      node.y = y.to_i
    end
    if z
      node.z = z.to_i
    end
    self._add_node_ node
  end
  def remove_from_parent
    self.parent&.remove self
  end
  def remove_all_children
    @_children_.dup&.each{|c| c.remove_from_parent }
  end
end


class Bi::Node
  include Bi::NodeBase
  attr_reader :texture

  def self.xywh(x,y,w,h)
    n = Bi::Node.new
    n.set_size w,h
    n.set_position x,y
    n
  end

  def self.rect(w,h)
    n = Bi::Node.new
    n.set_size w,h
    n
  end

  #
  # Texture and Color
  #
  def texture = @texture
  def color = @_color_
  def tint =  @_tint_
  def set_color(c) = self._set_color_(c.to_color)
  def set_tint(c) = self._set_tint_(c.to_color)
  alias :color= :set_color
  alias :tint= :set_tint
  def opacity=(o)
    self.color.a = (o*0xff).to_i
  end

  #
  # Scene Graph
  #
  alias :remove :_remove_node_
  def anchor=(anchor)
    raise "unknown anchor alias #{anchor}" unless ANCHOR_ALIAS[anchor]
    self.anchor_x, self.anchor_y = ANCHOR_ALIAS[anchor]
  end
  def scale=(s)
    self.set_scale s,s
  end

  #
  # action
  #
  def actions = @_actions

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

class Bi::ShaderNode
  include Bi::NodeBase
  attr_reader :shader
  alias :remove :_remove_node_
  def w
    @parent.w
  end
  def h
    @parent.h
  end
  def textures = @textures
end
