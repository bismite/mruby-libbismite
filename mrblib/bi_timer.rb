
module Bi::TimerRunner
  def create_timer(interval, count, &callback)
    self.add_timer Bi::Timer.new(interval, count, &callback)
  end
end
