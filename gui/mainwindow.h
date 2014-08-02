#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsItem>
#include "delegate.h"

#include "../main/supervisor/supervisor.hpp"
#include "gui_settings.hpp"
#include "../main/enums/object_enum.hpp"
#include "../main/helpers/clock.hpp"

namespace Ui {
	class MainWindow;
}

class Delegate; // to break circular dependency

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public slots:
	void update_ROI();

public:
    explicit MainWindow(Supervisor*, QWidget *parent = 0);
    ~MainWindow();

    friend class Delegate;

    bool is_paused();
    void start();
    void extract_window_info();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_frame_slider_valueChanged(int value);
    void on_zoom_slider_valueChanged(int value);
    void on_user_edit_fps_box_textChanged(const QString &arg1);
    void on_user_edit_fps_box_lostFocus();


private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsItem *scene_item;

    QStandardItemModel *settings_table;
    Delegate* settings_delegate;

    Supervisor* supervisor;

    QTimer* frame_timer;
    Clock frame_clk;

    bool lineEdit_userset;

    Map_symbol** symbol_buffer;
    QImage* frame_buffer;
    bool buffer_allocated;

    const QIcon pause_icon, play_icon, reset_icon;

    void resizeEvent ( QResizeEvent * event );

    bool paused;
    void unpause_game();
    void pause_game();

    void update_buffer();
    void get_window_size();
    void bound_zoom();
    void bound_fps(double);

    void paint_ROI();
    void colorize_ROI();

    void update_fps(int value);
};


#endif // MAINWINDOW_H
