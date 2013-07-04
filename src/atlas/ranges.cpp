#include <iostream>

#include <QtGui>

#include "window.h"
#include "ranges.h"

Ranges::Ranges(Window *parent)
    : window(parent)
{
    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->addWidget(new QLabel(tr("Low limit:")), 0, 0);
    lowText = new QLineEdit(tr("0.0"));
    inputLayout->addWidget(lowText, 0, 1);

    inputLayout->addWidget(new QLabel(tr("High limit:")), 1, 0);
    highText = new QLineEdit(tr("65535.0"));
    inputLayout->addWidget(highText, 1, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    okButton = new QPushButton(tr("OK"));
    buttonLayout->addWidget(okButton);

    cancelButton = new QPushButton(tr("Cancel"));
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    lowText->setMaxLength(12);
    lowText->setAlignment(Qt::AlignRight);
    highText->setMaxLength(12);
    highText->setAlignment(Qt::AlignRight);

    createActions();

    setWindowTitle(tr("Ranges"));
}

void Ranges::handleOk()
{
    QString low = lowText->text();
    QString high = highText->text();
    std::cout << "Low: " << low.toStdString()
              << " High: " << high.toStdString()
              << std::endl;
    hide();
}

void Ranges::handleCancel()
{
    hide();
}

void Ranges::createActions()
{
    connect(okButton, SIGNAL(released()), this, SLOT(handleOk()));
    connect(cancelButton, SIGNAL(released()), this, SLOT(handleCancel()));
}

