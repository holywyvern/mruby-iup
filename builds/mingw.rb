# frozen_string_literal: true

# Build an empty ruby build on current target
MRuby::Build.new do |conf|
  conf.toolchain :gcc
end

MRuby::CrossBuild.new('mingw') do |conf|
  toolchain :gcc

  conf.cc.command = 'x86_64-w64-mingw32-gcc'
  conf.cxx.command = 'x86_64-w64-mingw32-g++'
  conf.linker.command = 'x86_64-w64-mingw32-gcc'

  conf.linker.flags += %w[-static -static-libstdc++ -static-libgcc]
  conf.cc.defines += %w[PCRE_STATIC]

  conf.gembox 'default'

  conf.gem File.expand_path(File.join(__dir__, '..'))

  conf.gem github: 'iij/mruby-regexp-pcre'
  conf.gem github: 'mattn/mruby-json'
  conf.gem github: 'iij/mruby-require'

  if ENV['DEBUG'] == 'true'
    conf.enable_debug
    conf.cc.defines = %w[MRB_ENABLE_DEBUG_HOOK]
    conf.gem core: 'mruby-bin-debugger'
  end
end
