#include <QApplication>

#include <iostream>

#ifndef VOL
#include "imgwindow.h"
#else
#include "volwindow.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifndef VOL
    ImgWindow window;
#else 
    VolWindow window;
#endif

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

