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
    QPolygon pol, polygonek;

    for (int i=0; i<points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
        pol.append(QPoint(points[i].x(), points[i].y()));
    }

    //Draw non-convex polygon
    qp.drawPolygon(polygon);

    //Draw triangulation
    for(Edge e : dt)
    {
        //Get start point, get end point
        QPoint3D s_point = e.getStart();
        QPoint3D e_point = e.getEnd();

        //Draw line
        qp.setPen(Qt::blue);
        qp.drawLine(s_point, e_point);
    }

    //Draw contour lines
    for (Edge c:contours)
    {
        //Get start point, get end point
        QPoint3D s_point = c.getStart();
        QPoint3D e_point = c.getEnd();
        QPen fill_pen(Qt::magenta, 1);
        qp.setPen(fill_pen);

        if ((int)s_point.getZ()%(dz*5)  == 0)
        {
            QPen fill_pen(Qt::magenta, 3);
            qp.setPen(fill_pen);
        }
        //Draw line
        qp.drawLine(s_point,e_point);
    }

    //Draw slope
    double slope_exposition;
    int red, green, blue;

    for (Triangle t:triangles)
    {
        //Get vertices of each triangle
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope
        if (sl_exp == true)
        {
            if (colorchanged == false)
            {
                double k = 255/M_PI_2;
                slope_exposition = t.getSlope();

                //Convert to color
                red = green = blue = 255 - k * slope_exposition;
            }

            else if (colorchanged == true)
            {
                slope_exposition = t.getSlope();
                double fraction = slope_exposition/(20*M_PI/180);

                if (slope_exposition > (20*M_PI/180))
                {
                    red = 255;
                    green = 255;
                    blue = 0;
                }
                else
                {
                red = 255;
                green = 255-(fraction*255);
                blue = 0;
                }
            }
        }

        else if (sl_exp == false)
        {
            if (colorchanged == false)
            {
                double k = 255/(2*M_PI);
                slope_exposition = t.getExposition();
                //Convert to color
                red = green = blue = 255 - k * slope_exposition;
            }
            else if (colorchanged == true)
            {
                slope_exposition = t.getExposition();
                std::cout << slope_exposition*180/M_PI << std::endl;

                slope_exposition *= 180/M_PI;
                if((slope_exposition >= 22.5) && (slope_exposition < 67.5))
                {
                    //Northeast - Orange
                    red = 245;
                    green = 192;
                    blue = 78;
                }

                if((slope_exposition >= 67.5) && (slope_exposition < 112.5))
                {
                    //East - Yellow
                    red = 245;
                    green = 232;
                    blue = 91;
                }

                else if((slope_exposition >= 112.5) && (slope_exposition < 157.5))
                {
                    //Southeast Green
                    red = 123;
                    green = 240;
                    blue = 84;
                }

                else if((slope_exposition >= 157.5) && (slope_exposition < 202.5))
                {
                    //South - Cyan
                    red = 81;
                    green = 244;
                    blue = 252;
                }

                else if((slope_exposition >= 202.5) && (slope_exposition < 247.5))
                {
                    //Southwest - Blue
                    red = 23;
                    green = 135;
                    blue = 227;
                }

                else if((slope_exposition >= 247.5) && (slope_exposition < 292.5))
                {
                    //West - Dark Blue
                    red = 8;
                    green = 53;
                    blue = 225;
                }

                else if((slope_exposition >= 292.5) && (slope_exposition < 337.5))
                {
                    //Northwest - Purple
                    red = 247;
                    green = 8;
                    blue = 255;
                }

                else if((slope_exposition > 337.5) || (slope_exposition < 22.5))
                {
                    //North - Red
                    red = 232;
                    green = 35;
                    blue = 35;
                }
            }
        }

        QColor color (red, green, blue,255);

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
    triangles.clear();
    polygon.clear();
    repaint();
}

void Draw::clearDT()
{
    dt.clear();
    triangles.clear();
    repaint();
}



void Draw::loadData(QString &file_name)
{
    //Load data from the *.txt file

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
        double temp = points[i].x();
        points[i].setX(-k*(points[i].y()-y_max));
        points[i].setY(k*(temp-x_min));
        }

    }
    inputFile.close();
}


void Draw::loadPolygon(QString &file_name)
{
    //Load data from the *.txt file
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
            double z = rand() % 1000;

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

        //Save polygon to the vector of QPoint3D
            points.push_back(point);
    }

    //Compute scales to zoom in in canvas
    double canvas_weight = 1031.0;
    double canvas_height = 777.0;

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
        double temp = points[i].x();
        points[i].setX(-k*(points[i].y()-y_max)+11);
        points[i].setY(k*(temp-x_min)+11);
        polygon.append(points[i]);
        }
    }

    inputFile.close();
}



