#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "edge.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //zmin = 100.0;
    //zmax = 1000.0;
    dz = 5.0;

    ui->lineEdit_3->setText(QString::number(dz));
}


Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    //Clear all
    ui->Canvas->clear();
    repaint();
}

void Widget::on_pushButton_clicked()
{
    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT
    Algorithms a;
    std::vector<Edge> dt = a.dT(points);

    //Set DT
    ui->Canvas->setDT(dt);

    repaint();

}

void Widget::on_pushButton_cleardt_clicked()
{
    //Clear DT
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_lineEdit_3_editingFinished()
{
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_pushButton_3_clicked()
{
    //Create contours
    std::vector<Edge> dt = ui->Canvas->getDT();
    double zmin = ui->Canvas->getZmin();
    double zmax = ui->Canvas->getZmax();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        Algorithms a;
        //Create contours
        std::vector<Edge> contours = a.getContourLines(dt, zmin, zmax, dz);

        //Set contours
        ui->Canvas->setContours(contours);

        repaint();
    }

}


void Widget::on_pushButton_4_clicked()
{
    //Draw slope
    std::vector<Edge> dt = ui->Canvas->getDT();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        Algorithms a;
        //Analyze DTM
        std::vector<Triangle> triangles = a.analyzeDTM(dt);

        //Set triangles
        ui->Canvas->setTriangles(triangles);

        if (ui->comboBox->currentIndex() == 0)
            ui->Canvas->sl_exp = true;

        repaint();
    }
}

void Widget::on_pushButton_5_clicked()
{
    //Open text file with dialog
    //QString file_name = "C:/Users/monik/OneDrive/Documents/GitHub/ADK_uloha3/ADK_uloha3/souradnicePB.txt";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Text file"), "", tr("Text Files (*.txt)"));
    ui->Canvas->loadData(file_name);
    repaint();
}


