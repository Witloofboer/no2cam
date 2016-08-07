#include "MockCamera.h"

#include <QColor>
#include <QImage>

MockCamera::MockCamera()
    : _index(0)
{
    QString imageId(":/images/camera%1.jpg");

    for(int img=0; img<3; ++img)
    {
        QImage image(imageId.arg(img+1));

        for (int i=0; i<2048; ++i)
            for (int j=0; j<2048; ++j)
                {
                    QColor c(image.pixel(i, j));
                    _images[img][0][i][j] = c.redF();
                    _images[img][1][i][j] = c.greenF();
                    _images[img][2][i][j] = c.blueF();
                }
    }
}

