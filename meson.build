project('gst-ed137', 'c',
  version : '0.1.0',
  meson_version : '>= 0.54.0',
  default_options : [ 'warning_level=1',
                      'buildtype=debugoptimized' ])


core_conf = configuration_data()
core_conf.set('PACKAGE', '"@0@"'.format(meson.project_name()))
core_conf.set('VERSION', '"@0@"'.format(meson.project_version()))
configure_file(output : 'config.h', configuration : core_conf)
configinc = include_directories('.')

common_args = ['-DHAVE_CONFIG_H']

gst_req = '>= 1.20.0'

# dependencies
gst_dep = dependency('gstreamer-1.0', version : gst_req, fallback : ['gstreamer', 'gst_dep'])
gst_base_dep = dependency('gstreamer-base-1.0')

# RTP Library (Fixes the missing header error)
gst_rtp_dep = dependency('gstreamer-rtp-1.0', version : gst_req, fallback : ['gst-plugins-base', 'rtp_dep'])

# Audio Library (Optional, but good for ATM projects)
gst_audio_dep = dependency('gstreamer-audio-1.0', version : gst_req, fallback : ['gst-plugins-base', 'audio_dep'])


# Set the directory where plugins should be installed.
fs = import('fs')
if fs.is_samepath(get_option('prefix'), '~')
  plugins_install_dir = '@0@/.local/share/gstreamer-1.0/plugins'.format(get_option('prefix'))
else
  plugins_install_dir = '@0@/gstreamer-1.0'.format(get_option('libdir'))
endif

plugin_deps = [gst_dep, gst_rtp_dep, gst_audio_dep, gst_base_dep]

subdir('gst')