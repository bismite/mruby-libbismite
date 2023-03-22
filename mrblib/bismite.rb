
class Bi
  @@bi = nil
  attr_accessor :title
  attr_accessor :timers, :layers, :default_shader

  def self.init(w,h,opts={})
    unless @@bi
      fps = opts[:fps] || 0
      title = opts[:title] || ""
      highdpi = opts[:highdpi] || false
      @@bi = Bi.new(w,h,fps,highdpi,title)
      @@bi.title = title
      @@bi.timers = []
    end
    @@bi
  end
  def self.w
    @@bi.w
  end
  def self.h
    @@bi.h
  end
  def self.now
    @@bi.now
  end
  def self.start_run_loop
    @@bi.start_run_loop
  end
  def self.debug=(debug)
    @@bi.debug = debug
  end
  def self.debug
    @@bi.debug
  end
  def self.title
    @@bi.title
  end
  def self.title=(title)
    @@bi.title = title
    @@bi.set_title title
  end

  # Layer
  def self.layers
    @@bi.layers
  end
  def self.add_layer(layer)
    unless @@bi.layers.layers.include? layer
      @@bi.layers.add_layer layer
    end
    layer
  end
  def self.remove_layer(layer)
    @@bi.layers.remove_layer layer
  end
  def self.remove_all_layers
    @@bi.layers.layers.each{|l| @@bi.remove_layer l }
    @@bi.layers.clear
  end

  # shader
  def self.default_shader
    @@bi.default_shader
  end

  #
  def self.messagebox(title,message,dialog_type=:information)
    @@bi.messagebox title, message,dialog_type
  end
  def self.profile
    @@bi.profile
  end
  def profile
    @profile ||= Bi::Profile.new self
  end
end
