project('dlbs', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

fmt = dependency('fmt')

deps = [
  fmt
]

src = [
  'src/main.cpp',
  'src/multithread/HotThread.cpp',
  'src/net/TcpClient.cpp',
  'src/net/TcpServer.cpp'
]

executable('dlbs',
           src,
           include_directories : ['./src'],
           dependencies : deps,
           cpp_args : ['-DPROJECT_VERSION=@0@'.format(meson.project_version())],
           install : true)

