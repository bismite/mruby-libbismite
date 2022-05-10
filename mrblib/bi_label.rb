
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
end
