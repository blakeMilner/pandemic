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
#include "../main/settings/map_settings.hpp"
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
    void on_play_button_clicked();
    void on_reset_button_clicked();
    void on_next_button_clicked();

    void on_frame_slider_valueChanged(int value);
    void on_zoom_slider_valueChanged(int value);

    void on_user_edit_fps_box_textChanged(const QString &arg1);
    void on_user_edit_fps_box_lostFocus();

    void adjust_ROI(Nav_symbol dir, int pps);
    void update_pan();

    void on_pan_east_clicked();
    void on_pan_west_clicked();
    void on_pan_north_clicked();
    void on_pan_south_clicked();
    void on_pan_southwest_clicked();
    void on_pan_northwest_clicked();
    void on_pan_northeast_clicked();
    void on_pan_southeast_clicked();

    void on_pan_east_pressed();
    void on_pan_east_released();
    void on_pan_west_pressed();
    void on_pan_west_released();
    void on_pan_north_pressed();
    void on_pan_north_released();
    void on_pan_southwest_pressed();
    void on_pan_southwest_released();
    void on_pan_northwest_pressed();
    void on_pan_northwest_released();
    void on_pan_northeast_pressed();
    void on_pan_northeast_released();
    void on_pan_southeast_pressed();
    void on_pan_southeast_released();
    void on_pan_south_pressed();
    void on_pan_south_released();

private:
    bool pan_east_pressed;

    QTimer* pan_timer;
    Nav_symbol pan_direction;




    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsItem *scene_item;

    QStandardItemModel *settings_table;
    Delegate* settings_delegate;

    Supervisor* supervisor;

    QTimer* frame_timer;
    Clock frame_clk;

    bool user_edit_fps_box;

    Map_symbol** symbol_buffer;
    QImage* frame_buffer;
    bool buffer_allocated;

    const QIcon pause_icon, play_icon, reset_icon, next_icon;

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
