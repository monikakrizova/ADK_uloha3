#include "algorithms.h"
#include "sortbyx.h"
#include <list>
#include <cmath>

Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPoint3D &a,QPoint3D &p1,QPoint3D &p2)
{
    //Analyze point and line position
    double eps = 1.0e-10;

    //Coordinate differences
    double ux=p2.x()-p1.x();
    double uy=p2.y()-p1.y();

    double vx=a.x()-p1.x();
    double vy=a.y()-p1.y();

    //Half plane test(cross product)
    double t = ux*vy-vx*uy;

    //Point in the left halfplane
    if (t > eps)
        return 1;

    //Point in the right halfplane
    if (t < -eps)
        return 0;

    //Point on the line
    return -1;
}

std::tuple<QPoint3D,double>Algorithms::getCircleCenterAndRadius(QPoint3D &p1,QPoint3D &p2,QPoint3D &p3)
{
    //Return center and radius of inscribed circle
    double k1 = p1.x() * p1.x() + p1.y() * p1.y();
    double k2 = p2.x() * p2.x() + p2.y() * p2.y();
    double k3 = p3.x() * p3.x() + p3.y() * p3.y();
    double k4 = p1.y() - p2.y();
    double k5 = p1.y() - p3.y();
    double k6 = p2.y() - p3.y();
    double k7 = p1.x() - p2.x();
    double k8 = p1.x() - p3.x();
    double k9 = p2.x() - p3.x();
    double k10 = p1.x() * p1.x();
    double k11 = p2.x() * p2.x();
    double k12 = p3.x() * p3.x();

    //Center of inscribed circle
    double m = 0.5 *(-k12 * k4 + k11 * k5 - (k10 + k4 * k5) * k6)/(-p3.x() * k4 + p2.x() * k5 - p1.x() * k6);
    double n = 0.5 * (-k1 * k9  + k2 * k8 - k3 * k7)/(-p1.y() * k9  + p2.y()  * k8 - p3.y()  * k7);
    double r = sqrt((p1.x() - m) * (p1.x() - m) + (p1.y() - n) * (p1.y() - n));

    //Create center
    QPoint3D c(m,n);

    return {c,r};
}

int Algorithms::getDelaunayPoint(QPoint3D &s,QPoint3D &e,std::vector<QPoint3D> &points)
{
    //Find and return Delaunay point
    int min_index = -1;
    double min_r = 1.e16;

    for (int i=0; i<points.size(); i++)
    {
        //Point is different from s,e
        if((points[i]!=s) && (points[i]!=e))
        {
            // Point in left halfplane
            if(getPointLinePosition(points[i],s,e)==1)
            {
                //Center and radius of inscribed circle
                auto[c,r] = getCircleCenterAndRadius(points[i],s,e);

                //Correct radius
                if(getPointLinePosition(c,s,e)==0)
                    r = -r;

                //Update minimum radius
                if (r < min_r)
                {
                   min_r = r;
                   min_index = i;
                }
            }
        }
    }

    return min_index;
}

int Algorithms::getNearestPoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    //Find nearest point
    double min_dist = 1.e16;
    int min_ind = -1;

    for (int i = 0; i < points.size(); i++)
    {
        //Point is different from p
        if((points[i]!=p))
        {
            double dist = sqrt((points[i].x()-p.x())*(points[i].x()-p.x())+(points[i].y()-p.y())*(points[i].y()-p.y()));

            //Update minimum
            if (dist < min_dist)
            {
                min_dist = dist;
                min_ind = i;
            }
        }
    }

    return min_ind;
}

std::vector<Edge> Algorithms::dT(std::vector<QPoint3D> &points)
{
    //Create Delaunay triangulation using incremental method
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Find pivot (minimum x)
    QPoint3D q = *min_element(points.begin(), points.end(), sortByX());

    //Point nearest to pivot q
    int i_nearest = getNearestPoint(q, points);
    QPoint3D qn = points[i_nearest];

    //Create new edge
    Edge e(q, qn);

    //Find optimal Delaunay point
    int i_point = getDelaunayPoint(q, qn, points);

    //Point has not been found, change orientation, search again
    if (i_point == -1)
    {
        e.changeOrientation();
        i_point = getDelaunayPoint(qn, q, points);
    }

    //Delaunay point + 3rd vertex
    QPoint3D v3 = points[i_point];

    //Create edges
    QPoint3D es = e.getStart();
    QPoint3D ee = e.getEnd();
    Edge e2(ee, v3);
    Edge e3(v3, es);

    //Adding 3 edges to DT
    dt.push_back(e);
    dt.push_back(e2);
    dt.push_back(e3);

    //Adding 3 edges to AEL
    ael.push_back(e);
    ael.push_back(e2);
    ael.push_back(e3);

    //Proces edges until AEL is empty
    while(!ael.empty())
    {
        //get last edge
        e = ael.back();
        ael.pop_back();

        //Change orientation
        e.changeOrientation();

        //Find optimal Delaunay point
        QPoint3D qs = e.getStart();
        QPoint3D qe = e.getEnd();
        i_point = getDelaunayPoint(qs, qe, points);

        //Point has been found
        if (i_point != -1)
        {
            //Delaunay point + 3rd vertex
            v3 = points[i_point];

            //Create edges
            es = e.getStart();
            ee = e.getEnd();
            Edge e2(ee, v3);
            Edge e3(v3, es);

            //Add 3 edges to DT
            dt.push_back(e);
            dt.push_back(e2);
            dt.push_back(e3);

            //Update AEL
            updateAEL(e2, ael);
            updateAEL(e3, ael);
        }
    }

    return dt;
}

