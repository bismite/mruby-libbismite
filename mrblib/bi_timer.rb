
class Bi::Timer
  attr_reader :node
end

module Bi::TimerRunner
  attr_reader :timers
  def create_timer(interval, count, &callback)
    self.add_timer Bi::Timer.new(interval, count, &callback)
  end
end
