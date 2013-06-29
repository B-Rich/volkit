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

    if (v->read(12) > 0)
    {
        std::cout << "Error - Unable to read frame 12" << std::endl;
        return 0;
    }

    Window window;
    window.setImg(v, 0.0, 65000.0, 60);
    window.show();

    return app.exec();
}

