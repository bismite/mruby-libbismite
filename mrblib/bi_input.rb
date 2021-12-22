
module Bi::KeyCode
  def self.name(code)
    Bi::KeyCode.constants.each{|c|
      return c if Bi::KeyCode.const_get(c) == code
    }
    return nil
  end
end

module Bi::ScanCode
  def self.name(code)
    Bi::ScanCode.constants.each{|c|
      return c if Bi::ScanCode.const_get(c) == code
    }
    return nil
  end
end

module Bi::KeyMod
  def self.codes
    Bi::KeyMod.constants
  end
  def self.parse(code)
    Bi::KeyMod.constants.map{|k|
      c = Bi::KeyMod.const_get(k)
      (c!=0 and code&c==c) ? c : nil
    }.compact
  end
  def self.name(code)
    Bi::KeyMod.constants.each{|c|
      return c if Bi::KeyMod.const_get(c) == code
    }
    return nil
  end
end
