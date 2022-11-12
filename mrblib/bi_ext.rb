
class Bi
  def self.transition_start(t)
    @@bi.transition_start t
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
