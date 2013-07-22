#include <QtGui>

#include <stdlib.h>
#include <iostream>
#include <string>

#include "volimg/imgloader.h"
#include "polytool.h"
#include "sampletool.h"
#include "imgwidget.h"
#include "imgwindow.h"

ImgWindow::ImgWindow()
    : imgParams(this),
      colorMap(),
      imgLoaded(false)
{
    workWidget = new QWidget;
    setCentralWidget(workWidget);

    selectedTool = new SampleTool(this);
    imgWidget = new ImgWidget(selectedTool, this);

    sliceScroll = new QScrollBar(Qt::Vertical);
    connect(sliceScroll, SIGNAL(valueChanged(int)), this, SLOT(setSlice(int)));
    connect(this, SIGNAL(sliceChanged(int)), sliceScroll, SLOT(setValue(int)));

    QHBoxLayout *horLayout = new QHBoxLayout;
    horLayout->addWidget(imgWidget);
    horLayout->addWidget(sliceScroll);

    frameScroll = new QScrollBar(Qt::Horizontal);
    connect(frameScroll, SIGNAL(valueChanged(int)), this, SLOT(setFrame(int)));
    connect(this, SIGNAL(frameChanged(int)), frameScroll, SLOT(setValue(int)));

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

    setWindowTitle(tr("Image window"));
}

void ImgWindow::imageOpen()
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

void ImgWindow::imageClose()
{
    if (imgLoaded)
    {
        closeImg();
        imgWidget->unsetData();
    }
}

void ImgWindow::imageHorizontal()
{
    imageHorizontalAct->setEnabled(false);
    imageSagittalAct->setEnabled(true);
    imageCoronalAct->setEnabled(true);
    setOrientation(Img::ORIENTATION_HORIZONTAL);
}

void ImgWindow::imageSagittal()
{
    imageHorizontalAct->setEnabled(true);
    imageSagittalAct->setEnabled(false);
    imageCoronalAct->setEnabled(true);
    setOrientation(Img::ORIENTATION_SAGITTAL);
}

void ImgWindow::imageCoronal()
{
    imageHorizontalAct->setEnabled(true);
    imageSagittalAct->setEnabled(true);
    imageCoronalAct->setEnabled(false);
    setOrientation(Img::ORIENTATION_CORONAL);
}

void ImgWindow::imageParams()
{
    imgParams.show();
}

void ImgWindow::imageColormap()
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

void ImgWindow::imageExit()
{
    exit(0);
}

void ImgWindow::toolsSelect()
{
    // TODO
    statusBar()->showMessage(tr("Click on object to select"));
}

void ImgWindow::toolsPolygon()
{
    delete selectedTool;
    selectedTool = new PolyTool;
    imgWidget->setTool(selectedTool);
    statusBar()->showMessage(tr("Draw polygon"));
}

void ImgWindow::toolsSample()
{
    delete selectedTool;
    selectedTool = new SampleTool(this);
    imgWidget->setTool(selectedTool);
    statusBar()->showMessage(tr("Select point to sample"));
}

void ImgWindow::createActions()
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

    imageParamsAct = new QAction(tr("&Parameters"), this);
    connect(imageParamsAct, SIGNAL(triggered()), this, SLOT(imageParams()));

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

void ImgWindow::createMenus()
{
    imageMenu = menuBar()->addMenu(tr("&Image"));
    imageMenu->addAction(imageOpenAct);
    imageMenu->addAction(imageCloseAct);
    imageMenu->addSeparator();
    imageMenu->addAction(imageHorizontalAct);
    imageMenu->addAction(imageSagittalAct);
    imageMenu->addAction(imageCoronalAct);
    imageMenu->addSeparator();
    imageMenu->addAction(imageParamsAct);
    imageMenu->addAction(imageColormapAct);
    imageMenu->addSeparator();
    imageMenu->addAction(imageExitAct);
}

void ImgWindow::createToolBars()
{
    toolsToolBar = addToolBar(tr("Tools"));
    toolsToolBar->addAction(toolsSelectAct);
    toolsToolBar->addAction(toolsPolygonAct);
    toolsToolBar->addAction(toolsSampleAct);
}

void ImgWindow::setSlice(int slice)
{
    if (slice != imgSlice)
    {
        imgSlice = slice;
        if (imgLoaded)
        {
            imgWidget->setData(
                imgBase->getWidth(), imgBase->getHeight(),
                &imgData[imgSlice * imgBase->getWidth() * imgBase->getHeight()]
                );
        }
    }
}

void ImgWindow::setFrame(int frame)
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

void ImgWindow::readImgData()
{
    imgBase->getData(imgData, &colorMap);
    sliceScroll->setRange(0, imgBase->getDepth() - 1);
    sliceScroll->setValue(imgSlice);
    imgWidget->setData(
        imgBase->getWidth(), imgBase->getHeight(),
        &imgData[imgSlice * imgBase->getWidth() * imgBase->getHeight()]
        );
}

int ImgWindow::loadImg(
    const char *fn,
    int slice,
    int frame
    )
{
    int result = 1;

    Img *img = ImgLoader::open(fn);
    if (img)
    {
        imageHorizontalAct->setEnabled(false);
        imageSagittalAct->setEnabled(true);
        imageCoronalAct->setEnabled(true);
        result = readImg(img, slice, frame);
    }

    return result;
}

int ImgWindow::readImg(
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
            new uint32_t[img->getWidth() * img->getHeight() * img->getDepth()];
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

void ImgWindow::setOrientation(Img::Orientation o)
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

void ImgWindow::setParams(float low, float high, bool trans)
{
    if (imgLoaded)
    {
        imgBase->setLimits(low, high);
        if (trans)
        {
            imgBase->setTransparency(Img::TRANSPARENCY_VOXEL);
        }
        else
        {
            imgBase->setTransparency(Img::TRANSPARENCY_NONE);
        }
        readImgData();
    }
}

void ImgWindow::closeImg()
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

int ImgWindow::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) && imgLoaded)
    {
        readImgData();
    }

    return result;
}

