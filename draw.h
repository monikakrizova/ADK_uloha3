#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpoint3d.h"
#include <QtGui>
#include "edge.h"
#include "triangle.h"
#include "iostream"


class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    QPolygonF pol;
    std::vector<Edge> dt;
    std::vector<Edge> contours;
    std::vector<Triangle> triangles;
    double y_max = 0.0, x_min = 999999999.0; //pro transformaci
    double y_min = 999999999.0, x_max = 0.0; //pro meritko
    double z_min = 999999999.0, z_max = 0.0; //pro souradnice
    int dz = 1;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    void loadData(QString &file_name);
    void loadPolygon(QString &file_name);
    std::vector<QPoint3D> getPoints(){return points;}
    double getZmin(){return z_min;}
    double getZmax(){return z_max;}
    void setDT(std::vector<Edge> &dt_){dt = dt_;}
    void setDZ(int &dz_){dz = dz_;}
    std::vector<Edge> getDT(){return dt;}
    void setContours(std::vector<Edge> &contours_){contours = contours_;}
    std::vector<Edge> getContours(){return contours;}
    std::vector<Triangle> getTriangles(){return triangles;}
    void setTriangles(std::vector<Triangle> &triangles_){triangles = triangles_;}
    bool sl_exp = false, colorchanged = false;
    void clearDT();

signals:

public slots:
};

#endif // DRAW_H
