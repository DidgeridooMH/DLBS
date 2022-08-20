project('dlbs', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

fmt = dependency('fmt')
json = dependency('nlohmann_json')

deps = [
  fmt,
  json
]

src = [
  'src/main.cpp',
  'src/config/Config.cpp',
  'src/multithread/HotThread.cpp',
  'src/net/tcp/TcpClient.cpp',
  'src/net/tcp/TcpServer.cpp',
  'src/net/filters/HttpFilter.cpp',
  'src/net/endpoint/EndPointConnection.cpp'
]

executable('dlbs',
           src,
           include_directories : ['./src'],
           dependencies : deps,
           cpp_args : ['-DPROJECT_VERSION=@0@'.format(meson.project_version())],
           install : true)

