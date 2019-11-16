#ifndef PLAYER_H
#define PLAYER_H
#include<gst/gst.h>

class Player
{
    GstElement  *m_sink,*m_source,*m_pipeline;
public:
    Player();
    ~Player();
    int play(const char* url);
    int stop();
    int attach(unsigned long wId);

};

#endif // PLAYER_H
