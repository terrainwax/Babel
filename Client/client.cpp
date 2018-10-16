#include <openssl/evp.h>
#include "iostream"
#include <iomanip>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtCore/QPropertyAnimation>
#include "client.h"
#include "Packet.h"
#include "Command.h"
#include "Voice.h"
#include "login.h"
#include "AudioThread.h"
#include <sstream>
#include <QtNetwork/qabstractsocket.h>
#include  "QTimer"
#include "VoiceThread.h"

Client::Client()
{
    OpenSSL_add_all_algorithms();
    this->TcpClient = new QTcpSocket();
    this->UdpClient = new QUdpSocket();
    this->audio = new AudioThread();
    this->voice = new VoiceThread();
    this->Amanager = new AudioManager();
    this->Amanager->cleanReadBuffer();
    this->Emanager = new EncodeManager();
    this->Emanager->decoderCreate();
    this->Emanager->encoderCreate();
    this->Amanager->initParams();
    this->Amanager->initStream();
    this->serverName = "babibel.paladium-pvp.fr";
    this->serverPort = 42069;
    this->audio->start();
    connect(this->UdpClient, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(this->UdpClient, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(this->TcpClient, SIGNAL(connected()),this, SLOT(connected()));
    connect(this->TcpClient, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(this->TcpClient, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(this->TcpClient, SIGNAL(readyRead()),this, SLOT(readyRead()));

}


void Client::processPendingDatagrams()
{
        QHostAddress ip;
        quint16 port;
        std::cout << "receive something1" << std::endl;
    if (!this->isCo)
    {

        QByteArray data;
        this->UdpClient->readDatagram(data.data(), 12, &ip, &port);
        std::cout << data.toStdString() << std::endl;
        if (data.toStdString() == std::string("HOLEPUNCHING")) {
            this->isCo = true;
            QByteArray send;
            send.append("HOLEPUNCHING");
            this->UdpClient->writeDatagram(send,ip, port);
        }
    }
    else {
        this->UdpClient->readDatagram(this->_ad.data(), Packet::max_body_length + Packet::header_length, &ip, &port);
        if (this->_ad.decodeHeader()) {
            bool conversionOK = false;
            QHostAddress ip4Address(ip.toIPv4Address(&conversionOK));
            QString ip4String;
            if (conversionOK) {
                ip4String = ip4Address.toString();
            }

            for (std::pair<const std::string, std::map<std::string, int>> &ent2 : this->clientCall) {
                std::map<std::string, int>::iterator i = ent2.second.find(ip4String.toStdString());
                if (i == ent2.second.end()) {
                    return;
                }
            }
            auto voice = (Voice *) this->_ad.body();
            if (voice->magic != VOICE_MAGIC)
                return;
            unsigned char *data = this->Emanager->decode(voice->data.data, voice->data.N_bytes);
            if (!this->soundmuted)
                this->Amanager->writeOnStream(data);
        }
    }
}

bool Client::Call(QString ip, int port)
{
    std::cout << "port bind = " << port << std::endl;
    this->UdpClient->bind(QHostAddress::Any, port);
    this->incall = true;
    this->audio->start();
    /*QEventLoop blocking;
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(3 * 1000);
    connect(this->UdpClient, &QUdpSocket::readyRead, &blocking, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &blocking, &QEventLoop::quit);
    timer.start();
    blocking.exec();*/

    return true;

}


bool Client::Connect() {
    this->TcpClient->connectToHost(this->serverName, this->serverPort);
    if (!this->TcpClient->waitForConnected()) {
        std::cerr << "Connection timeout" << std::endl;
        return false;
    }
    return true;
}

void Client::connected()
{
    qDebug() << "connected...";
}

void Client::disconnected()
{
    qDebug() << "disconnected...";
}

void Client::bytesWritten(qint64 bytes)
{

}

void Client::readyRead()
{
    ReadHeader();

}

int Client::VoiceSize(const unsigned char* data)
{
    int i = 0;
    for (i = 0; data[i] != 0 ; ++i);
    return i;
}

void Client::ReadHeader()
{
    while (this->TcpClient->bytesAvailable()) {
        this->TcpClient->read(_p.data(), Packet::header_length);
        if (_p.decodeHeader()) {
            ReadBody();
        } else
            std::cout << "fail" << std::endl;
    }

}

void Client::ReadBody()
{

        this->TcpClient->read(_p.body(), _p.bodyLength());
        if (!_secured) {
            if (_p.str().substr(0, 30) == "-----BEGIN RSA PUBLIC KEY-----") {
                const BabelString RSAPublicKey = _p.str();
                _crypto.setRemotePublicKey(RSAPublicKey);

                std::cout << BabelString("Received RSA Public Key:\n") + _crypto.getRemotePublicKey() << std::endl;

                BabelString aesKey = _crypto.getAESKey();

                std::cout << "Sending Encrypted AES Key: ";

                for (int i = 0; i < aesKey.getSize(); ++i)
                    std::cout << std::hex << std::setfill('0') << std::setw(2)
                              << (unsigned int) (unsigned char) aesKey.getData()[i];

                std::cout << std::endl;

                BabelString encryptedAESKey = _crypto.encryptRSA(aesKey);

                deliver(BabelString("ENCRYPTED AES KEY:") + encryptedAESKey);

                BabelString aesIv = _crypto.getAESIv();

                std::cout << "Sending Encrypted AES Iv: ";

                for (int i = 0; i < aesIv.getSize(); ++i)
                    std::cout << std::hex << std::setfill('0') << std::setw(2)
                              << (unsigned int) (unsigned char) aesIv.getData()[i];

                std::cout << std::endl;

                BabelString encryptedAESIv = _crypto.encryptRSA(aesIv);

                deliver(BabelString("ENCRYPTED AES IV:") + encryptedAESIv);

                std::cout << BabelString("ClientSession Secured: ") << std::endl;

                _secured = true;
            }
        } else {
            BabelString decrypted = _crypto.decryptAES(_p.str());
            switch (id) {
                case CommandIdentifier::LOGIN :
                    LoginResponse(decrypted.getData());
                    id = -1;
                    break;
                case CommandIdentifier::CALL :
                    CallResponse(decrypted.getData());
                    id = -1;
                    break;
                case CommandIdentifier::JOIN :
                    JoinResponse(decrypted.getData());
                    id = -1;
                    break;
                default:
                    ParseDefault(decrypted.getData());
            }
            std::cout << "Response :" << decrypted.getData() << std::endl;
        }

}
void Client::CallResponse(char *str)
{
    std::string text = std::string(str);

    std::istringstream iss(text);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    if (results.at(0) == std::string("OK")) {
        std::string ip = results.at(1);
        this->clientCall[this->userwaited][ip] = -1;
        Call(ip.c_str(), this->Myport);

    }

}
void Client::JoinResponse(char *str)
{
    std::string text = std::string(str);

    std::istringstream iss(text);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    if (results.at(0) == std::string("OK")) {
        std::string ip = results.at(1);
        std::string port = results.at(2);
        this->clientCall[this->userwaited][ip] = std::atoi(port.c_str());
        Call(ip.c_str(), this->Myport);
        if (!this->isCo) {
            QByteArray send;
            send.append("HOLEPUNCHING");
            this->UdpClient->writeDatagram(send, QHostAddress(ip.c_str()), std::atoi(port.c_str()));
        }
    }

}

void Client::deliver(const BabelString &message, bool command)
{
    BabelString data = message;
    if (command) {
        auto comand = (Command *) message.getData();
        std::cout << comand->data.id << std::endl;
    }
    if (_secured)
        data = _crypto.encryptAES(message);


    Packet packet;
    packet.bodyLength(data.getSize());
    std::memcpy(packet.body(), data.getData(), packet.bodyLength());
    packet.encodeHeader();

    std::cout << ((Command *)packet.body())->magic << std::endl;

    bool write_in_progress = !_writeMessageQueue.empty();
    _writeMessageQueue.push_back(packet);

    if (!write_in_progress) {
        write();
    }
}

void Client::deliver(const BabelString &message)
{
    BabelString data = message;
    if (_secured)
        data = _crypto.encryptAES(message);


    Packet packet;
    packet.bodyLength(data.getSize());
    std::memcpy(packet.body(), data.getData(), packet.bodyLength());
    packet.encodeHeader();

    bool write_in_progress = !_writeMessageQueue.empty();
    _writeMessageQueue.push_back(packet);

    if (!write_in_progress) {
        write();
    }
}

void Client::ParseDefault(char *str)
{
    if (str == nullptr)
        return;
    std::string text = std::string(str);

    std::istringstream iss(text);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    if (results.at(0) == std::string("LIST")) {
        setUserStatus(results.at(1), results.at(2));
    } else if (results.at(0) == std::string("CALL"))
    {
        ui->widget_2->setHidden(false);
        ui->widget_2->setName(results.at(1));

    }
    else if (results.at(0) == std::string("JOIN"))
    {
        std::string ip;
        int port;
        for (auto &ent1 : this->clientCall[results.at(1)])
        {
            ent1.second = std::atoi(results.at(2).c_str());
            ip = ent1.first;
            port = ent1.second;
        }
        if (!this->isCo) {
            QByteArray send;
            send.append("HOLEPUNCHING");
            this->UdpClient->writeDatagram(send, QHostAddress(ip.c_str()), port);
        }
        this->ui->widget_3->setName(results.at(1));
        this->ui->widget_3->setHidden(false);
    }
    else if (results.at(0) == std::string("HANG"))
    {
        //this->isCo = false;
        this->incall = false;
        this->clientCall.clear();
        this->UdpClient->close();
        this->ui->widget_3->setHidden(true);
    }

}

void Client::LoginResponse(char *str)
{
    if (strcmp(str,"KO") == 0)
    {
        ui->lineEdit->setDisabled(0);
        ui->lineEdit_2->setDisabled(0);
        return;
    }


    ui->pseudo->setText(ui->lineEdit->text());
    if (ui->stackedWidget->currentIndex() == 0)
    {
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->label_5->setGraphicsEffect(eff);
        ui->frame_3->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
        a->setDuration(500);
        a->setStartValue(1);
        a->setEndValue(0);
        a->setEasingCurve(QEasingCurve::OutBack);
        a->start(QPropertyAnimation::DeleteWhenStopped);
        connect(a,&QPropertyAnimation::finished,w,&Login::hideThisWidget);
    }
    Command c2 = {0};
    c2.magic = COMMAND_MAGIC;
    c2.data.id = CommandIdentifier::ONLINE;
    deliver(BabelString((char *)&c2, sizeof(Command)));
    Command c3 = {0};
    c3.magic = COMMAND_MAGIC;
    c3.data.id = CommandIdentifier::LIST;
    deliver(BabelString((char *)&c3, sizeof(Command)));
}

void Client::write()
{
    auto _message = BabelString(_writeMessageQueue.front().data(),
                                _writeMessageQueue.front().length());
    this->TcpClient->write(_message.getData(), _message.getSize());
    _writeMessageQueue.pop_front();
    if (!_writeMessageQueue.empty()) {
        write();
    }
}

void Client::setUserStatus(std::string user, std::string status) {
    for (int i = 0; i < ui->listWidget->count() ; ++i) {
        User *use = static_cast<User *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
        if (use->getName() == user) {
            use->setStatus(status);
            return;
        }
        else if (user == this->ui->pseudo->text().toStdString())
            return;
    }
    QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listWidget);
    ui->listWidget->addItem (listWidgetItem);
    User *theWidgetItem = new User;
    theWidgetItem->setName(user);
    theWidgetItem->setStatus(status);
    listWidgetItem->setSizeHint (theWidgetItem->sizeHint ());
    ui->listWidget->setItemWidget (listWidgetItem, theWidgetItem);
}
