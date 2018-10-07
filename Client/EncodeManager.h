//
// Created by terrainwax on 03/10/2018.
//

#ifndef CPP_BABEL_2018_ENCODEMANAGER_H
#define CPP_BABEL_2018_ENCODEMANAGER_H

#include "iostream"
#include "opus.h"


#define FRAME_SIZE (480)
#define SAMPLE_RATE (24000)
#define CHANNELS (2)
#define BITRATE 64000
#define MAX_PACKET_SIZE (3*1276)
#define APPLICATION OPUS_APPLICATION_VOIP

#define MAX_FRAME_SIZE (6*480)

class                           EncodeManager
{

private:
    OpusEncoder                   *_encode;
    OpusDecoder                   *_decode;

    opus_int16                    _in[FRAME_SIZE*CHANNELS];
    opus_int16                    _out[MAX_FRAME_SIZE*CHANNELS];
    int                           _nbBytes;


public:
    EncodeManager();
    ~EncodeManager();

    bool                          encoderCreate();
    bool                          decoderCreate();

    unsigned char*                encode(unsigned char *, int);
    unsigned char*                decode(unsigned char *, int);

    int                           getEncodeLen() const;

};


#endif //CPP_BABEL_2018_ENCODEMANAGER_H
