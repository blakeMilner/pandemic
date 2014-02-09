#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace GS = GUI_settings;


MainWindow::MainWindow(Supervisor* s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    paused(true),
    supervisor(s),
    frame_timer(new QTimer(this))
{
    frame_buffer = new QImage(GS::MAX_ROI_DIMS.x, GS::MAX_ROI_DIMS.y, QImage::Format_RGB32);

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
    ui->pushButton->setIcon(QIcon(GS::play_icon_path));
    ui->pushButton->setIconSize(QSize(25,25));

    // set slider step - value indicates pixels per symbol
    ui->horizontalSlider->setMinimum(GS::MIN_pix_per_symbol);
    ui->horizontalSlider->setMaximum(GS::MAX_pix_per_symbol);

    // set slider step - value indicates framerate (fps)
    ui->horizontalSlider_2->setMinimum(GS::MIN_FPS);
    ui->horizontalSlider_2->setMaximum(GS::MAX_FPS);
    ui->lineEdit->setValidator(new QIntValidator(GS::MIN_FPS, GS::MAX_FPS, this)); // only accept numbers
    ui->lineEdit->setText(QString::number(GS::fps)); // set initial fps in text box
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

void MainWindow::on_pushButton_clicked()
{
    // pause/unpause game and update icon symbol
    if(paused){
        unpause_game();
        ui->pushButton->setIcon(QIcon(GS::pause_icon_path));
    }
    else{
        pause_game();
        ui->pushButton->setIcon(QIcon(GS::play_icon_path));
    }
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

void MainWindow::colorize_ROI(){
	static QRgb next_pix;
    int x_offset;
    int y_offset;

	// put ROI stuff in GUI
	for(int x = 0; x < GS::ROI_dims.x; x++){
	for(int y = 0; y < GS::ROI_dims.y; y++){
		next_pix = GS::color_map[symbol_buffer[x][y]];

        x_offset = GS::pix_per_symbol*x;
        y_offset = GS::pix_per_symbol*y;

		for(int px = 0; px < GS::pix_per_symbol; px++){
		for(int py = 0; py < GS::pix_per_symbol; py++){
            frame_buffer->setPixel(x_offset + px, y_offset + py, next_pix);
		}}
	}}
}

// GREAT IDEA: ADD CLICK FUNCTIONALITY TO MAIN GRAPHICS VIEW, SHOW POP UP WINDOW ABOUT STATS, INFO ON BLOCK

// use qtabwidget for tabs

// can simply edit xml ui and h file for custom view class

// add command line functionality? - textbrowser

// option to focus on character of choice?

void MainWindow::update_ROI(){
    supervisor->copy_ROI(symbol_buffer, GS::ROI_coors, GS::ROI_dims);
    colorize_ROI();

    QPixmap im = QPixmap::fromImage(*frame_buffer);

    QGraphicsPixmapItem* i = scene->addPixmap(im);
    //i->setPos(10,10);


    supervisor->iterate();

    //scene->setSceneRect(0,0,1000,1000);
    //resize(1400,1400); // resize entire window if possible, otherwise limit scene resize
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // update ROI view
    GS::pix_per_symbol = value;
    GS::ROI_dims.x = GS::MAX_ROI_DIMS.x / GS::pix_per_symbol;
    GS::ROI_dims.y = GS::MAX_ROI_DIMS.y / GS::pix_per_symbol;

    // immediately update with new view, don't wait on timer
    update_ROI();
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

    ui->lineEdit->setText(QString::number(GS::fps));
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    update_fps(value);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    bool conversion_good = false;
    int user_int = arg1.toInt(&conversion_good); // convert to int, check if conversion worked

    if(conversion_good){
        update_fps(user_int);
        ui->horizontalSlider_2->setValue(GS::fps);
    }
}

void MainWindow::on_lineEdit_lostFocus()
{
    // return current fps to line in case it is blank
    ui->lineEdit->setText(QString::number(GS::fps));
}
