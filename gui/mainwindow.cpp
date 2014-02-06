#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Supervisor* s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    paused(true),
    supervisor(s),
    ROI_coors(GUI_settings::ROI_coors),
    ROI_dims(GUI_settings::ROI_dims),   // just give max value for this, but don't use it all?
	frame_timer(new QTimer(this))
{
    frame_buffer = new QImage(ROI_dims.x, ROI_dims.y, QImage::Format_RGB32);

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    connect(frame_timer, SIGNAL(timeout()), this, SLOT(update_ROI()));

	symbol_buffer = new Map_symbol*[GUI_settings::MAX_ROI_DIMS.x];
	for(int i = 0; i < GUI_settings::MAX_ROI_DIMS.x; i++){
		symbol_buffer[i] = new Map_symbol[GUI_settings::MAX_ROI_DIMS.y];
	}
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete frame_buffer;

	for(int i = 0; i < GUI_settings::MAX_ROI_DIMS.x; i++){
		delete symbol_buffer[i];
	}
	delete symbol_buffer;
}

void MainWindow::on_pushButton_clicked()
{

    // TODO:: make safe pointer for buffer and figure out how to display this..

    unpause_game();

}

bool MainWindow::is_paused(){ return paused; }
void MainWindow::unpause_game(){
	paused = false;
    frame_timer->start(1000);
}
void MainWindow::pause_game(){
	paused = true;
    frame_timer->stop();
}

void MainWindow::colorize_ROI(){
	static QRgb next_pix;

	for(int x = 0; x < ROI_dims.x; x++){
	for(int y = 0; y < ROI_dims.y; y++){
		next_pix = GUI_settings::color_map[symbol_buffer[x][y]];

		// something wrong with color table here
		frame_buffer->setPixel(x, y, next_pix);
	}}
}

void MainWindow::update_ROI(){
	supervisor->copy_ROI(symbol_buffer, ROI_coors, ROI_dims);
	colorize_ROI();

	QPixmap im = QPixmap::fromImage(*frame_buffer);

	QGraphicsPixmapItem* i = scene->addPixmap(im);
	i->setPos(10,10);

	supervisor->iterate();
}
