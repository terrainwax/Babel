#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <ui_login.h>
#include "AudioManager.h"
#include "EncodeManager.h"
#include "ClientCrypto.h"
#include "Packet.h"
#include "Command.h"
#include "login.h"
#include "AudioThread.h"
#include "VoiceThread.h"

class Client : public QObject
{
Q_OBJECT
public:
    Client();
    void Initialize();
    bool Connect();
    void ReadHeader();
    void ReadBody();
    void deliver(const BabelString &message, bool command);
    void deliver(const BabelString &message);
    void write();
    void LoginResponse(char *str);
    void CallResponse(char *str);
    void JoinResponse(char *str);
    void ParseDefault(char *str);
    void setUserStatus(std::string user, std::string status);
    bool Call(QString ip, int port);
    int  VoiceSize(const unsigned char *data);
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void processPendingDatagrams();




public:
    QTcpSocket *TcpClient;
    QUdpSocket *UdpClient;
    AudioManager *Amanager;
    EncodeManager *Emanager;
    QString serverName;
    quint16 serverPort;
    std::string userwaited;
    std::map<std::string , std::map<std::string, int>> clientCall;
    int Myport;
    bool muted = false;
    ClientCrypto _crypto;
    Packet _p;
    Packet _ad;
    PacketQueue _writeMessageQueue;
    int id;
    AudioThread *audio;
    VoiceThread *voice;

    bool incall = false;

    bool _secured;

public:
    Ui::Login *ui;
    Login *w;

};

#endif // CLIENT_H
