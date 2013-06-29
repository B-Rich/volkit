#include <QApplication>

#include <iostream>

#include "window.h"
#include "v.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    V *v = new V;
    if (v->open(argv[1]) > 0)
    {
        std::cout << "Error - Unable to open image " << argv[1] << std::endl;
        return 1;
    }

    Window window;
    int frame = v->getFrameNr() / 2;
    if (window.readImg(v, 0.0, 65000.0, v->getDimz() / 2, frame) != 0)
    {
        std::cout << "Error - Unable to read frame " << frame << std::endl;
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

