
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
