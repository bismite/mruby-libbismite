
class Bi
  @@bi = nil
  attr_reader :title
  attr_accessor :timers
  attr_reader :default_framebuffer
  attr_reader :default_framebuffer_node
  attr_reader :default_shader

  def self.init(w,h,opts={})
    unless @@bi
      fps = opts[:fps] || 0
      title = opts[:title] || ""
      flags = 0
      flags = BI_WINDOW_ALLOW_HIGHDPI if opts[:highdpi]
      @@bi = Bi.new(w,h,fps,flags,title)
      @@bi.title = title
      @@bi.timers = []
    end
    @@bi
  end
  def self.w = @@bi.w
  def self.h = @@bi.h

  def self.now = @@bi.now
  def self.start_run_loop = @@bi.start_run_loop

  def self.title = @@bi.title
  def self.title=(title)
    @@bi.title = title
  end

  def self.draw_framebuffer_node(node)
    @@bi.draw_framebuffer_node node
  end

  #
  def self.add(node)
    @@bi.default_framebuffer_node.add node
  end
  def self.remove(node)
    @@bi.default_framebuffer_node.remove node
  end

  # Default Framebuffer
  def self.default_framebuffer_node = @@bi.default_framebuffer_node
  def self.default_framebuffer = @@bi.default_framebuffer

  # shader
  def self.default_shader = @@bi.default_shader

  #
  def self.messagebox(title,message,dialog_type=:information) = @@bi.messagebox(title,message,dialog_type)
  def self.profile = @@bi.profile
  def profile
    @profile ||= Bi::Profile.new self
  end
end
