
class Bi::Framebuffer
  def clear(color=nil)
    color = Bi::Color.transparent unless color
    self._clear_ color.r, color.g, color.b, color.a
  end
  def textures = @textures
end
