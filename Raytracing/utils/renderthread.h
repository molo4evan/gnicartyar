#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include <QThread>

#include <model/camera.h>

class RenderThread: public QThread {
Q_OBJECT
public:
    RenderThread(std::function<void(size_t)> notify, QImage &img, Camera& camera);
    void run();

signals:
    void ended();

private:
    std::function<void(size_t)> notify;
    QImage &img;
    Camera &camera;
};

#endif // RENDERTHREAD_H
