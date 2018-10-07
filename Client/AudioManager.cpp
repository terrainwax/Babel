//
// Created by terrainwax on 03/10/2018.
//

#include "AudioManager.h"

AudioManager::AudioManager()
{
    _stream = NULL;
    _readBufferSize = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE;
    _readBuffer= new unsigned char [_readBufferSize];
}

AudioManager::~AudioManager()
{
}

bool            AudioManager::initPa()
{
    if ((Pa_Initialize()) != paNoError)
        return (false);
    return (true);
}

bool            AudioManager::openStream()
{
    if ((Pa_OpenStream(&_stream, &_inputParam, &_outputParam, SAMPLE_RATE,
                       FRAMES_PER_BUFFER, paClipOff, NULL, NULL)) != paNoError)
        return (false);
    return (true);
}

bool            AudioManager::startStream()
{
    if ((Pa_StartStream(_stream)) != paNoError)
        return (false);
    return (true);
}

bool            AudioManager::initStream()
{
    if ((openStream()) == false)
        std::cerr << "can not open stream" << std::endl;
    if ((startStream()) == false)
        std::cerr << "cannot start stream" <<std::endl;
    return (true);
}

bool            AudioManager::initParams()
{
    if ((initPa()) == false)
        std::cerr << "can not ijnit PA" << std::endl;
    initInputParams();
    initOutputParams();
    return (true);
}

bool            AudioManager::initInputParams()
{
    if ((_inputParam.device = Pa_GetDefaultInputDevice()) == paNoDevice)
        return (false);
    _inputParam.channelCount = NUM_CHANNELS;
    _inputParam.sampleFormat = PA_SAMPLE_TYPE;
    #ifdef __linux__
    _inputParam.suggestedLatency = 0.030f;
    #else
    _inputParam.suggestedLatency = Pa_GetDeviceInfo(_inputParam.device)->defaultLowInputLatency;
    #endif
    _inputParam.hostApiSpecificStreamInfo = NULL;
    return (true);
}

bool            AudioManager::initOutputParams()
{
    if ((_outputParam.device = Pa_GetDefaultOutputDevice()) == paNoDevice)
        return (false);
    _outputParam.channelCount = NUM_CHANNELS;
    _outputParam.sampleFormat = PA_SAMPLE_TYPE;
    #ifdef __linux__
    _outputParam.suggestedLatency = 0.030f;
    #else
    _outputParam.suggestedLatency = Pa_GetDeviceInfo(_inputParam.device)->defaultLowInputLatency;
    #endif
    _outputParam.hostApiSpecificStreamInfo = NULL;
    return (true);
}

bool            AudioManager::writeOnStream(unsigned char *buff)
{
    if ((Pa_WriteStream(_stream, buff, FRAMES_PER_BUFFER)) != paNoError)
    {
        std::cout << "FAIL WRITE" <<std::endl;
        return (false);
    }
    return (true);
}

bool            AudioManager::readFromStream()
{
    if ((Pa_ReadStream(_stream, _readBuffer, FRAMES_PER_BUFFER)) != paNoError)
        return (false);
    return (true);
}

void            AudioManager::cleanReadBuffer()
{
    for (int i = 0; i != _readBufferSize; i++)
        _readBuffer[i] = 0;
}

int             AudioManager::getReadBufferSize() const
{
    return (_readBufferSize);
}

unsigned char*          AudioManager::getReadBuffer() const {
    return (_readBuffer);
}
