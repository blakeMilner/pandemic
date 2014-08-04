#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>

namespace GS = GUI_settings;

// map lenghts are skewed in places


// TODO: change linedit back to fps


MainWindow::MainWindow(Supervisor* s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),

    settings_table(new QStandardItemModel(10,1,this)),
    settings_delegate(new Delegate(this)),

    supervisor(s),

    paused(true),
    frame_timer(new QTimer(this)),
    frame_clk(Clock()),

    pan_timer(new QTimer(this)),
    pan_direction(EAST),

    reset_icon(QIcon(GS::reset_icon_path)),
    pause_icon(QIcon(GS::pause_icon_path)),
    play_icon(QIcon(GS::play_icon_path)),
    next_icon(QIcon(GS::next_icon_path)),

    user_edit_fps_box(true),
    scene_item(0)
{
    frame_buffer = new QImage(GS::MAX_ROI_DIMS.x, GS::MAX_ROI_DIMS.y, QImage::Format_RGB32);

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    // set these so that when we go from larger FOV to smaller, the view doesn't have scrollbars
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // connect timer in order to update ROI continuously
    connect(frame_timer, SIGNAL(timeout()), this, SLOT(update_ROI()));
    // connect timer in order to time panning movements
    connect(pan_timer, SIGNAL(timeout()), this, SLOT(update_pan()));

    // make buffer for raw symbols from map
	symbol_buffer = new Map_symbol*[GS::MAX_ROI_DIMS.x];
	for(int i = 0; i < GS::MAX_ROI_DIMS.x; i++){
		symbol_buffer[i] = new Map_symbol[GS::MAX_ROI_DIMS.y];
    }

    // initialize button icons
    ui->play_button->setIcon(play_icon);
    ui->play_button->setIconSize(QSize(25,25));
    ui->reset_button->setIcon(reset_icon);
    ui->reset_button->setIconSize(QSize(25,25));
    ui->next_button->setIcon(next_icon);
    ui->next_button->setIconSize(QSize(25,25));

    // set zoom slider step - value indicates pixels per symbol
    ui->zoom_slider->setMinimum(GS::MIN_pix_per_symbol);
    ui->zoom_slider->setMaximum(GS::MAX_pix_per_symbol);

    // set fps slider step - value indicates framerate (fps)
    ui->frame_slider->setMinimum(GS::MIN_FPS);
    ui->frame_slider->setMaximum(GS::MAX_FPS);
    ui->user_edit_fps_box->setValidator(new QIntValidator(GS::MIN_FPS, GS::MAX_FPS, this)); // only accept numbers
    ui->user_edit_fps_box->setText(QString::number(GS::fps)); // set initial fps in text box

    // initialize settings table
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 2; col++){
            QModelIndex index = settings_table->index(row,col,QModelIndex());
            settings_table->setData(index, 0);
        }
    }

    ui->tableView->setModel(settings_table);
    ui->tableView->setItemDelegate(settings_delegate);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete frame_buffer;

	for(int i = 0; i < GS::MAX_ROI_DIMS.x; i++){
		delete symbol_buffer[i];
	}
	delete symbol_buffer;
}

// show window, make buffer, and initialize screen
void MainWindow::start(){
    setVisible(true);

    get_window_size();
    bound_zoom();

    paint_ROI();
}

void MainWindow::get_window_size(){
    // minus 2 to fit within margins
    GS::ROI_dims.x = ui->graphicsView->width() - 2;
    GS::ROI_dims.y = ui->graphicsView->height() - 2;
}

// correct zoom level such that every sized map would cover the monitor
void MainWindow::bound_zoom(){
    while((GS::pps <= GS::MAX_pix_per_symbol)
          and (GS::ROI_dims / GS::pps).not_within(Map_settings::map_len - 2))
    {
        GS::pps++;
    }

    ui->zoom_slider->setValue(GS::pps);
}

// correct fps slider if we're capping out
void MainWindow::bound_fps(double frm_time){
    if(GS::fps > 1 / frm_time){
        GS::fps = 1 / frm_time;
        ui->user_edit_fps_box->setText(QString::number(GS::fps));
    }
}

void MainWindow::resizeEvent ( QResizeEvent * event ){
    get_window_size();
    bound_zoom();
    paint_ROI();
}

bool MainWindow::is_paused(){ return paused; }
void MainWindow::unpause_game(){
    ui->play_button->setIcon(pause_icon);
	paused = false;
    frame_timer->start(GS::ms_per_frame);
}
void MainWindow::pause_game(){
    ui->play_button->setIcon(play_icon);
	paused = true;
    frame_timer->stop();
}

