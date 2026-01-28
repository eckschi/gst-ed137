Eurocae ED-137 Encoder Plugin

NOTE:
plugins can be installed locally by using "$HOME" as prefix:

  $ meson --prefix="$HOME" build/
  $ ninja -C build/ install

However be advised that the automatic scan of plugins in the user home
directory won't work under gst-build devenv.

example launch

gst-launch-1.0 audiotestsrc num-buffers=1 ! alawenc ! ed137pay ! fakesink dump=true