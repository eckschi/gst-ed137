#include <gst/gst.h>
#include <gst/rtp/gstrtpbuffer.h>
#include <gst/rtp/gstrtpbasepayload.h>
#include <string.h>

#define PACKAGE "gsted137pay"
#define VERSION "1.0.0"
#define GST_PACKAGE_NAME "ED-137 PTT Plugin"
#define GST_PACKAGE_ORIGIN "http://gstreamer.net"


#define GST_TYPE_ED137_PAY (gst_ed137_pay_get_type())
G_DECLARE_FINAL_TYPE (GstEd137Pay, gst_ed137_pay, GST, ED137_PAY, GstRTPBasePayload)

struct _GstEd137Pay {
  GstRTPBasePayload parent;
  gboolean active;
  guint ptt_id;
};

G_DEFINE_TYPE (GstEd137Pay, gst_ed137_pay, GST_TYPE_RTP_BASE_PAYLOAD);

enum {
  PROP_0,
  PROP_ACTIVE,
  PROP_PTT_ID
};

static GstFlowReturn
gst_ed137_pay_handle_buffer (GstRTPBasePayload * payload, GstBuffer * buffer)
{
  GstEd137Pay *self = GST_ED137_PAY (payload);
  GstBuffer *outbuf;
  GstRTPBuffer rtp = GST_RTP_BUFFER_INIT;

  /* 1. Wrap the incoming buffer into an RTP packet */
  /* PCMA has 1 sample per byte; we use the base class to handle timestamping */
  outbuf = gst_rtp_base_payload_allocate_output_buffer (payload, 12, 0, 0);
  
  // Copy payload data from input to output
  GstMemory *mem = gst_buffer_get_all_memory (buffer);
  gst_buffer_append_memory (outbuf, mem);

  if (gst_rtp_buffer_map (outbuf, GST_MAP_READWRITE, &rtp)) {
    gpointer ext_data;
    
    /* 2. RFC 3550 Extension: Profile 0x0167, Length 2 (32-bit words) */
    gst_rtp_buffer_set_extension_data (&rtp, 0x0167, 2);

    if (gst_rtp_buffer_get_extension_data (&rtp, NULL, &ext_data, NULL)) {
      guint8 *data = (guint8 *) ext_data;
      
      /* Resulting header after SSRC: 01 67 00 02 00 41 b2 ... */
      data[0] = 0x00;
      data[1] = 0x41;
      data[2] = 0xB2;
      data[3] = self->active ? 0x01 : 0x00;
      data[4] = (guint8) (self->ptt_id & 0xFF);
      data[5] = 0x00;
      data[6] = 0x00;
      data[7] = 0x00;
    }
    gst_rtp_buffer_unmap (&rtp);
  }

  return gst_rtp_base_payload_push (payload, outbuf);
}

static void
gst_ed137_pay_set_property (GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec)
{
  GstEd137Pay *self = GST_ED137_PAY (object);
  switch (prop_id) {
    case PROP_ACTIVE: self->active = g_value_get_boolean (value); break;
    case PROP_PTT_ID: self->ptt_id = g_value_get_uint (value); break;
    default: G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec); break;
  }
}

static void
gst_ed137_pay_get_property (GObject * object, guint prop_id, GValue * value, GParamSpec * pspec)
{
  GstEd137Pay *self = GST_ED137_PAY (object);
  switch (prop_id) {
    case PROP_ACTIVE: g_value_set_boolean (value, self->active); break;
    case PROP_PTT_ID: g_value_set_uint (value, self->ptt_id); break;
    default: G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec); break;
  }
}

static void
gst_ed137_pay_class_init (GstEd137PayClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstRTPBasePayloadClass *rtpbasepayload_class = GST_RTP_BASE_PAYLOAD_CLASS (klass);

  gobject_class->set_property = gst_ed137_pay_set_property;
  gobject_class->get_property = gst_ed137_pay_get_property;

  g_object_class_install_property (gobject_class, PROP_ACTIVE,
      g_param_spec_boolean ("ptt-active", "Active", "ED-137 Active", FALSE, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_PTT_ID,
      g_param_spec_uint ("ptt-id", "PTT ID", "ED-137 PTT ID", 0, 255, 0, G_PARAM_READWRITE));

  rtpbasepayload_class->handle_buffer = gst_ed137_pay_handle_buffer;

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gst_caps_from_string ("application/x-rtp, media=audio, payload=8, encoding-name=PCMA, clock-rate=8000")));
  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gst_caps_from_string ("audio/x-alaw, rate=8000, channels=1")));

  gst_element_class_set_metadata (element_class, "ED137 Payloader", "Codec/Payloader/RTP", "ED137 PCMA Payloader", "Sepp the Forcher");
}

static void gst_ed137_pay_init (GstEd137Pay * self) 
{
    gst_rtp_base_payload_set_options (GST_RTP_BASE_PAYLOAD (self), "audio", TRUE, "PCMA", 8000);

    self->active = FALSE; 
    self->ptt_id = 0; 
}

static gboolean plugin_init (GstPlugin * plugin) {
  return gst_element_register (plugin, "ed137pay", GST_RANK_NONE, GST_TYPE_ED137_PAY);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR, ed137pay, "ED-137", plugin_init, "1.26.0", "LGPL", "GStreamer", "https://gstreamer.net/")