#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "../main/supervisor/supervisor.hpp"
#include "gui_settings.hpp"
#include "../main/enums/object_enum.hpp"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public slots:
	void update_ROI();

public:
    explicit MainWindow(Supervisor*, QWidget *parent = 0);
    ~MainWindow();

    bool is_paused();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_lostFocus();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    Supervisor* supervisor;

    QTimer* frame_timer;

    Map_symbol** symbol_buffer;
    QImage* frame_buffer;

    bool paused;
    void unpause_game();
    void pause_game();

    void colorize_ROI();

    void update_fps(int value);
};


#endif // MAINWINDOW_H
