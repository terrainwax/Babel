#ifndef USER_H
#define USER_H

#include <QWidget>

namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    ~User();
    void setName(std::string name);
    void setStatus(std::string status);
    std::string  getName();

private:
    Ui::User *ui;
};

#endif // USER_H
