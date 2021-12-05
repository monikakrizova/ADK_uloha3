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
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_cleardt_clicked();

    void on_lineEdit_3_editingFinished();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_contour_labels_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_ClearPoints_clicked();

    void on_checkBox_ShowContours_stateChanged(int arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
