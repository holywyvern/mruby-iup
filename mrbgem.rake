MRuby::Gem::Specification.new('mruby-iup') do |spec|
  spec.bins = ['gui']
  spec.license = 'Apache-2.0'
  spec.authors = 'Ramiro Rojo'

  spec.cc.include_paths  << File.join(__dir__, 'vendor', 'iup', 'include')
  spec.cxx.include_paths << File.join(__dir__, 'vendor', 'iup', 'include')

  spec.build.cc.include_paths  << File.join(__dir__, 'vendor', 'iup', 'include')
  spec.build.cxx.include_paths << File.join(__dir__, 'vendor', 'iup', 'include')

  spec.build.linker.library_paths << File.join(__dir__, 'vendor', 'iup', 'win64', 'lib')
  spec.build.linker.libraries += %w[
    iup_mglplot iup_plot iup_scintilla iup iupcd iupcontrols iupgl
    iupglcontrols iupim iupimglib iupole iuptuio wsock32 ws2_32
  ]
end
