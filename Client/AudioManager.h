//
// Created by terrainwax on 03/10/2018.
//

#ifndef CPP_BABEL_2018_AUDIOMANAGER_H
#define CPP_BABEL_2018_AUDIOMANAGER_H

#include "iostream"
#include "portaudio.h"

#define NUM_CHANNELS    (2)
#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_RATE  (24000)
#define FRAMES_PER_BUFFER (480)
#define SAMPLE_SIZE (3)
#define FRAME_SIZE (480)

class                           AudioManager
{

private:
    PaStream                      *_stream;
    int                           _readBufferSize;
    PaStreamParameters            _inputParam;
    PaStreamParameters            _outputParam;
    unsigned char                         *_readBuffer;

public:
    AudioManager();
    ~AudioManager();

    // Init Stream
    bool                          initPa();
    bool                          openStream();
    bool                          startStream();
    bool                          initStream();

    // Init params stream
    bool                          initParams() const;
    bool                          initInputParams();
    bool                          initOutputParams();
    bool                          initParams();

    // I/O
    bool                          writeOnStream(unsigned char *buff);
    bool                          readFromStream();

    // Utils
    void                          cleanReadBuffer();
    int                           getReadBufferSize() const;
    unsigned char                 *getReadBuffer() const;

};


#endif //CPP_BABEL_2018_AUDIOMANAGER_H
