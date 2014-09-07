#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>

namespace GS = GUI_settings;
//namespace MS = Map_settings;

// GENIUS IDEA: Don't just copy in map pixel by picel, but have supervisor place all obejects and assume blank

MainWindow::MainWindow(Supervisor* s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),

    // make settings table - using item base model
    settings_table(new QStandardItemModel(GS::NUM_SETTINGS, 1, this)),
    settings_delegate(new Delegate(this)),

    // connect gui to the supervisor - which contains the running simulation
    supervisor(s),

    // create a timer that fires whenever we want a new frame
    paused(true),
    frame_timer(new QTimer(this)),
    frame_clk(Clock()),

    // timer that controls how fast we pan
    pan_timer(new QTimer(this)),
    pan_direction(NONE),

    // button icons
    pause_icon(QIcon(GS::pause_icon_path)),
    play_icon(QIcon(GS::play_icon_path)),
    next_icon(QIcon(GS::next_icon_path)),

    user_edit_fps_box(true),

    // make scene items for the main view screen and zoomed out region viewer
    mainScene(new QGraphicsScene(this)),
    regionScene(new QGraphicsScene(this)),
    mainScene_item(0),
    regionScene_item(0),

    changed_by_user(false),

    started(false)
{
    ui->setupUi(this);

    // set mainView scenes for main view and zoomed out view
    ui->mainView->setScene(mainScene);
    ui->regionView->setScene(regionScene);
    frame_buffer = new QImage(Map_settings::MAX_ROI_DIMS.x, Map_settings::MAX_ROI_DIMS.y, QImage::Format_RGB32);

    // set these so that when we go from larger FOV to smaller, the view doesn't have scrollbars
    ui->mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->regionView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->regionView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // connect timer in order to update ROI continuously
    connect(frame_timer, SIGNAL(timeout()), this, SLOT(update_ROI()));
    // connect timer in order to time panning movements
    connect(pan_timer, SIGNAL(timeout()), this, SLOT(update_pan()));

    // make buffer for raw symbols from map
    symbol_buffer = new Map_symbol*[Map_settings::MAX_ROI_DIMS.x];
    for(int i = 0; i < Map_settings::MAX_ROI_DIMS.x; i++){
        symbol_buffer[i] = new Map_symbol[Map_settings::MAX_ROI_DIMS.y];
    }

    // initialize button icons
    ui->play_button->setIcon(play_icon);
    ui->play_button->setIconSize(QSize(25,25));
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
    for(int row = 0; row < GS::NUM_SETTINGS; row++){
        for(int col = 0; col < 1; col++){
            QModelIndex index = settings_table->index(row,col,QModelIndex());
            settings_table->setData(index, 0);
        }
    }

    // add labels for settings table
    settings_table->setVerticalHeaderLabels(GS::settings_label_list);

    // setup settings table on other tab
    ui->settings_viewer->setModel(settings_table);
    ui->settings_viewer->setItemDelegate(settings_delegate);

    // setup box for region size - this will be the increment for the entire world size since
    // we want to slice evenly
    ui->grid_size_spinBox->setRange(Map_settings::MIN_REGION_LEN, Map_settings::MAX_REGION_LEN);
    ui->grid_size_spinBox->setValue(Map_settings::region_len);
    ui->grid_size_spinBox->setSingleStep(Map_settings::REGION_LEN_INCREMENT);

    // setup max/min and initial values for world size select
    ui->world_size_spinBox_X->setRange(Map_settings::MIN_ROI_DIMS.x, Map_settings::MAX_ROI_DIMS.x);
    ui->world_size_spinBox_Y->setRange(Map_settings::MIN_ROI_DIMS.y, Map_settings::MAX_ROI_DIMS.y);
    ui->world_size_spinBox_X->setValue(Map_settings::map_len.x);
    ui->world_size_spinBox_Y->setValue(Map_settings::map_len.y);
    ui->world_size_spinBox_X->setSingleStep(Map_settings::region_len);
    ui->world_size_spinBox_Y->setSingleStep(Map_settings::region_len);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete frame_buffer;

    for(int i = 0; i < Map_settings::MAX_ROI_DIMS.x; i++){
		delete symbol_buffer[i];
	}
	delete symbol_buffer;

    delete regionScene;
    delete mainScene;
}

