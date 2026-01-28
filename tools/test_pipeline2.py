import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GLib
import os

# 1. Make sure GStreamer can find your compiled .so file
# Point this to the folder containing libgsted137pay.so
os.environ["GST_PLUGIN_PATH"] = os.getcwd() + "/build"

Gst.init(None)

# 2. Build the pipeline
# audiotestsrc ! alawenc ! ed137pay ! udpsink
pipeline = Gst.parse_launch(
    "audiotestsrc is-live=true ! alawenc ! ed137pay name=ptt_pay ! udpsink host=127.0.0.1 port=5000"
)

payloader = pipeline.get_by_name("ptt_pay")

def toggle_ptt(state):
    """Simple function to switch PTT on the fly"""
    print(f"Setting PTT to: {state}")
    payloader.set_property("ptt-active", state)
    payloader.set_property("ptt-id", 1) # Example ID

# 3. Example: Toggle PTT every 2 seconds
def ptt_logic():
    # This toggles the property. The C code handles the 01 67 header 
    # and the keep-alive packets automatically.
    current_state = payloader.get_property("ptt-active")
    toggle_ptt(not current_state)
    return True

GLib.timeout_add(2000, ptt_logic)

pipeline.set_state(Gst.State.PLAYING)

try:
    loop = GLib.MainLoop()
    loop.run()
except KeyboardInterrupt:
    pipeline.set_state(Gst.State.NULL)