#include <QApplication>

#include <iostream>

#include "imgwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ImgWindow window;

    if (argc > 1)
    {
        if (window.loadImg(argv[1]) != 0)
        {
            std::cerr << "Error - Unable to open image "
                      << argv[1] << std::endl;
            return 1;
        }
    }

    if (argc > 2)
    {
        if (window.loadColormap(argv[2]) != 0)
        {
            std::cout << "Warning - Unable to open colormap "
                      << argv[2] << std::endl;
        }
    }
    window.show();

    return app.exec();
}