// expand symbols to multiple pixels (from zoom settings) and map to color
void MainWindow::colorize_ROI(){
    int x, y;
    int xoffset, yoffset;
    QRgb next_pix;

    for(x = 0; x < GS::ROI_dims.x / GS::pps; x++){
    xoffset = x * GS::pps;

    for(y = 0; y < GS::ROI_dims.y / GS::pps; y++){
        // offset to get new location
        next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];

        yoffset = y * GS::pps;

        for(int px = 0; px < GS::pps; px++){
        for(int py = 0; py < GS::pps; py++){
            frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
        }}
    }}

    //
    // do symbols that are partially off the screen
    //

    int y_pix_left = GS::ROI_dims.y % GS::pps;
    int x_pix_left = GS::ROI_dims.x % GS::pps;

    // bottom row
    if(y_pix_left != 0){
        yoffset = y * GS::pps;

        for(x = 0; x < GS::ROI_dims.x / GS::pps; x++){
            next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];
            xoffset = x * GS::pps;

            for(int px = 0; px < GS::pps; px++){
            for(int py = 0; py < y_pix_left; py++){
                frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
            }}
        }
    }

    // rightmost column
    if(x_pix_left != 0){
        xoffset = x * GS::pps;

        for(y = 0; y < GS::ROI_dims.y / GS::pps; y++){
            next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];
            yoffset = y * GS::pps;

            for(int px = 0; px < x_pix_left; px++){
            for(int py = 0; py < GS::pps; py++){
                frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
            }}
        }
    }

    // bottom right pixel
    x = 1 + (GS::ROI_dims.x / GS::pps);
    y = 1 + (GS::ROI_dims.y / GS::pps);
    next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];
    for(int px = 0; px < x_pix_left; px++){
    for(int py = 0; py < y_pix_left; py++){
        frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
    }}

}

// GREAT IDEA: ADD CLICK FUNCTIONALITY TO MAIN GRAPHICS VIEW, SHOW POP UP WINDOW ABOUT STATS, INFO ON BLOCK

// use qtabwidget for tabs

// can simply edit xml ui and h file for custom view class

// add command line functionality? - textbrowser

// option to focus on character of choice?


void MainWindow::paint_ROI(){
    if(scene_item) delete scene_item; // why delete sceneitem? reuse it..

    supervisor->copy_ROI(symbol_buffer, GS::ROI_coors, (GS::ROI_dims / GS::pps) + 1);
    colorize_ROI(); // transfer symbol_buffer to frame_buffer

    QPixmap im = QPixmap::fromImage(*frame_buffer);

    scene_item = scene->addPixmap(im);
}

void MainWindow::update_ROI(){
    frame_clk.tick();
    paint_ROI();


    supervisor->iterate();
    double last_frm_time = frame_clk.tock();

    // check that the internal fps reflects what fps w'ere actually getting
    bound_fps(last_frm_time);
}

void MainWindow::on_zoom_slider_valueChanged(int value)
{
    // update ROI view if we're not zoomed to the max
    if((GS::ROI_dims / value) <= (Map_settings::map_len - 2)){
        GS::pps = value;
        paint_ROI();
    }
    else{      // reset value otherwise
        ui->zoom_slider->setValue(GS::pps);
    }
}

void MainWindow::update_fps(int value){
    // update fps
    GS::fps = value;
    GS::ms_per_frame = (1000/GS::fps);

    // restart timer with new framerate
    if(!paused){
        frame_timer->stop();
        frame_timer->start(GS::ms_per_frame);
    }

    // tell on_user_edit_fps_box_textChanged to not iterate

    user_edit_fps_box = false;
    ui->user_edit_fps_box->setText(QString::number(GS::fps));
    user_edit_fps_box = true;
}

void MainWindow::on_frame_slider_valueChanged(int value)
{
    update_fps(value);
}

void MainWindow::on_user_edit_fps_box_textChanged(const QString &arg1)
{
    if(user_edit_fps_box){
        static bool conversion_good = false;
        static int user_int = arg1.toInt(&conversion_good); // convert to int, check if conversion worked

        if(conversion_good){
            update_fps(user_int);
            ui->frame_slider->setValue(GS::fps);
        }
    }
}

void MainWindow::on_user_edit_fps_box_lostFocus()
{
    // return current fps to line in case it is blank
    ui->user_edit_fps_box->setText(QString::number(GS::fps));
}


void MainWindow::on_play_button_clicked()
{
    // pause/unpause game and update icon symbol
    if(paused)
        unpause_game();
    else
        pause_game();
}