/* show window, make buffer, and initialize screen
 * Must be called before the game can begin!
 */
void MainWindow::start(){
    setVisible(true);

    get_window_size();
    bound_zoom();

    paint_ROI();

    // only paint region view once...
    paint_regionView();
    // to display yellow box
    update_regionScene();

    started = true;
}

void MainWindow::get_window_size(){
    // minus 2 to fit within margins
    GS::ROI_dims.x = ui->mainView->width() - 2;
    GS::ROI_dims.y = ui->mainView->height() - 2;
}

void MainWindow::update_map_settings(){
    Map_settings::map_len = Map_settings::next_game_world_size;
    Map_settings::region_len = Map_settings::next_game_region_len;
}

// correct zoom level such that every sized map would cover the monitor
void MainWindow::bound_zoom(){
    while((GS::mainView_pps <= GS::MAX_pix_per_symbol)
          and (GS::ROI_dims / GS::mainView_pps).not_within(Map_settings::map_len - 2))
    {
        GS::mainView_pps++;
    }

    ui->zoom_slider->setValue(GS::mainView_pps);
}

// correct fps slider if we're capping out
void MainWindow::bound_fps(double frm_time){
    if(GS::fps > 1 / frm_time){
        GS::fps = 1 / frm_time;
        ui->user_edit_fps_box->setText(QString::number(GS::fps));
    }
}

// Is called immediately upon startup too... some things don't exist at this time. Pretty annoying
void MainWindow::resizeEvent ( QResizeEvent * event ){
    get_window_size();
    bound_zoom();
    paint_ROI();

    // do this to avoid updating the region scene before it is created (at startup before we have a view)
    if( started ){
        update_regionScene();
    }
}

// pausing functionality
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

    for(x = 0; x < GS::ROI_dims.x / GS::mainView_pps; x++){
    xoffset = x * GS::mainView_pps;

    for(y = 0; y < GS::ROI_dims.y / GS::mainView_pps; y++){
        // offset to get new location
        next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];

        yoffset = y * GS::mainView_pps;

        for(int px = 0; px < GS::mainView_pps; px++){
        for(int py = 0; py < GS::mainView_pps; py++){
            frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
        }}
    }}

    //
    // do symbols that are partially off the screen
    //

    int y_pix_left = GS::ROI_dims.y % GS::mainView_pps;
    int x_pix_left = GS::ROI_dims.x % GS::mainView_pps;

    // bottom row
    if(y_pix_left != 0){
        yoffset = y * GS::mainView_pps;

        for(x = 0; x < GS::ROI_dims.x / GS::mainView_pps; x++){
            next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];
            xoffset = x * GS::mainView_pps;

            for(int px = 0; px < GS::mainView_pps; px++){
            for(int py = 0; py < y_pix_left; py++){
                frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
            }}
        }
    }

    // rightmost column
    if(x_pix_left != 0){
        xoffset = x * GS::mainView_pps;

        for(y = 0; y < GS::ROI_dims.y / GS::mainView_pps; y++){
            next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];
            yoffset = y * GS::mainView_pps;

            for(int px = 0; px < x_pix_left; px++){
            for(int py = 0; py < GS::mainView_pps; py++){
                frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
            }}
        }
    }

    // bottom right pixel
    x = 1 + (GS::ROI_dims.x / GS::mainView_pps);
    y = 1 + (GS::ROI_dims.y / GS::mainView_pps);
    next_pix = GS::color_map[symbol_buffer[x + GS::ROI_coors.x][y + GS::ROI_coors.y]];
    for(int px = 0; px < x_pix_left; px++){
    for(int py = 0; py < y_pix_left; py++){
        frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
    }}

}

void MainWindow::paint_ROI(){
    if(mainScene_item) delete mainScene_item; // why delete sceneitem? reuse it..

    supervisor->copy_ROI(symbol_buffer, GS::ROI_coors, (GS::ROI_dims / GS::mainView_pps) + 1);
    colorize_ROI(); // transfer symbol_buffer to frame_buffer

    QPixmap im = QPixmap::fromImage(*frame_buffer);

    mainScene_item = mainScene->addPixmap(im);
}

