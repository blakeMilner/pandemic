#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Supervisor* s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    game_iter(s, scene)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    game_iter.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::on_pushButton_clicked()
{
    QPixmap* a = new QPixmap(100,100);
    a->fill(Qt::black);

    scene->addPixmap(*a);
}
