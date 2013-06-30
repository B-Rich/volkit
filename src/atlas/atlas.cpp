#include <QApplication>

#include <iostream>

#include "window.h"
#include "v.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc <= 1)
    {
        std::cerr << "Usage is: " << argv[0]
                  << " <filename> [colormap]" << std::endl;
        return 1;
    }

    Window window;
    if (window.loadImg(argv[1], 0.0, 65000.0) != 0)
    {
        std::cerr << "Error - Unable to open image " << argv[1] << std::endl;
        return 1;
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

