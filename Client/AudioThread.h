//
// Created by terrainwax on 05/10/2018.
//

#ifndef CPP_BABEL_2018_AUDIOTHREAD_H
#define CPP_BABEL_2018_AUDIOTHREAD_H

#include "QThread"
#include "QObject"

class AudioThread : public QThread
{
    Q_OBJECT

protected:
    void run();
};


#endif //CPP_BABEL_2018_AUDIOTHREAD_H
