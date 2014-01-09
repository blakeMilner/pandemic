#include "iterator.hpp"

Game_iterator::Game_iterator(Supervisor *s, QGraphicsScene* sc):
    simulation(s),
    scene(sc),
    is_running(false),
    ROI_coors(GUI_settings::ROI_coors),
    ROI_dims(GUI_settings::ROI_dims)
{
    buffer = new Map_symbol*[ROI_coors.x];
    for(int i = 0; i < ROI_coors.x; i++)
        buffer[i] = new Map_symbol[ROI_coors.y];
}

void Game_iterator::run(){
    for(;;){
        if(this->is_running){
            simulation->iterate();
            simulation->copy_ROI(buffer, ROI_coors, ROI_dims);

            // transform to pixmap
        }
    }
}
