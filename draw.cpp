#include "draw.h"
#include <stdlib.h>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Initialize random number generator
    srand (time(NULL));
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.begin(this);

    //Draw points
    int r=4;
    QPolygon pol;

    for (int i=0; i<points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
        pol.append(QPoint(points[i].x(), points[i].y()));
    }

    //Draw triangulation
    for(Edge e : dt)
    {
        //Get start point, get end point
        QPoint3D s_point = e.getStart();
        QPoint3D e_point = e.getEnd();

        //Draw line
        qp.drawLine(s_point, e_point);
    }

    //Draw contour lines
    for (Edge c:contours)
    {
        //Get start point, get end point
        QPoint3D s_point = c.getStart();
        QPoint3D e_point = c.getEnd();
        qp.setPen(Qt::magenta);

        //Draw line
        qp.drawLine(s_point,e_point);
    }

    //Draw slope
    double slope_exposition;
    int col;
    QColor color;

    for (Triangle t:triangles)
    {
        //Get vertices of each triangle
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope
        if (sl_exp == true)
        {
            double k = 255/M_PI_2;
            slope_exposition = t.getSlope();

            //Convert to color
            col = 255 - k * slope_exposition;
        }

        else if (sl_exp == false)
        {
            double k = 255/(2*M_PI);
            slope_exposition = t.getExposition();
            //Convert to color
            col = 255 - k * slope_exposition;
        }

        std::cout << "angle: " << slope_exposition << ", color: " << col << std::endl;

        QColor color(col, col, col);

        //Set pen and brush
        qp.setBrush(color);
        qp.setPen(color);

        //Create polygon for triangle
        QPolygon pol;
        pol.push_back(QPoint(p1.x(), p1.y()));
        pol.push_back(QPoint(p2.x(), p2.y()));
        pol.push_back(QPoint(p3.x(), p3.y()));

        //Draw triangle
        qp.drawPolygon(pol);
    }

    //Stop drawing
    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event->pos().x();
    int y = event->pos().y();
    int z = rand() % 1000;

    //Create point
    QPoint3D p(x, y, z);

    //Add point to the vector
    points.push_back(p);

    //Update screen
    repaint();
}

void Draw::clear()
{
    points.clear();
    dt.clear();
    contours.clear();
    repaint();
}

void Draw::clearDT()
{
    dt.clear();
    repaint();
}

void Draw::loadData(QString &file_name)
{
    //Load data from the *.txt file
    QPolygonF polygon;
    std::vector<QPolygonF> buildings_;

    QPoint3D point;

    QFile inputFile(file_name);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            int id = line.split(" ")[0].toInt();
            double y = line.split(" ")[1].toDouble();
            double x = line.split(" ")[2].toDouble();
            double z = line.split(" ")[3].toDouble();


            //Add vertice to the end of the QPoint3D vector
            point.setX(x);
            point.setY(y);
            point.setZ(z);

            if (y > y_max)
                y_max = y;
            else if (y < y_min)
                y_min = y;
            if (x < x_min)
                x_min = x;
            else if (x > x_max)
                x_max = x;
            if (z < z_min)
                z_min = z;
            else if (z > z_max)
                z_max = z;

        //Save polygon to the vector of QPolygonFs
        points.push_back(point);
    }

    //Compute scales to zoom in in canvas
    double canvas_weight = 952.0;
    double canvas_height = 748.0;

    double dy = fabs(y_max-y_min);
    double dx = fabs(x_max-x_min);

    double k;
    if (dy > dx)
        k = canvas_weight/dy;
    else
        k = canvas_height/dx;

    //Transform coordinates from JTSK to canvas
    for (int unsigned i = 0; i < points.size(); i++)
        {
        QPoint3D pol = points[i];

        double temp = points[i].x();
        points[i].setX(-k*(points[i].y()-y_max));
        points[i].setY(k*(temp-x_min));
        }

    }
    inputFile.close();
}
