
class Bi::Color
  def self.rgba32(rgba)
    r = rgba >> 24 & 0xff
    g = rgba >> 16 & 0xff
    b = rgba >>  8 & 0xff
    a = rgba & 0xff
    Bi::Color.new r,g,b,a
  end
end
