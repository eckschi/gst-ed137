#ifndef __GST_ED137_PAY_H__
#define __GST_ED137_PAY_H__

#include <gst/gst.h>
#include <gst/rtp/gstrtpbasepayload.h>

G_BEGIN_DECLS

#define GST_TYPE_ED137_PAY (gst_ed137_pay_get_type())
G_DECLARE_FINAL_TYPE (GstEd137Pay, gst_ed137_pay, GST, ED137_PAY, GstRTPBasePayload)

G_END_DECLS

#endif /* __GST_ED137_PAY_H__ */