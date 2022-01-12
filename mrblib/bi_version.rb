
module Bi::Version
  def self.libbismite
    "#{BISMITE_MAJOR}.#{BISMITE_MINOR}.#{BISMITE_PATCH}"
  end

  def self.mruby_bicore
    "1.1.1"
  end

  def self.emscripten
    EMSCRIPTEN_MAJOR ? "#{EMSCRIPTEN_MAJOR}.#{EMSCRIPTEN_MINOR}.#{EMSCRIPTEN_PATCH}" : nil
  end

  def self.clang
    CLANG_MAJOR ? "#{CLANG_MAJOR}.#{CLANG_MINOR}.#{CLANG_PATCH}" : nil
  end

  def self.gnuc
    GNUC_MAJOR ? "#{GNUC_MAJOR}.#{GNUC_MINOR}.#{GNUC_PATCH}" : nil
  end

  def self.sdl_compiled
    "#{SDL_COMPILED_MAJOR}.#{SDL_COMPILED_MINOR}.#{SDL_COMPILED_PATCH}"
  end

  def self.sdl
    "#{SDL_LINKED_MAJOR}.#{SDL_LINKED_MINOR}.#{SDL_LINKED_PATCH}"
  end
end
