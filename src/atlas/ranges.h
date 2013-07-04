#ifndef RANGES_H
#define RANGES_H

#include <QWidget>

class QWidget;
class QLineEdit;
class QPushButton;

class Ranges : public QWidget
{
    Q_OBJECT

public:
    Ranges(QWidget *parent);

private slots:
    void buttonApply();
    void buttonClose();

private:
    void createActions();

    QLineEdit *lowText;
    QLineEdit *highText;
    QPushButton *applyButton;
    QPushButton *closeButton;

    QWidget *parentWidget;
};

#endif