void Algorithms::updateAEL(Edge &e, std::list<Edge> &ael)
{
    //Update AEL

    //Switch orientation
    e.changeOrientation();

    //Look for e3 in AEL
    auto ie = std::find(ael.begin(),ael.end(),e);

    //E2 has not been found, add to the list
    if(ie == ael.end())
    {
        e.changeOrientation();
        ael.push_back(e);
    }

    //Erase from ael
    else
        ael.erase(ie);
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Get countour point intersection of triangle and horizontal countour
    double xb = (p2.x()-p1.x())/(p2.getZ()-p1.getZ())*(z-p1.getZ())+p1.x();
    double yb = (p2.y()-p1.y())/(p2.getZ()-p1.getZ())*(z-p1.getZ())+p1.y();

    return QPoint3D(xb, yb, z);
}

std::vector<Edge> Algorithms::getContourLines(std::vector<Edge> &dt, double zmin, double zmax, double dz)
{
    //Get countour lines from delaunay triangulation
    std::vector<Edge> contours;

    Draw d;

    //Process all triangles of dt
    for (int i = 0; i < dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Get height of points
        double z1 = p1.getZ();
        double z2 = p2.getZ();
        double z3 = p3.getZ();

        //Check all horizontal planes
        for (double z = zmin; z <= zmax; z+=dz)
        {
            //Height differences
            double dz1 = z1 - z;
            double dz2 = z2 - z;
            double dz3 = z3 - z;

            //Edge intersected by plane?
            double dz12 = dz1*dz2;
            double dz23 = dz2*dz3;
            double dz31 = dz3*dz1;

            //Triangle is complanar
            if ((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge p1-p2 is collinear
            else if ((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //Edge p2-p3 is collinear
            else if ((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //Edge p3-p1 is collinear
            else if ((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Plane intersects edges p1-p2 and p2-p3
            else if (((dz12 <= 0) && (dz23 < 0)) || ((dz12 < 0) && (dz23 <= 0)))
            {
                //Compute intersections
                QPoint3D A = getContourPoint(p1, p2, z);
                QPoint3D B = getContourPoint(p2, p3, z);

                //Create edge and add it to the list
                Edge ab(A, B);
                contours.push_back(ab);
            }

            //Plane intersects edges p2-p3 and p3-p1
            else if (((dz23 <= 0) && (dz31 < 0)) || ((dz23 < 0) && (dz31 <= 0)))
            {
                //Compute intersections
                QPoint3D A = getContourPoint(p2, p3, z);
                QPoint3D B = getContourPoint(p3, p1, z);

                //Create edge and add it to the list
                Edge ab(A, B);
                contours.push_back(ab);
            }

            //Plane intersects edges p3-p1 and p1-p2
            else if (((dz31 <= 0) && (dz12 < 0)) || ((dz31 < 0) && (dz12 <= 0)))
            {
                //Compute intersections
                QPoint3D A = getContourPoint(p3, p1, z);
                QPoint3D B = getContourPoint(p1, p2, z);

                //Create edge and add it to the list
                Edge ab(A, B);
                contours.push_back(ab);
            }
        }
    }

    return contours;
}

double Algorithms::getSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute slope of triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p2.x();
    double vy = p3.y() - p2.y();
    double vz = p3.getZ() - p2.getZ();

    //Normal vector
    double nx = uy*vz - vy*uz;
    double ny = -ux*vz + vx*uz;
    double nz = ux*vy - vx*uy;

    //Norm
    double n = sqrt(nx*nx + ny*ny + nz*nz);

    return acos(nz/n);
}

double Algorithms:: getExposition(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute exposition (direction of the projected normal vector)
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p2.x();
    double vy = p3.y() - p2.y();
    double vz = p3.getZ() - p2.getZ();

    //Normal vector
    double nx = uy*vz - vy*uz;
    double ny = -ux*vz + vx*uz;
    double nz = ux*vy - vx*uy;

    double dir = atan2(nx, ny);

    if (dir < 0)
        dir += M_PI;
    //Direction of the vector
    return dir;
}

std::vector<Triangle> Algorithms::analyzeDTM(std::vector<Edge> &dt)
{
    //Computing slope and exposition for each triangle
    std::vector<Triangle> triangles;
    for (int i = 0; i < dt.size(); i+=3)
    {
        //Get triangle edges
        Edge e1 = dt[i];
        Edge e2 = dt[i+1];

        //Get triangle vertices
        QPoint3D p1 = e1.getStart();
        QPoint3D p2 = e1.getEnd();
        QPoint3D p3 = e2.getEnd();

        //Compute slope and exposition
        double slope = getSlope(p1, p2, p3);
        double exposition = getExposition(p1, p2, p3);

        //Create triangle
        Triangle t(p1, p2, p3, slope, exposition);

        //Add triangle to the list
        triangles.push_back(t);
    }

    return triangles;
}
