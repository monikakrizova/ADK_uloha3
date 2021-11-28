#ifndef EDGE_H
#define EDGE_H

#include <QtGui>
#include "qpoint3d.h"

class Edge
{
private:
    QPoint3D s,e;

public:
    Edge(QPoint3D &s_, QPoint3D &e_):s(s_), e(e_){}
    void changeOrientation(){QPoint3D temp=s; s=e; e=temp;}
    QPoint3D getStart() const {return s;}
    QPoint3D getEnd() const {return e;}

    bool operator == (const Edge &e2)
    {
        return (s==e2.getStart()) && (e == e2.getEnd());
    }

};

#endif // EDGE_H
