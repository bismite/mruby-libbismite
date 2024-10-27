
class Bi::Label < Bi::Node
  def anchor_x=(x)
    super x
    self.anchor_reposition
    x
  end
  def anchor_y=(y)
    super y
    self.anchor_reposition
    y
  end
  def set_anchor(x,y)
    super x,y
    self.anchor_reposition
    self
  end
  #
  # Color
  #
  def color
    @_color_
  end
  def tint
    @_tint_
  end
  def background_color
    @_background_color_
  end
  def background_tint
    @_background_tint_
  end
  def set_color(c)
    self._set_color_ c.to_color
  end
  def set_tint(c)
    self._set_tint_ c.to_color
  end
  def set_background_color(c)
    self._set_background_color_ c.to_color
  end
  def set_background_tint(c)
    self._set_background_tint_ c.to_color
  end
  alias :color= :set_color
  alias :tint= :set_tint
  alias :background_color= :set_background_color
  alias :background_tint= :set_background_tint
  def opacity=(o)
    c = self.color.dup
    c.a = (o*0xff).to_i
    self.set_color c
  end
  def background_opacity=(o)
    c = self.background_color.dup
    c.a = (o*0xff).to_i
    self.set_background_color c
  end
end
