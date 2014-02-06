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
    frame_buffer = new QImage(GS::ROI_dims.x * GS::MAX_pix_per_symbol,
    						  GS::ROI_dims.y * GS::MAX_pix_per_symbol, QImage::Format_RGB32);

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    connect(frame_timer, SIGNAL(timeout()), this, SLOT(update_ROI()));

	symbol_buffer = new Map_symbol*[GS::MAX_ROI_DIMS.x];
	for(int i = 0; i < GS::MAX_ROI_DIMS.x; i++){
		symbol_buffer[i] = new Map_symbol[GS::MAX_ROI_DIMS.y];
	}
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
	unpause_game();
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

	// put ROI stuff in GUI
	for(int x = 0; x < GS::ROI_dims.x; x++){
	for(int y = 0; y < GS::ROI_dims.y; y++){
		next_pix = GS::color_map[symbol_buffer[x][y]];

		int x_offset = GS::pix_per_symbol*x;
		int y_offset = GS::pix_per_symbol*y;

		for(int px = 0; px < GS::pix_per_symbol; px++){
		for(int py = 0; py < GS::pix_per_symbol; py++){
			frame_buffer->setPixel(x_offset + px, y_offset + py, next_pix);
		}}
	}}
}

void MainWindow::update_ROI(){
	supervisor->copy_ROI(symbol_buffer, GS::ROI_coors, GS::ROI_dims);
	colorize_ROI();

	QPixmap im = QPixmap::fromImage(*frame_buffer);

	QGraphicsPixmapItem* i = scene->addPixmap(im);
	//i->setPos(10,10);

	supervisor->iterate();
}
