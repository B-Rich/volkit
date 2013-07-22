#include <iostream>
#include <stdlib.h>

#include <QtGui>

#include "imgwindow.h"
#include "imgparams.h"

ImgParams::ImgParams(QWidget *parent)
    : parentWidget(parent)
{
    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->addWidget(new QLabel(tr("Low limit:")), 0, 0);
    lowText = new QLineEdit(tr("0.0"));
    inputLayout->addWidget(lowText, 0, 1);

    inputLayout->addWidget(new QLabel(tr("High limit:")), 1, 0);
    highText = new QLineEdit(tr("65535.0"));
    inputLayout->addWidget(highText, 1, 1);

    inputLayout->addWidget(new QLabel(tr("Transparency:")), 2, 0);
    transCheck = new QCheckBox();
    inputLayout->addWidget(transCheck, 2, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    applyButton = new QPushButton(tr("Apply"));
    buttonLayout->addWidget(applyButton);

    closeButton = new QPushButton(tr("Close"));
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    lowText->setMaxLength(12);
    lowText->setAlignment(Qt::AlignRight);
    highText->setMaxLength(12);
    highText->setAlignment(Qt::AlignRight);

    createActions();

    setWindowTitle(tr("Image parameters"));
}

void ImgParams::buttonApply()
{
    QString low = lowText->text();
    QString high = highText->text();
    ImgWindow *w = (ImgWindow *) parentWidget;

    w->setParams(atof(low.toStdString().c_str()),
                 atof(high.toStdString().c_str()),
                 transCheck->isChecked());
}

void ImgParams::buttonClose()
{
    hide();
}

void ImgParams::createActions()
{
    connect(applyButton, SIGNAL(released()), this, SLOT(buttonApply()));
    connect(closeButton, SIGNAL(released()), this, SLOT(buttonClose()));
}

