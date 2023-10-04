
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
end
