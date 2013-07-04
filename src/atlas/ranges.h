#ifndef RANGES_H
#define RANGES_H

#include <QWidget>

class Window;
class QLineEdit;
class QPushButton;

class Ranges : public QWidget
{
    Q_OBJECT

public:
    Ranges(Window *parent);

private:
    QLineEdit *lowText;
    QLineEdit *highText;
    QPushButton *okButton;
    QPushButton *cancelButton;

    Window *window;
};

#endif

