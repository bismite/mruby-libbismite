
class Bi::LayerGroup
  include Bi::TimerRunner
  attr_reader :framebuffer
end

class Bi::Layer
  include Bi::TimerRunner
  attr_reader :shader
end
