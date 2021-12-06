#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    private:
        double dz;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_ClearAll_clicked();

    void on_pushButton_CreateDT_clicked();

    void on_pushButton_cleardt_clicked();

    void on_lineEdit_3_editingFinished();

    void on_pushButton_CreateContours_clicked();

    void on_pushButton_AnalyzeDTM_clicked();

    void on_pushButton_LoadPoints_clicked();

    void on_pushButton_LoadPolygon_clicked();

    void on_pushButton_CreatePolDT_clicked();

    void on_pushButton_contour_labels_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_ClearPoints_clicked();

    void on_checkBox_ShowContours_stateChanged(int arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
