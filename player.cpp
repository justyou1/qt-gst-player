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
    GstElement  *avdec, *depay, *conv,*source,*sink;
    if(!gst_is_initialized())
        gst_init(NULL,NULL);
    /*create pipeline*/
    m_pipeline = gst_pipeline_new ("video-player");
    /* set the source audio file */
    source = gst_element_factory_make ("rtspsrc", "rtsp-source");

    avdec = gst_element_factory_make ("avdec_h264", "avdec");
    depay = gst_element_factory_make ("rtph264depay", "depay");
    conv = gst_element_factory_make ("autovideoconvert", "converter");
    sink = gst_element_factory_make ("xvimagesink", "video-output");
    if (!m_pipeline || !source || !depay || !avdec || !conv || !sink) {
        g_print ("One element could not be created\n");
    }

    /* put all elements in a bin */
    gst_bin_add_many (GST_BIN (m_pipeline),
              source, depay, NULL);
    g_signal_connect(source, "pad-added", G_CALLBACK(new_pad),depay);
    gst_bin_add_many (GST_BIN (m_pipeline),
              avdec, conv, sink, NULL);
    if(!gst_element_link_many (depay,avdec, conv,sink,NULL)){
         printf("Failed to link elements 4\n");
    }
    gst_element_set_state(sink, GST_STATE_READY);
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
    m_wid = wId;
    return 0;
}
int Player::play(const char* url)
{
    GstElement * source=gst_bin_get_by_name (GST_BIN(m_pipeline),"rtsp-source");
    g_object_set (source, "location",url, NULL);
    gst_object_unref (GST_OBJECT (source));

    GstElement * sink=gst_bin_get_by_name (GST_BIN(m_pipeline),"video-output");
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), m_wid);
    gst_object_unref (GST_OBJECT (sink));

    GstStateChangeReturn sret = gst_element_set_state (m_pipeline,GST_STATE_PLAYING);

    printf("%d\n",sret);
    return sret == GST_STATE_CHANGE_SUCCESS ? 0 : -1;
}
int Player::stop(){
    //GstState state,pending;
    GstStateChangeReturn sret;
    //GstStateChangeReturn sret = gst_element_get_state(m_pipeline,&state,&pending,100);
    //if(sret == GST_STATE_CHANGE_SUCCESS && state == GST_STATE_PLAYING)
        sret = gst_element_set_state (m_pipeline,GST_STATE_NULL);
        printf("%d\n",sret);
    return sret == GST_STATE_CHANGE_SUCCESS ? 0 : -1;
}
int Player::paused(){

    GstStateChangeReturn sret = gst_element_set_state (m_pipeline,GST_STATE_PAUSED);
    printf("%d\n",sret);
    return sret == GST_STATE_CHANGE_SUCCESS ? 0 : -1;
}
