#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>
#include "edge.h"
#include "qpoint3d.h"
#include "triangle.h"
#include "draw.h"
#include "ui_widget.h"


class Algorithms
{
public:
    Algorithms();
    int getPointLinePosition(QPoint3D &a,QPoint3D &p1,QPoint3D &p2);
    std::tuple<QPoint3D,double> getCircleCenterAndRadius(QPoint3D &p1,QPoint3D &p2,QPoint3D &p3);
    int getDelaunayPoint(QPoint3D &s,QPoint3D &e,std::vector<QPoint3D> &points);
    int getNearestPoint(QPoint3D &p, std::vector<QPoint3D> &points);
    std::vector<Edge> dT(std::vector<QPoint3D> &points);
    std::vector<Edge> dTPolygon(std::vector<QPoint3D> &points);
    void updateAEL(Edge &e, std::list<Edge> &ael);
    QPoint3D getContourPoint(QPoint3D &p1, QPoint3D &p2, double z);
    std::vector<Edge> getContourLines(std::vector<Edge> &dt, double zmin, double zmax, double dz);
    double getSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    double getExposition(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    std::vector<Triangle> analyzeDTM(std::vector<Edge> &dt);
    QPoint3D getCentreOfMass(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    int getPositionRayCrossing(QPoint3D &q, std::vector<QPoint3D> &pol);
    double get2LinesAngle(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &p4);
    int getPositionWinding(QPoint3D &q, std::vector<QPoint3D> &pol);
    std::tuple<std::vector<QPoint3D>,std::vector<double>> calculateLabelPoints(std::vector<Edge> &main_contours);
};

#endif // ALGORITHMS_H
