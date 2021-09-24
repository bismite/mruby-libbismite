
class Bi
  def self.transition_start(t)
    @@bi.transition_start t
  end
end

class Bi::Node
  attr_reader :actions
  def add_action(action)
    @actions ||= []
    @actions << action
    self._add_action action
    action
  end
  def remove_action(action)
    @actions ||= []
    @actions.delete action
    self._remove_action action
    action
  end
  def remove_all_actions
    if @actions
      @actions.each{|a|
        self._remove_action a
      }
      @actions.clear
    end
  end
end

class Bi::Action::Base
  attr_accessor :callback
end

class Bi::Font
  attr_reader :texture
end

class Bi::Label
  attr_reader :text
  def text=(text)
    self.set_text text
  end
  alias :set_background_color :set_color
  alias :set_color :set_text_color
end
