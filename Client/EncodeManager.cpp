//
// Created by terrainwax on 03/10/2018.
//

#include "EncodeManager.h"

EncodeManager::EncodeManager()
{
}

EncodeManager::~EncodeManager()
{
}

bool            EncodeManager::encoderCreate()
{
    int           error;

    if ((_encode = opus_encoder_create(SAMPLE_RATE, CHANNELS, OPUS_APPLICATION_VOIP, &error)) == NULL)
    {
        std::cerr << "Can not create encode" <<std::endl;
        return (false);
    }
    if (opus_encoder_ctl(_encode, OPUS_SET_BITRATE(BITRATE))<0)
    {
        std::cerr << "Can not create encode" <<std::endl;
        return (false);
    }
    return (true);
}

bool            EncodeManager::decoderCreate()
{
    int           error;

    if ((_decode = opus_decoder_create(SAMPLE_RATE, CHANNELS, &error)) == NULL)
    {
        std::cerr << "Can not create decoder" <<std::endl;
        return (false);
    }

    return (true);
}

unsigned char*          EncodeManager::encode(unsigned char *data, int size)
{
    unsigned char         *c_bits = new unsigned char [size];
    unsigned char         *pcm_bytes = new unsigned char [size];

    memcpy(pcm_bytes, data, size);
    int i;
    for (i=0;i<CHANNELS*FRAME_SIZE;i++)
        _in[i]=pcm_bytes[2*i+1]<<8|pcm_bytes[2*i];
    _nbBytes = opus_encode(_encode, _in, FRAME_SIZE, c_bits, size);
    if (_nbBytes<0)
    {
        std::cerr << "cannot decode" << std::endl;
        return NULL;
    }
    return (c_bits);
}
unsigned char*          EncodeManager::decode(unsigned char *data, int size)
{

    int   frame_size = opus_decode(_decode, data, size, _out,MAX_FRAME_SIZE * CHANNELS * 2, 0);
    unsigned char         *pcm_bytes = new unsigned char [MAX_FRAME_SIZE * CHANNELS * 2];

    if (frame_size<0)
    {
        std::cerr << "cannot decode" << std::endl;
        return (NULL);
    }
    memcpy(pcm_bytes, _out, size);
    int i;
    for(i=0;i<CHANNELS*frame_size;i++)
    {
        pcm_bytes[2*i]=_out[i]&0xFF;
        pcm_bytes[2*i+1]=(_out[i]>>8)&0xFF;
    }

    return (pcm_bytes);
}

int             EncodeManager::getEncodeLen() const { return (this->_nbBytes); }