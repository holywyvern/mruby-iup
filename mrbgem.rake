MRuby::Gem::Specification.new('mruby-iup') do |spec|
  spec.bins = ['gui']
  spec.license = 'Apache-2.0'
  spec.authors = 'Ramiro Rojo'

  spec.cc.include_paths  << File.join(__dir__, 'vendor', 'iup', 'include')
  spec.cxx.include_paths << File.join(__dir__, 'vendor', 'iup', 'include')

  spec.build.cc.include_paths  << File.join(__dir__, 'vendor', 'iup', 'include')
  spec.build.cxx.include_paths << File.join(__dir__, 'vendor', 'iup', 'include')

  spec.build.linker.library_paths << File.join(__dir__, 'vendor', 'iup', 'win64')
  spec.build.linker.libraries += %w[
    iup_mglplot iup_plot iup_scintilla iupcd iupcontrols iupgl
    iupglcontrols iupim iupimglib iupole iuptuio iup

    im_avi im_fftw3 im_jp2 im_process_omp im_process im

    ftgl freetype6 z

    stdc++

    wsock32 ws2_32 user32 kernel32 opengl32 ole32 gdi32
    comctl32 msimg32 winmm comdlg32 oleaut32 imm32 shell32
    uuid
  ]
end
