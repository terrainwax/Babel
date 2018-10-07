//
// Created by terrainwax on 07/10/2018.
//

#ifndef CPP_BABEL_2018_VOICETHREAD_H
#define CPP_BABEL_2018_VOICETHREAD_H


#include "QThread"
#include "QObject"

class VoiceThread : public QThread
{
Q_OBJECT

protected:
    void run();
};


#endif //CPP_BABEL_2018_VOICETHREAD_H
