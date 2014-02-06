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
    
    QTimer* frame_timer;

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    Supervisor* supervisor;

    Map_symbol** symbol_buffer;
    QImage* frame_buffer;
    Pair<int> ROI_coors, ROI_dims;

    bool paused;
    void unpause_game();
    void pause_game();

    void colorize_ROI();
};


#endif // MAINWINDOW_H
