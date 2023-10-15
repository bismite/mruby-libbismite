
class Bi::Node
  include Bi::TimerRunner
  attr_reader :texture
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

  def anchor=(anchor)
    raise "unknown anchor alias #{anchor}" unless ANCHOR_ALIAS[anchor]
    self.anchor_x, self.anchor_y = ANCHOR_ALIAS[anchor]
  end

  def remove_from_parent
    self.parent&.remove self
  end

  def scale=(s)
    self.set_scale s,s
  end

  def set_color(r,g,b,a=nil)
    self.color.r = r
    self.color.g = g
    self.color.b = b
    self.color.a = a if a
    self.color
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
