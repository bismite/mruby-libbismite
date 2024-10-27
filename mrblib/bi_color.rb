
class Bi::Color
  def self.rgba32(rgba)
    r = rgba >> 24 & 0xff
    g = rgba >> 16 & 0xff
    b = rgba >>  8 & 0xff
    a = rgba & 0xff
    Bi::Color.new r,g,b,a
  end
  def self.rgba(x) = Bi::Color::rgba32(x)
  def self.rgb(x) = Bi::Color::rgba32(x << 8 | 0xff)
  def dup()
    Bi::Color.new r,g,b,a
  end
  def to_s
    "#%02X" % r + "%02X" % g + "%02X" % b + "%02X" % a
  end
  # named colors
  def self.transparent = rgba(0)
  def self.white = rgb 0xffffff
  def self.black = rgb 0
  def self.cyan = rgb 0x00ffff
  def self.magenta = rgb 0xff00ff
  def self.yellow = rgb 0xffff00
  def self.red = rgb 0xff0000
  def self.green = rgb 0x00ff00
  def self.blue = rgb 0x0000ff
  def self.silver = rgb 0xc0c0c0
  def self.gray = rgb 0x808080
end
