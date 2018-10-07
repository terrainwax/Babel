#ifndef RECEIVE_H
#define RECEIVE_H

#include <QWidget>

namespace Ui {
class receive;
}

class receive : public QWidget
{
    Q_OBJECT

public:
    explicit receive(QWidget *parent = nullptr);
    ~receive();
    void setName(std::string);

private slots:
    void on_join_clicked();

    void on_hang_clicked();

private:
    Ui::receive *ui;
};

#endif // RECEIVE_H
