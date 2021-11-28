#ifndef QPOINT3D_H
#define QPOINT3D_H

#include <QtGui>


class QPoint3D : public QPointF
{
    private:
        double z;

    public:
        QPoint3D():QPointF(0.0, 0.0), z(0.0){}
        QPoint3D(double x_, double y_): QPointF(x_, y_), z(0.0){}
        QPoint3D(double x_, double y_, double z_):QPointF(x_, y_), z(z_){}

        double getZ(){return z;}
        void setZ(double z_){z = z_;}
};

#endif // QPOINT3D_H