// zoom out regional view as far as we can
void MainWindow::paint_regionView(){
    // TODO: make sampling better... maybe do a running gaussian filter?

    // get entire map in working buffer
    supervisor->copy_ROI(symbol_buffer, Pair<int>(0,0), Map_settings::map_len - 1);

    // get dimensions of zoomed out view region, make buffer
    regionView_dims = Pair<int>(ui->regionView->width(), ui->regionView->height());

    // need to fix these....
    regionView_buffer = new QImage(regionView_dims.x, regionView_dims.y, QImage::Format_RGB32);
    regionView_original_buffer = new QImage(regionView_dims.x, regionView_dims.y, QImage::Format_RGB32);

    // calculate sampling interval, to fill in zoomed out view exactly
    Pair<float> ds = Pair<float>( Map_settings::map_len.x / (float) regionView_dims.x, Map_settings::map_len.y / (float) regionView_dims.y );

    for(int x = 0; x < regionView_dims.x; x++){
    for(int y = 0; y < regionView_dims.y; y++){
        regionView_buffer->setPixel(x, y, GS::color_map[ symbol_buffer[(int) (x * ds.x)][(int) (y * ds.y)] ]);
    }
    }

    // make original pixmap to draw from and highlighted pixmap
    QPainter painter(regionView_original_buffer);
    painter.drawImage(QPoint(1, 1), *regionView_buffer);
    QPixmap im = QPixmap::fromImage(*regionView_buffer);

    // add pixmap to region scene -> display image
    regionScene_item = regionScene->addPixmap(im);
}

// update highlighting box on zoomed out view for new area
void MainWindow::update_regionScene(){
    // MAKE SURE TO ADD PAUSE AFTER PAN CLICK... WILL MAKE THIS FASTER!
    QPainter painter(regionView_buffer);
    painter.drawImage(QPoint(0, 0), *regionView_original_buffer);

    Pair<float> ds = Pair<float>( Map_settings::map_len.x / (float) regionView_dims.x, Map_settings::map_len.y / (float) regionView_dims.y );

    for(int x = GS::ROI_coors.x / ds.x; x < ((GS::ROI_dims.x/GS::mainView_pps) + GS::ROI_coors.x) / ds.x; x++){
        regionView_buffer->setPixel(x, GS::ROI_coors.y / ds.y, QT_YELLOW);
        regionView_buffer->setPixel(x, (GS::ROI_coors.y + (GS::ROI_dims.y/GS::mainView_pps)) / ds.y, QT_YELLOW);
        // replace these with def for yellow...
    }
    for(int y = GS::ROI_coors.y / ds.y; y < ((GS::ROI_dims.y/GS::mainView_pps) + GS::ROI_coors.y) / ds.y; y++){
        regionView_buffer->setPixel( GS::ROI_coors.x / ds.x, y, QT_YELLOW);
        regionView_buffer->setPixel( (GS::ROI_coors.x + (GS::ROI_dims.x/GS::mainView_pps)) / ds.x, y, QT_YELLOW);
    }

    QPixmap im = QPixmap::fromImage(*regionView_buffer);

    // add pixmap to region scene -> display image
    regionScene_item = regionScene->addPixmap(im);
}

void MainWindow::update_ROI(){
    frame_clk.tick();

    paint_ROI();
    supervisor->iterate();

    double last_frm_time = frame_clk.tock();

    // check that the internal fps reflects what fps w'ere actually getting
    bound_fps(last_frm_time);
}

// when the ROI is on the bottom side(s) and a zoom out occurs, we'll push them back in
// instead of going out of bounds
void MainWindow::shift_ROI_in(){
    while( GS::ROI_dims.x/GS::mainView_pps + GS::ROI_coors.x > Map_settings::map_len.x - 2)
        GS::ROI_coors.x -- ;
    while( GS::ROI_dims.y/GS::mainView_pps + GS::ROI_coors.y > Map_settings::map_len.y - 2)
        GS::ROI_coors.y -- ;
}

