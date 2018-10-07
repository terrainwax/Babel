//
// Created by terrainwax on 05/10/2018.
//

#include "AudioThread.h"
#include "GlobalSession.h"
#include "Voice.h"

void AudioThread::run() {
    while(client->incall && client->Amanager->readFromStream()) {
        unsigned char *data = client->Amanager->getReadBuffer();
        unsigned char *encode = client->Emanager->encode(data, client->Amanager->getReadBufferSize());
        Voice v;
        v.magic = VOICE_MAGIC;
        std::memset(v.data.data, 0 , sizeof(v.data.data));
        v.data.N_bytes = static_cast<unsigned int>(client->Emanager->getEncodeLen());
        std::memcpy(v.data.data , encode,v.data.N_bytes);
        BabelString b = BabelString((char *)&v, sizeof(Voice));
        Packet packet;
        packet.bodyLength(b.getSize());
        std::memcpy(packet.body(), b.getData(), packet.bodyLength());
        packet.encodeHeader();
        auto voice = (Voice *) packet.body();
        client->UdpClient->writeDatagram(packet.data() , packet.length(), QHostAddress("127.0.0.1"),
                                         static_cast<quint16>(client->portcalled));
    }
    std::cout << "call not started" << std::endl;
}