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
    dz = 1.0;

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
    //Set dz
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_pushButton_3_clicked()
{
    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT
    Algorithms a;
    std::vector<Edge> dt = a.dT(points);

    //Set DT
    ui->Canvas->setDT(dt);

    //Create contours
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
    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT
    Algorithms a;
    std::vector<Edge> dt = a.dT(points);

    //Draw slope
    if (ui->comboBox->currentIndex() == 0)
        ui->Canvas->sl_exp = true;
    else if (ui->comboBox->currentIndex() == 1)
        ui->Canvas->sl_exp = false;

    //Draw slope
    if (ui->comboBox_2->currentIndex() == 0)
        ui->Canvas->colorchanged = false;
    else if (ui->comboBox_2->currentIndex() == 1)
        ui->Canvas->colorchanged = true;

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        Algorithms a;
        //Analyze DTM
        std::vector<Triangle> triangles = a.analyzeDTM(dt);

        //Set triangles
        ui->Canvas->setTriangles(triangles);
    }
    repaint();
}

void Widget::on_pushButton_5_clicked()
{
    //Open text file with dialog
    //QString file_name = "D:/skola_ING/semestr3/ADKaGIS/kladivova_spererova_adk-master/U3_kladivova_spererova/U3_test_data/test.txt";
    QString file_name = "D:/Github/ADK/ADK_uloha3/e1.txt";
    //QString file_name = QFileDialog::getOpenFileName(this, tr("Open Text file"), "", tr("Text Files (*.txt)"));
    ui->Canvas->loadData(file_name);
    repaint();
}



void Widget::on_pushButton_contour_labels_clicked()
{
    std::vector<Edge> main_contours = ui->Canvas->getMainContours();

}

void Widget::on_checkBox_stateChanged(int arg1)
{
    if (ui->checkBox->isChecked())
        ui->Canvas->labels = true;
    else
        ui->Canvas->labels = false;
}