void MainWindow::on_reset_button_clicked()
{
    pause_game();
    supervisor->reset_game();
    update_ROI();
}

void MainWindow::on_next_button_clicked()
{
    if(!paused) pause_game();
   update_ROI();
}

// methods for changing ROI position when panning
void MainWindow::adjust_ROI(Nav_symbol dir, int pps){
    Pair<int> move_vector = NAV::get_direction(dir);

    Pair<int> new_coor = GS::ROI_coors + (pps * move_vector);

    while( new_coor.x > (Map_settings::map_len.x - (GS::ROI_dims.x / GS::pps) - 2) ){
        new_coor.x--;
        if (new_coor.x == GS::ROI_coors.x ){
            break;
        }
    }
    while( new_coor.y < 0 ){
        new_coor.y++;
        if (new_coor.y == GS::ROI_coors.y ){
            break;
        }
    }
    while( new_coor.y > (Map_settings::map_len.y - (GS::ROI_dims.y / GS::pps) - 2) ){
        new_coor.y--;
        if (new_coor.y == GS::ROI_coors.y ){
            break;
        }
    }
    while( new_coor.x < 0 ){
        new_coor.x++;
        if (new_coor.x == GS::ROI_coors.x ){
            break;
        }
    }

    GS::ROI_coors = new_coor;

    paint_ROI();
}

void MainWindow::update_pan(){
    // bit of trickery here: do modulo as pps gets bigger so we never go above certain speed
    adjust_ROI( pan_direction, ((GS::pan_pps - 2) % (GS::pan_pps + 10))  );
    GS::pan_pps++;
}

void MainWindow::on_pan_east_clicked(){         adjust_ROI(EAST, GS::pan_pps);  }
void MainWindow::on_pan_west_clicked(){         adjust_ROI(WEST, GS::pan_pps);  }
void MainWindow::on_pan_north_clicked(){        adjust_ROI(NORTH, GS::pan_pps);  }
void MainWindow::on_pan_south_clicked(){        adjust_ROI(SOUTH, GS::pan_pps);  }
void MainWindow::on_pan_southwest_clicked(){    adjust_ROI(SOUTHW, GS::pan_pps);  }
void MainWindow::on_pan_northwest_clicked(){    adjust_ROI(NORTHW, GS::pan_pps);  }
void MainWindow::on_pan_northeast_clicked() {   adjust_ROI(NORTHE, GS::pan_pps);  }
void MainWindow::on_pan_southeast_clicked(){    adjust_ROI(SOUTHE, GS::pan_pps);  }

void MainWindow::on_pan_east_pressed(){        pan_timer->start(GS::ms_per_pan);  pan_direction = EAST; }
void MainWindow::on_pan_east_released(){       pan_timer->stop(); GS::pan_pps = 3;  }
void MainWindow::on_pan_west_pressed(){        pan_timer->start(GS::ms_per_pan);  pan_direction = WEST; }
void MainWindow::on_pan_west_released(){       pan_timer->stop(); GS::pan_pps = 3; }
void MainWindow::on_pan_north_pressed(){       pan_timer->start(GS::ms_per_pan);  pan_direction = NORTH; }
void MainWindow::on_pan_north_released(){      pan_timer->stop(); GS::pan_pps = 3; }
void MainWindow::on_pan_south_pressed(){       pan_timer->start(GS::ms_per_pan);  pan_direction = SOUTH; }
void MainWindow::on_pan_south_released(){      pan_timer->stop(); GS::pan_pps = 3; }
void MainWindow::on_pan_southwest_pressed(){   pan_timer->start(GS::ms_per_pan);  pan_direction = SOUTHW; }
void MainWindow::on_pan_southwest_released(){  pan_timer->stop(); GS::pan_pps = 3; }
void MainWindow::on_pan_northwest_pressed(){   pan_timer->start(GS::ms_per_pan);  pan_direction = NORTHW; }
void MainWindow::on_pan_northwest_released(){  pan_timer->stop(); GS::pan_pps = 3; }
void MainWindow::on_pan_northeast_pressed(){   pan_timer->start(GS::ms_per_pan);  pan_direction = NORTHE; }
void MainWindow::on_pan_northeast_released(){  pan_timer->stop(); GS::pan_pps = 3; }
void MainWindow::on_pan_southeast_pressed(){   pan_timer->start(GS::ms_per_pan);  pan_direction = SOUTHE; }
void MainWindow::on_pan_southeast_released(){  pan_timer->stop(); GS::pan_pps = 3; }
