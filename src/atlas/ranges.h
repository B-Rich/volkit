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

private slots:
    void handleOk();
    void handleCancel();

private:
    void createActions();

    QLineEdit *lowText;
    QLineEdit *highText;
    QPushButton *okButton;
    QPushButton *cancelButton;

    Window *window;
};

#endif

