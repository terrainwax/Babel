#ifndef INCALL_H
#define INCALL_H

#include <QWidget>

namespace Ui {
class incall;
}

class incall : public QWidget
{
    Q_OBJECT

public:
    explicit incall(QWidget *parent = nullptr);
    ~incall();
    void setName(std::string);

private slots:
    void on_endcall_clicked();

    void on_mute_clicked();

    void on_soundmute_clicked();

private:
    Ui::incall *ui;
};

#endif // INCALL_H
