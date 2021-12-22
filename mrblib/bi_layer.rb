
class Bi::LayerGroup
  include Bi::TimerRunner
  def layers
    @layers
  end
end

class Bi::Layer
  include Bi::TimerRunner
  attr_reader :shader
  attr_reader :post_process_shader
end