void MainWindow::on_zoom_slider_valueChanged(int value)
{
    // update ROI view if we're not zoomed to the max
    if((GS::ROI_dims / value) <= (Map_settings::map_len - 2)){
        GS::mainView_pps = value;

        if(started)
            shift_ROI_in();
        paint_ROI();
    }
    else{      // reset value otherwise
        ui->zoom_slider->setValue(GS::mainView_pps);
    }

    // this gets called on startup, resulting in segfault. not too elegant...
    if(started){
        update_regionScene();
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

    // put map settings that user changed during different simulations as new settings
    update_map_settings();

    supervisor->reset_game();

    update_ROI();
}

// advance forward one frame and then pause
void MainWindow::on_next_button_clicked()
{
    if(!paused) {
        pause_game();
    }

   update_ROI();
}

// methods for changing ROI position when panning
void MainWindow::adjust_ROI(Nav_symbol dir, int pps){
    Pair<int> move_vector = NAV::get_direction(dir);

    Pair<int> new_coor = GS::ROI_coors + (pps * move_vector);

    // these 4 while loops basically dial the new coordinates back down if they don't fit
    while( new_coor.x > (Map_settings::map_len.x - (GS::ROI_dims.x / GS::mainView_pps) - 2) ){
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
    while( new_coor.y > (Map_settings::map_len.y - (GS::ROI_dims.y / GS::mainView_pps) - 2) ){
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
    update_regionScene();
}

void MainWindow::update_pan(){
    // bit of trickery here: do modulo as pps gets bigger so we never go above certain speed
    // fine tuned for your liking...
    // We're pausing so that we don't experience lag from the game updating
    pause_game();
    adjust_ROI( pan_direction, ((GS::pan_pps - 2) % (GS::pan_pps + 10))  );
    unpause_game();

    GS::pan_pps++;
}

/* a bunch of ugly functions for each of the panning buttons - can this be improved? */
void MainWindow::on_pan_east_clicked(){         adjust_ROI(EAST, GS::pan_pps);  }
void MainWindow::on_pan_west_clicked(){         adjust_ROI(WEST, GS::pan_pps);  }
void MainWindow::on_pan_north_clicked(){        adjust_ROI(NORTH, GS::pan_pps);  }
void MainWindow::on_pan_south_clicked(){        adjust_ROI(SOUTH, GS::pan_pps);  }
void MainWindow::on_pan_southwest_clicked(){    adjust_ROI(SOUTHW, GS::pan_pps);  }
void MainWindow::on_pan_northwest_clicked(){    adjust_ROI(NORTHW, GS::pan_pps);  }
void MainWindow::on_pan_northeast_clicked() {   adjust_ROI(NORTHE, GS::pan_pps);  }
void MainWindow::on_pan_southeast_clicked(){    adjust_ROI(SOUTHE, GS::pan_pps);  }

void MainWindow::on_pan_east_pressed(){        pan_timer->start(GS::pan_pps);  pan_direction = EAST; }
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

// the grid size is the internal slicing of the game map into grids - for parallel and performance reasons
void MainWindow::on_grid_size_spinBox_valueChanged(int newval)
{
    if( (newval > Map_settings::MIN_REGION_LEN) && (newval < Map_settings::MAX_REGION_LEN)){
        Map_settings::next_game_region_len = newval;

        // set new step size of world size boxes
        ui->world_size_spinBox_X->setSingleStep(newval);
        ui->world_size_spinBox_Y->setSingleStep(newval);

        /* do a round world size up or down to match new region size */
        Pair<int> leftover = (Map_settings::next_game_world_size % newval);

        if( (Map_settings::next_game_world_size - leftover) > Map_settings::MIN_ROI_DIMS ){
            Map_settings::next_game_world_size -= leftover;
            ui->world_size_spinBox_X->setValue(Map_settings::next_game_world_size.x);
            ui->world_size_spinBox_Y->setValue(Map_settings::next_game_world_size.y);
        }
        else if( (Map_settings::next_game_world_size + newval - leftover) < Map_settings::MAX_ROI_DIMS ){
           Map_settings::next_game_world_size += (newval - leftover);
           ui->world_size_spinBox_X->setValue(Map_settings::next_game_world_size.x);
           ui->world_size_spinBox_Y->setValue(Map_settings::next_game_world_size.y);
        }
    }
}
