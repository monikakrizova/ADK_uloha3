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
            if (slope_param == 0)
            {
                double k = 255/M_PI_2;
                slope_exposition = t.getSlope();

                //Convert to color
                red = green = blue = 255 - k * slope_exposition;
            }

            else if (slope_param == 1)
            {
                slope_exposition = t.getSlope();
                double fraction = slope_exposition/(20*M_PI/180);

                if (slope_exposition < (20*M_PI/180))
                {
                    red = fraction*255;
                    green = 255-fraction*255;
                    blue = 0;
                }
                else
                {
                    red = 255;
                    green = 0;
                    blue = 0;
                }
            }
            else if (slope_param == 2)
            {
                slope_exposition = t.getSlope();
                double fraction = slope_exposition/(20*M_PI/180);

                if (slope_exposition >= (20*M_PI/180))
                {
                    red = 255;
                    green = 0;
                    blue = 0;
                }
                else
                {
                    red = 255;
                    green = 255-fraction*255;
                    blue = 0;
                }
            }
            else if (slope_param == 3)
            {
                slope_exposition = t.getSlope();
                double fraction = slope_exposition/(20*M_PI/180);
                if (slope_exposition < (20*M_PI/180))
                {
                    red = 0;
                    green = 255-fraction*255;
                    blue = 255;
                }
                else
                {
                    red = 0;
                    green = 0;
                    blue = 255;
                }
            }
        }

        else if (sl_exp == false)
        {
            if (expos_param == 0)
            {
                double k = 255/(2*M_PI);
                slope_exposition = t.getExposition();
                //Convert to color
                red = green = blue = 255 - k * slope_exposition;
            }
            else if (expos_param == 1)
            {
                slope_exposition = t.getExposition();

                slope_exposition *= 180/M_PI;
                if((slope_exposition >= 22.5) && (slope_exposition < 67.5))
                {
                    //Northeast - Orange
                    red = 255;
                    green = 127;
                    blue = 0;
                }

                if((slope_exposition >= 67.5) && (slope_exposition < 112.5))
                {
                    //East - Yellow
                    red = 255;
                    green = 255;
                    blue = 0;
                }

                else if((slope_exposition >= 112.5) && (slope_exposition < 157.5))
                {
                    //Southeast Green
                    red = 0;
                    green = 255;
                    blue = 0;
                }

                else if((slope_exposition >= 157.5) && (slope_exposition < 202.5))
                {
                    //South - Cyan
                    red = 0;
                    green = 255;
                    blue = 255;
                }

                else if((slope_exposition >= 202.5) && (slope_exposition < 247.5))
                {
                    //Southwest - Blue
                    red = 0;
                    green = 127;
                    blue = 255;
                }

                else if((slope_exposition >= 247.5) && (slope_exposition < 292.5))
                {
                    //West - Dark Blue
                    red = 0;
                    green = 0;
                    blue = 255;
                }

                else if((slope_exposition >= 292.5) && (slope_exposition < 337.5))
                {
                    //Northwest - Purple
                    red = 255;
                    green = 0;
                    blue = 255;
                }

                else if((slope_exposition > 337.5) || (slope_exposition < 22.5))
                {
                    //North - Red
                    red = 255;
                    green = 0;
                    blue = 0;
                }
            }
            else if (expos_param == 2)
            {
                slope_exposition = t.getExposition();

                slope_exposition *= 180/M_PI;
                if ((slope_exposition >= 0) && (slope_exposition < 45))
                {
                    //Northeast - Orange
                    double fraction = slope_exposition/(45);
                    red = 255;
                    green = 0;
                    blue = 127*fraction;
                }

                else if((slope_exposition >= 45) && (slope_exposition < 90))
                {
                    //Orange to yellow
                    slope_exposition -= 45;
                    double fraction = slope_exposition/(45);
                    red = 255;
                    green = 127+fraction*127;
                    blue = 0;
                }

                else if((slope_exposition >= 90) && (slope_exposition < 135))
                {
                    //Yellow to green
                    slope_exposition -= 90;
                    double fraction = slope_exposition/(45);
                    red = 255-fraction*127;
                    green = 255;
                    blue = 0;
                }

                else if((slope_exposition >= 135) && (slope_exposition < 180))
                {
                    //Green to Cyan
                    slope_exposition -= 135;
                    double fraction = slope_exposition/(45);
                    red = 0;
                    green = 255;
                    blue = fraction*255;
                }

                else if((slope_exposition >= 180) && (slope_exposition < 225))
                {
                    //Cyan to ligth blue
                    slope_exposition -= 180;
                    double fraction = slope_exposition/(45);
                    red = 0;
                    green = 255-fraction*127;
                    blue = 255;
                }

                else if((slope_exposition >= 255) && (slope_exposition < 270))
                {
                    //Light Blue to Dark Blue
                    slope_exposition -= 255;
                    double fraction = slope_exposition/(45);
                    red = 0;
                    green = 127-fraction*127;
                    blue = 255;
                }

                else if((slope_exposition >= 270) && (slope_exposition < 315))
                {
                    //Blue to Purple
                    slope_exposition -= 270;
                    double fraction = slope_exposition/(45);
                    red = fraction*255;
                    green = 0;
                    blue = 255;
                }

                else if (slope_exposition > 315)
                {
                    //Purple to Red
                    slope_exposition -= 315;
                    double fraction = slope_exposition/(45);
                    red = 255;
                    green = 0;
                    blue = 255-255*fraction;
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
    double canvas_weight = 872.0;
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
            double z = rand() % 100;

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
    double canvas_weight = 872;
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



