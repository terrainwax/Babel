#ifndef CALL_H
#define CALL_H

#include <QWidget>

namespace Ui {
class call;
}

class call : public QWidget
{
    Q_OBJECT

public:
    explicit call(QWidget *parent = nullptr);
    ~call();
    void setName(std::string);

private slots:
    void on_call_2_clicked();

private:
    Ui::call *ui;
};

#endif // CALL_H
