#ifndef PLAYER_H
#define PLAYER_H
#include<gst/gst.h>

class Player
{
    GstElement  *m_pipeline;
    unsigned long m_wid;
public:
    Player();
    ~Player();
    int play(const char* url);
    int stop();
    int paused();
    int attach(unsigned long wId);

};

#endif // PLAYER_H
