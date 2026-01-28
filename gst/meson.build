# Path: gst/meson.build

plugin_sources = [
  #'ed137ptt/gstrtped137ptt.c'
  'ed137pay/gsted137pay.c'
]

plugins_install_dir = gst_dep.get_variable(pkgconfig: 'pluginsdir')

shared_library('gsted137pay',
  plugin_sources,
  dependencies : plugin_deps,
  gnu_symbol_visibility : 'default',
  install : true,
  install_dir : plugins_install_dir,
)