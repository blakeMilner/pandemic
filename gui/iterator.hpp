#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "../main/containers/containers.hpp"
#include "../main/supervisor/supervisor.hpp"
#include "gui_settings.hpp"
#include <QThread>
#include <QGraphicsScene>

class Game_iterator : public QThread{
private:
    Supervisor* simulation;
    QGraphicsScene* scene;

    Map_symbol** buffer; // buffer for map symbols
    Pair<int> ROI_coors, ROI_dims;
    bool is_running;

public:
    Game_iterator(Supervisor*, QGraphicsScene*);
    void unpause_game();
    void pause_game();
    void set_ROI_coors(Pair<int>);
    void set_ROI_dims(Pair<int>);
    void run();
};

#endif


