
class Bi::LayerGroup
  include Bi::TimerRunner
  attr_reader :framebuffer
  attr_reader :parent
end

class Bi::Layer
  include Bi::TimerRunner
  attr_reader :shader
  attr_reader :parent
  def remove_from_parent
    self.parent&.remove self
  end
end
