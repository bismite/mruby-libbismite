
class String
  def to_color
    tmp = self
    if tmp.start_with?("0x")
      tmp = tmp.delete_prefix "0x"
    elsif tmp.start_with?("#")
      tmp = tmp.delete_prefix "#"
    end
    if tmp.length == 6 # RRGGBB
      return Bi::Color.rgb(tmp.to_i(16))
    elsif tmp.length == 8 # RRGGBBAA
      return Bi::Color.rgba(tmp.to_i(16))
    end
    return nil
  end
end
