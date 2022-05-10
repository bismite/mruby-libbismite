
class Bi::Texture
  def to_sprite(x=nil,y=nil,w=nil,h=nil,cx=nil,cy=nil,ow=nil,oh=nil)
    n = Bi::Node.new
    if x and y and w and h and cx and cy and ow and oh
      n.set_texture self, x,y,w,h, cx,cy,ow,oh
      n.set_size ow,oh
    elsif x and y and w and h
      n.set_texture self, x,y,w,h
      n.set_size w,h
    else
      n.set_texture self, 0,0,self.w,self.h
      n.set_size self.w,self.h
    end
  end
end
