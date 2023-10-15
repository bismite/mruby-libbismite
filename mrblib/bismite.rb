
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
  def self.w = @@bi.w
  def self.h = @@bi.h

  def self.color = @@bi.color
  def self.color=(color)
    @@bi.color=color
  end

  def self.now = @@bi.now
  def self.start_run_loop = @@bi.start_run_loop

  def self.debug=(debug)
    @@bi.debug = debug
  end
  def self.debug = @@bi.debug
  def self.title = @@bi.title
  def self.title=(title)
    @@bi.title = title
  end

  # Layer
  def self.layers = @@bi.layers
  def self.add_layer(layer) = @@bi.layers.add_layer layer
  def self.remove_layer(layer) = @@bi.layers.remove_layer layer
  def self.remove_all_layers
    @@bi.layers.layers.each{|l| @@bi.remove_layer l }
    @@bi.layers.clear
  end

  # shader
  def self.default_shader = @@bi.default_shader

  #
  def self.messagebox(title,message,dialog_type=:information) = @@bi.messagebox(title,message,dialog_type)
  def self.profile = @@bi.profile
  def profile
    @profile ||= Bi::Profile.new self
  end
end
