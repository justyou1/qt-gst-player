#include "player.h"
#include<gst/gst.h>
#include<stdio.h>
#include<string.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

static void new_pad (GstElement *element, GstPad *pad,gpointer data)
{
   gchar *name;
   GstCaps * p_caps;
   gchar * description;
   GstElement *p_rtph264depay;

   name = gst_pad_get_name(pad);
   g_print("A new pad %s was created\n", name);

   // here, you would setup a new pad link for the newly created pad
   // sooo, now find that rtph264depay is needed and link them?
   p_caps = gst_pad_get_current_caps (pad);

   description = gst_caps_to_string(p_caps);

   p_rtph264depay = GST_ELEMENT(data);

   // try to link the pads then ...

   printf("111 %s\n",description);
   if(strstr(description,"video")){
       printf("222 %s\n",description);

       if(!gst_element_link_pads(element, name, p_rtph264depay, "sink"))
       {
           printf("Failed to link elements 3\n");
       }
   }

   g_free(description);
   g_free(name);
}

Player::Player()
{
    GstElement  *avdec, *depay, *conv;

    if(!gst_is_initialized())
        gst_init(NULL,NULL);
    /*create pipeline*/
    m_pipeline = gst_pipeline_new ("video-player");
    /* set the source audio file */
    m_source = gst_element_factory_make ("rtspsrc", "rtsp-source");

    avdec = gst_element_factory_make ("avdec_h264", "avdec");
    depay = gst_element_factory_make ("rtph264depay", "depay");
    conv = gst_element_factory_make ("autovideoconvert", "converter");
    m_sink = gst_element_factory_make ("xvimagesink", "video-output");
    if (!m_pipeline || !m_source || !depay || !avdec || !conv || !m_sink) {
        g_print ("One element could not be created\n");
    }

    /* put all elements in a bin */
    gst_bin_add_many (GST_BIN (m_pipeline),
              m_source, depay, NULL);
    g_signal_connect(m_source, "pad-added", G_CALLBACK(new_pad),depay);
    gst_bin_add_many (GST_BIN (m_pipeline),
              avdec, conv, m_sink, NULL);
    if(!gst_element_link_many (depay,avdec, conv,m_sink,NULL)){
         printf("Failed to link elements 4\n");
    }
    gst_element_set_state(m_sink, GST_STATE_READY);
}
Player::~Player()
{
    g_print ("Returned, stopping playback\n");
    gst_element_set_state (m_pipeline, GST_STATE_NULL);
    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (m_pipeline));
}
int Player::attach(unsigned long wId)
{
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (m_sink), wId);
    return 0;
}
int Player::play(const char* url)
{
    g_object_set (m_source, "location",url, NULL);
    GstStateChangeReturn sret = gst_element_set_state (m_pipeline,GST_STATE_PLAYING);

    return sret == GST_STATE_CHANGE_SUCCESS ? 0 : -1;
}
