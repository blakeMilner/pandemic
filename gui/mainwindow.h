#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QTreeWidget>

#include <list>

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

    void on_grid_size_spinBox_valueChanged(int arg1);
    void on_world_size_spinBox_X_valueChanged(int arg1);
    void on_world_size_spinBox_Y_valueChanged(int arg1);

private:
    bool pan_east_pressed;
    bool started;

    QTimer* pan_timer;
    Nav_symbol pan_direction;

    Ui::MainWindow *ui;
    QGraphicsScene* mainScene;
    QGraphicsScene* regionScene;
    QGraphicsItem *mainScene_item;
    QGraphicsItem *regionScene_item;
    Pair<int> regionView_dims;
    QImage* regionView_original_buffer;

    Supervisor* supervisor;

    QTimer* frame_timer;
    Clock frame_clk;

    bool user_edit_fps_box;

    Map_symbol** symbol_buffer;
    QImage* frame_buffer;
    QImage* regionView_buffer;
    bool buffer_allocated;

    const QIcon pause_icon, play_icon, reset_icon, next_icon;

    void resizeEvent ( QResizeEvent * event );

    void add_settings_categories();
    void addRoot(QString name, QString Description);
    void addChild(QTreeWidgetItem* parent, QString name, QString Description);
    list<QTreeWidgetItem*> stats_items;

    void update_map_settings();
    void update_statistics();

    bool paused;
    void unpause_game();
    void pause_game();

    void update_buffer();
    void get_window_size();
    void bound_zoom();
    void bound_fps(double);

    void shift_ROI_in();
    void paint_ROI();
    void colorize_ROI();
    void paint_regionView();

    void update_fps(int value);
    void update_regionScene();
};


#endif // MAINWINDOW_H
