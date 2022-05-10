
class Bi::Node
  include Bi::TimerRunner
  attr_reader :texture
  attr_reader :parent

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
