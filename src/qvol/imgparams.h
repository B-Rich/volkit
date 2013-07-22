#ifndef IMGPARAMS_H
#define IMGPARAMS_H

#include <QWidget>

class QWidget;
class QLineEdit;
class QCheckBox;
class QPushButton;

class ImgParams : public QWidget
{
    Q_OBJECT

public:
    ImgParams(QWidget *parent);

private slots:
    void buttonApply();
    void buttonClose();

private:
    void createActions();

    QLineEdit *lowText;
    QLineEdit *highText;
    QCheckBox *transCheck;
    QPushButton *applyButton;
    QPushButton *closeButton;

    QWidget *parentWidget;
};

#endif

