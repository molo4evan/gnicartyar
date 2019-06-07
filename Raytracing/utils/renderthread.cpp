#include "renderthread.h"

RenderThread::RenderThread
(std::function<void(size_t)> notify, QImage &img, Camera& camera):
    QThread(), notify(notify), img(img), camera(camera) {}

void RenderThread::run(){
    camera.raytrace(img, notify, [this]{ emit ended(); });
}
