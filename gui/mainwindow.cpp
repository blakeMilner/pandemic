#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    reset_icon(QIcon(GS::reset_icon_path)),
    pause_icon(QIcon(GS::pause_icon_path)),
    play_icon(QIcon(GS::play_icon_path)),

    lineEdit_userset(true),
    scene_item(0),

    frame_initialized(false)
{
    cout << "HRE1" << endl;
    frame_buffer = new QImage(GS::ROI_dims.x, GS::ROI_dims.y, QImage::Format_RGB32);

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    // connect timer in order to update ROI continuously
    connect(frame_timer, SIGNAL(timeout()), this, SLOT(update_ROI()));

    // make buffer for raw symbols from map
	symbol_buffer = new Map_symbol*[GS::MAX_ROI_DIMS.x];
	for(int i = 0; i < GS::MAX_ROI_DIMS.x; i++){
		symbol_buffer[i] = new Map_symbol[GS::MAX_ROI_DIMS.y];
    }

    // initialize button icons
    ui->pushButton->setIcon(play_icon);
    ui->pushButton->setIconSize(QSize(25,25));
    ui->pushButton_2->setIcon(reset_icon);
    ui->pushButton_2->setIconSize(QSize(25,25));

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

void MainWindow::start(){
    setVisible(true);

    get_window_size();
    update_buffer();

    paint_ROI();
}

void MainWindow::get_window_size(){
    // minus 2 to fit within margins
    GS::ROI_dims.x = ui->graphicsView->width() - 2;
    GS::ROI_dims.y = ui->graphicsView->height() - 2;
}

void MainWindow::update_buffer(){
    delete frame_buffer;

    frame_buffer = new QImage(GS::ROI_dims.x, GS::ROI_dims.y, QImage::Format_RGB32);
    bound_zoom();
}

// correct zoom level such that every sized map would cover the monitor
void MainWindow::bound_zoom(){
    while((GS::pix_per_symbol <= GS::MAX_pix_per_symbol)
          and (GS::ROI_dims / GS::pix_per_symbol).not_within(Map_settings::map_len - 2))
    {
        GS::pix_per_symbol++;
    }

    ui->zoom_slider->setValue(GS::pix_per_symbol);
}

void MainWindow::resizeEvent ( QResizeEvent * event ){
    get_window_size();

    update_buffer();
    paint_ROI();
}

bool MainWindow::is_paused(){ return paused; }
void MainWindow::unpause_game(){
	paused = false;
    frame_timer->start(GS::ms_per_frame);
}
void MainWindow::pause_game(){
	paused = true;
    frame_timer->stop();
}

// expand symbols to multiple pixels (from zoom settings) and map to color
void MainWindow::colorize_ROI(){
    int x, y;
    int xoffset, yoffset;
    QRgb next_pix;

    for(x = 0; x < GS::ROI_dims.x / GS::pix_per_symbol; x++){
    xoffset = x * GS::pix_per_symbol;

    for(y = 0; y < GS::ROI_dims.y / GS::pix_per_symbol; y++){
        next_pix = GS::color_map[symbol_buffer[x][y]];

        yoffset = y * GS::pix_per_symbol;

        for(int px = 0; px < GS::pix_per_symbol; px++){
        for(int py = 0; py < GS::pix_per_symbol; py++){
            frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
        }}
    }}

    //
    // do symbols that are partially off the screen
    //

    int y_pix_left = GS::ROI_dims.y % GS::pix_per_symbol;
    int x_pix_left = GS::ROI_dims.x % GS::pix_per_symbol;

    // bottom row
    if(y_pix_left != 0){
        yoffset = y * GS::pix_per_symbol;

        for(x = 0; x < GS::ROI_dims.x / GS::pix_per_symbol; x++){
            next_pix = GS::color_map[symbol_buffer[x][y]];
            xoffset = x * GS::pix_per_symbol;

            for(int px = 0; px < GS::pix_per_symbol; px++){
            for(int py = 0; py < y_pix_left; py++){
                frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
            }}
        }
    }

    // rightmost column
    if(x_pix_left != 0){
        xoffset = x * GS::pix_per_symbol;

        for(y = 0; y < GS::ROI_dims.y / GS::pix_per_symbol; y++){
            next_pix = GS::color_map[symbol_buffer[x][y]];
            yoffset = y * GS::pix_per_symbol;

            for(int px = 0; px < x_pix_left; px++){
            for(int py = 0; py < GS::pix_per_symbol; py++){
                frame_buffer->setPixel(xoffset + px, yoffset + py, next_pix);
            }}
        }
    }

    // bottom right pixel
    x = 1 + (GS::ROI_dims.x / GS::pix_per_symbol);
    y = 1 + (GS::ROI_dims.y / GS::pix_per_symbol);
    next_pix = GS::color_map[symbol_buffer[x][y]];
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
    if(scene_item) delete scene_item;

    supervisor->copy_ROI(symbol_buffer, GS::ROI_coors, (GS::ROI_dims / GS::pix_per_symbol) + 1);
    colorize_ROI();

    QPixmap im = QPixmap::fromImage(*frame_buffer);

    scene_item = scene->addPixmap(im);
}

void MainWindow::update_ROI(){
	paint_ROI();
    supervisor->iterate();
}

void MainWindow::on_zoom_slider_valueChanged(int value)
{
    // update ROI view if we're not zoomed to the max
    if((GS::ROI_dims / value) <= (Map_settings::map_len - 2)){
        GS::pix_per_symbol = value;
        paint_ROI();
    }
    else{      // reset value otherwise
        ui->zoom_slider->setValue(GS::pix_per_symbol);
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

    lineEdit_userset = false;
    ui->user_edit_fps_box->setText(QString::number(GS::fps));
    lineEdit_userset = true;
}

void MainWindow::on_frame_slider_valueChanged(int value)
{
    update_fps(value);
}

void MainWindow::on_user_edit_fps_box_textChanged(const QString &arg1)
{
    if(lineEdit_userset){
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


void MainWindow::on_pushButton_clicked()
{
    // pause/unpause game and update icon symbol
    if(paused){
        unpause_game();
        ui->pushButton->setIcon(pause_icon);
    }
    else{
        pause_game();
        ui->pushButton->setIcon(play_icon);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    pause_game();
    ui->pushButton->setIcon(play_icon);
    supervisor->reset_game();
    update_ROI();
}

