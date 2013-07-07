#include <QtGui>

#include <stdlib.h>
#include <iostream>
#include <string>

#include "img/vffimg.h"
#include "img/ecat7img.h"
#include "glwidget.h"
#include "polytool.h"
#include "window.h"

Window::Window()
    : rangesDialog(this),
      colorMap(),
      imgLoaded(false)
{
    workWidget = new QWidget;
    setCentralWidget(workWidget);

    selectedTool = new Polytool;
    glWidget = new GLWidget(selectedTool, this);

    sliceScroll = new QScrollBar(Qt::Vertical);
    connect(sliceScroll, SIGNAL(valueChanged(int)), this, SLOT(setSlice(int)));
    connect(this, SIGNAL(sliceChanged(int)), sliceScroll, SLOT(setValue(int)));

    QHBoxLayout *horLayout = new QHBoxLayout;
    horLayout->addWidget(glWidget);
    horLayout->addWidget(sliceScroll);

    frameScroll = new QScrollBar(Qt::Horizontal);
    connect(frameScroll, SIGNAL(valueChanged(int)), this, SLOT(setFrame(int)));
    connect(this, SIGNAL(frameChanged(int)), sliceScroll, SLOT(setValue(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horLayout);
    mainLayout->addWidget(frameScroll);

    workWidget->setLayout(mainLayout);

    sliceScroll->setValue(0);
    frameScroll->setValue(0);

    createActions();
    createMenus();
    createToolBars();
    statusBar()->showMessage(tr("Ready"));

    setWindowTitle(tr("Atlas"));
}

void Window::imageOpen()
{
    QString fn =
        QFileDialog::getOpenFileName(this, tr("Open File..."),
            QString(), tr("Ecat7 (*.v);;Vff (*.vff)"));
    if (!fn.isEmpty())
    {
        if (imgLoaded)
        {
            closeImg();
        }

        int result = loadImg(fn.toStdString().c_str());
        if (result == 0)
        {
            statusBar()->showMessage(tr("Loaded image: ") + fn);
        }
        else
        {
            statusBar()->showMessage(tr("Unable to load image: ") + fn);
        }
    }
}

void Window::imageClose()
{
    if (imgLoaded)
    {
        closeImg();
        glWidget->unsetData();
    }
}

void Window::imageHorizontal()
{
    imageHorizontalAct->setEnabled(false);
    imageSagittalAct->setEnabled(true);
    imageCoronalAct->setEnabled(true);
    setOrientation(Img::ORIENTATION_HORIZONTAL);
}

void Window::imageSagittal()
{
    imageHorizontalAct->setEnabled(true);
    imageSagittalAct->setEnabled(false);
    imageCoronalAct->setEnabled(true);
    setOrientation(Img::ORIENTATION_SAGITTAL);
}

void Window::imageCoronal()
{
    imageHorizontalAct->setEnabled(true);
    imageSagittalAct->setEnabled(true);
    imageCoronalAct->setEnabled(false);
    setOrientation(Img::ORIENTATION_CORONAL);
}

void Window::imageRanges()
{
    rangesDialog.show();
}

void Window::imageColormap()
{
    QString fn =
        QFileDialog::getOpenFileName(this, tr("Load colormap..."),
            QString(), tr("Colormap (*.map);;All Files (*)"));
    if (!fn.isEmpty())
    {
        int result = loadColormap(fn.toStdString().c_str());
        if (result == 0)
        {
            statusBar()->showMessage(tr("Loaded colormap: ") + fn);
        }
        else
        {
            statusBar()->showMessage(tr("Unable to load colormap: ") + fn);
        }
    }
}

void Window::imageExit()
{
    exit(0);
}

void Window::toolsSelect()
{
    selectedTool->setType(Tool::TYPE_SELECT);
    statusBar()->showMessage(tr("Click on object to select"));
}

void Window::toolsPolygon()
{
    selectedTool->setType(Tool::TYPE_POLYGON);
    statusBar()->showMessage(tr("Draw polygon"));
}

void Window::toolsSample()
{
    selectedTool->setType(Tool::TYPE_SAMPLE);
    statusBar()->showMessage(tr("Select point to sample"));
}

void Window::createActions()
{
    imageOpenAct = new QAction(tr("&Open"), this);
    connect(imageOpenAct, SIGNAL(triggered()), this, SLOT(imageOpen()));

    imageCloseAct = new QAction(tr("&Close"), this);
    connect(imageCloseAct, SIGNAL(triggered()), this, SLOT(imageClose()));

    imageHorizontalAct = new QAction(tr("&Horizontal"), this);
    connect(imageHorizontalAct, SIGNAL(triggered()), this, SLOT(imageHorizontal()));

    imageSagittalAct = new QAction(tr("&Sagittal"), this);
    connect(imageSagittalAct, SIGNAL(triggered()), this, SLOT(imageSagittal()));

    imageCoronalAct = new QAction(tr("&Coronal"), this);
    connect(imageCoronalAct, SIGNAL(triggered()), this, SLOT(imageCoronal()));

    imageRangesAct = new QAction(tr("&Ranges"), this);
    connect(imageRangesAct, SIGNAL(triggered()), this, SLOT(imageRanges()));

    imageColormapAct = new QAction(tr("Color&map"), this);
    connect(imageColormapAct, SIGNAL(triggered()), this, SLOT(imageColormap()));

    imageExitAct = new QAction(tr("&Exit"), this);
    connect(imageExitAct, SIGNAL(triggered()), this, SLOT(imageExit()));

    toolsSelectAct = new QAction(
        QIcon(":/icons/select.png"),
        tr("Select"),
        this
        );
    toolsSelectAct->setStatusTip(tr("Select object"));
    connect(toolsSelectAct, SIGNAL(triggered()), this, SLOT(toolsSelect()));

    toolsPolygonAct = new QAction(
        QIcon(":/icons/polygon.png"),
        tr("Polygon"),
        this
        );
    toolsPolygonAct->setStatusTip(tr("Draw polygon"));
    connect(toolsPolygonAct, SIGNAL(triggered()), this, SLOT(toolsPolygon()));

    toolsSampleAct = new QAction(
        QIcon(":/icons/sample.png"),
        tr("Sample"),
        this
        );
    toolsSampleAct->setStatusTip(tr("Sample image values"));
    connect(toolsSampleAct, SIGNAL(triggered()), this, SLOT(toolsSample()));
}

void Window::createMenus()
{
    imageMenu = menuBar()->addMenu(tr("&Image"));
    imageMenu->addAction(imageOpenAct);
    imageMenu->addAction(imageCloseAct);
    imageMenu->addSeparator();
    imageMenu->addAction(imageHorizontalAct);
    imageMenu->addAction(imageSagittalAct);
    imageMenu->addAction(imageCoronalAct);
    imageMenu->addSeparator();
    imageMenu->addAction(imageRangesAct);
    imageMenu->addAction(imageColormapAct);
    imageMenu->addSeparator();
    imageMenu->addAction(imageExitAct);
}

void Window::createToolBars()
{
    toolsToolBar = addToolBar(tr("Tools"));
    toolsToolBar->addAction(toolsSelectAct);
    toolsToolBar->addAction(toolsPolygonAct);
    toolsToolBar->addAction(toolsSampleAct);
}

void Window::setSlice(int slice)
{
    if (slice != imgSlice)
    {
        imgSlice = slice;
        if (imgLoaded)
        {
            glWidget->setData(
                imgBase->getWidth(), imgBase->getHeight(),
                &imgData[imgSlice * imgBase->getWidth() * imgBase->getHeight()]
                );
        }
    }
}

void Window::setFrame(int frame)
{
    if (frame != imgFrame)
    {
        imgFrame = frame;
        if (imgLoaded)
        {
           imgBase->read(frame);
           readImgData();
        }
    }
}

void Window::readImgData()
{
    imgBase->getData(imgData, &colorMap);
    sliceScroll->setRange(0, imgBase->getDepth() - 1);
    sliceScroll->setValue(imgSlice);
    glWidget->setData(
        imgBase->getWidth(), imgBase->getHeight(),
        &imgData[imgSlice * imgBase->getWidth() * imgBase->getHeight()]
        );
}

int Window::loadImg(
    const char *fn,
    int slice,
    int frame
    )
{
    Img *img = 0;

    std::string fname(fn);
    size_t pos = fname.find_last_of(".");
    std::string ext = fname.substr(pos + 1);
    if (ext.compare("vff") == 0)
    {
        img = new VffImg;
    }
    else if (ext.compare("v") == 0)
    {
        img = new Ecat7Img;
    }

    int result = img->open(fn);
    if (result == 0)
    {
        imageHorizontalAct->setEnabled(false);
        imageSagittalAct->setEnabled(true);
        imageCoronalAct->setEnabled(true);
        img->setLimits(0.0, 65000.0);
        result = readImg(img, slice, frame);
    }

    return result;
}

int Window::readImg(
    Img *img,
    int slice,
    int frame
    )
{
    if (slice < 0)
    {
        imgSlice = img->getDepth() / 2;
    }
    else
    {
        imgSlice = slice;
    }

    if (frame < 0)
    {
        imgFrame = img->getFrameNr() / 2;
    }
    else
    {
        imgFrame = frame;
    }

    int result = img->read(imgFrame);
    if (result == 0)
    {
        imgData =
            new uint32_t[img->getDimx() * img->getDimy() * img->getDimz()];
        if (imgData)
        {
            imgBase = img;
            readImgData();

            frameScroll->setRange(0, imgBase->getFrameNr() - 1);
            frameScroll->setValue(imgFrame);

            imgLoaded = true;
        }
        else
        {
            result = -1;
        }
    }

    return result;
}

void Window::setOrientation(Img::Orientation o)
{
    if (imgLoaded)
    {
        imgBase->setOrientation(o);
        if (imgSlice >= imgBase->getDepth())
        {
            imgSlice = imgBase->getDepth() - 1;
        }
        readImgData();
    }
}

void Window::setLimits(float low, float high)
{
    if (imgLoaded)
    {
        imgBase->setLimits(low, high);
        readImgData();
    }
}

void Window::closeImg()
{
    if (imgLoaded)
    {
        imgLoaded = false;
        delete imgData;
        imgData = 0;
        imgBase->close();
        delete imgBase;
        imgBase = 0;
    }
}

int Window::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) &&imgLoaded)
    {
        readImgData();
    }

    return result;
}

