#include "camera.h"

#include <QTime>

Camera::Camera(){
    generateMcam();
    generateMproj();
}

void Camera::setCamParams(QVector3D pcam, QVector3D pview, QVector3D vup){
    this->pcam = pcam;
    scene.setEpoint(pcam);
    this->pview = pview;
    QVector3D z = (pview - pcam).normalized();
    QVector3D right = QVector3D::crossProduct(z, vup);
    this->vup = QVector3D::crossProduct(right, z);
    generateMcam();
}

void Camera::setViewport(viewport_params params){
    viewport = params;

    generateMproj();
}

void Camera::setQuality(quality q){
    qual = q;
}

void Camera::setGamma(double g){
    gamma = g;
}

viewport_params Camera::getViewport(){
    return viewport;
}

QMatrix4x4 Camera::getMcam(){
    return mcam;
}

QMatrix4x4 Camera::getMproj(){
    return mproj;
}

Scene& Camera::getScene(){
    return scene;
}

quality Camera::getQuality(){
    return qual;
}

double Camera::getGamma(){
    return gamma;
}

vector<QVector3D> Camera::getCamParams(){
    return {pcam, pview, vup};
}

void Camera::init(){
    gabarite_box b = scene.getGabariteBox();
    pview = QVector3D(b.x_min + (b.x_max - b.x_min)/2,
                      b.y_min + (b.y_max - b.y_min)/2,
                      b.z_min + (b.z_max - b.z_min)/2);
    vup = {0, 0, 1};
    pcam.setY(pview.y());
    pcam.setZ(pview.z());
    double x = b.x_min - (b.z_max - b.z_min)/2 * sqrt(3); // ?
    pcam.setX(x);
    scene.setEpoint(pcam);
    generateMcam();
    viewport.zn = (b.x_min - pcam.x()) / 2;
    viewport.zf = b.x_max - pcam.x() + (b.x_max - b.x_min)/2;
    viewport.sw = (b.y_max - b.y_min) / 2;
    viewport.sh = (b.z_max - b.z_min) / 2;
    if (viewport.zf - viewport.zn != 0 && viewport.sw != 0 && viewport.sh != 0){
        double rat = viewport.sw / viewport.sh;
        double delta = currentRatio / rat;
        if (delta > 1){
            viewport.sw *= delta;
        } else {
            viewport.sh /= delta;
        }
        generateMproj();
    }
}

void Camera::scaleFocus(double coefficient){
    viewport.zn += coefficient;
    if (viewport.zn < 0) viewport.zn = 0;
    generateMproj();
}

void Camera::moveForward(float delta){ //z
    QMatrix4x4 shift = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, delta,
        0, 0, 0, 1
    };
    QVector4D pv = pview.toVector4D(), pc = pcam.toVector4D();
    pv.setW(1);
    pc.setW(1);
    pview = (mcam_inv * shift * mcam * pv).toVector3D();
    pcam = (mcam_inv * shift * mcam * pc).toVector3D();
    scene.setEpoint(pcam);
    generateMcam();
}

void Camera::moveRight(float delta){   //x
    QMatrix4x4 shift = {
        1, 0, 0, delta,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    QVector4D pv = pview.toVector4D(), pc = pcam.toVector4D();
    pv.setW(1);
    pc.setW(1);
    pview = (mcam_inv * shift * mcam * pv).toVector3D();
    pcam = (mcam_inv * shift * mcam * pc).toVector3D();
    scene.setEpoint(pcam);
    generateMcam();
}

void Camera::moveUp(float delta){      //y
    QMatrix4x4 shift = {
        1, 0, 0, 0,
        0, 1, 0, delta,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    QVector4D pv = pview.toVector4D(), pc = pcam.toVector4D();
    pv.setW(1);
    pc.setW(1);
    pview = (mcam_inv * shift * mcam * pv).toVector3D();
    pcam = (mcam_inv * shift * mcam * pc).toVector3D();
    scene.setEpoint(pcam);
    generateMcam();
}

void Camera::rotate(double x_angle, double y_angle){
    QMatrix4x4 rotate_x = QMatrix4x4(
                1, 0,                   0,                    0,
                0, cos(-y_angle), -sin(-y_angle), 0,
                0, sin(-y_angle), cos(-y_angle),  0,
                0, 0,                   0,                    1);
    QMatrix4x4 rotate_y = QMatrix4x4(
                cos(x_angle),  0, sin(x_angle), 0,
                0,                    1, 0,                   0,
                -sin(x_angle), 0, cos(x_angle), 0,
                0,                    0, 0,                   1);
    QVector4D pc = pcam.toVector4D();
    pc.setW(1);
    QVector4D up = (pcam + vup).toVector4D();
    up.setW(1);
    pc = mview_inv * rotate_x * rotate_y * mview * pc;
    up = mview_inv * rotate_x * rotate_y * mview * up;
    pcam = pc.toVector3D();
    vup = (up - pcam).toVector3D();
    generateMcam();
}

void Camera::setAspectRatio(double ratio){
    viewport.sw = ratio * viewport.sh;
    currentRatio = ratio;
    generateMproj();
}

void Camera::generateMcam(){
    QVector3D k = pview - pcam;
    k = k / k.length();
    QVector3D i = QVector3D::crossProduct(k, vup);
    i = i / i.length();
    QVector3D j = QVector3D::crossProduct(k, i);
    QMatrix4x4 m1(
        i.x(), i.y(), i.z(), 0,
        j.x(), j.y(), j.z(), 0,
        k.x(), k.y(), k.z(), 0,
        0,     0,     0,     1
    );
    QMatrix4x4 m2(
        1, 0, 0, -pcam.x(),
        0, 1, 0, -pcam.y(),
        0, 0, 1, -pcam.z(),
        0, 0, 0, 1
    );
    QMatrix4x4 m3(
        1, 0, 0, pcam.x(),
        0, 1, 0, pcam.y(),
        0, 0, 1, pcam.z(),
        0, 0, 0, 1
    );
    QMatrix4x4 m4(
                1, 0, 0, -pview.x(),
                0, 1, 0, -pview.y(),
                0, 0, 1, -pview.z(),
                0, 0, 0, 1
    );
    QMatrix4x4 m5(
                1, 0, 0, pview.x(),
                0, 1, 0, pview.y(),
                0, 0, 1, pview.z(),
                0, 0, 0, 1
    );
    mcam = m1 * m2;
    mcam_inv = m3 * m1.transposed();
    mview = m1 * m4;
    mview_inv = m5 * m1.transposed();
}

void Camera::generateMproj(){
    double a = 2 * viewport.zn / viewport.sw;
    double b = 2 * viewport.zn / viewport.sh;
    double c = viewport.zf / (viewport.zf - viewport.zn);
    double d = -viewport.zf * viewport.zn / (viewport.zf - viewport.zn);
    mproj = QMatrix4x4(
        a, 0, 0, 0,
        0, b, 0, 0,
        0, 0, c, d,
        0, 0, 1, 0
    );
}

QVector3D Camera::getNearClipPoint(double x, double y){
    QVector4D point(x, y, viewport.zn, 1);
    return (mcam_inv * point).toVector3D();
}

void Camera::correct(vector<vector<Luminocity> > &lums){
    double max_lum = 0;
    for (auto& line: lums){
        for (auto& lum: line) {
            max_lum = max({max_lum, lum.Ir(), lum.Ig(), lum.Ib()});
        }
    }
    if (max_lum <= 0) return;
    double coef = max(max_lum, 1.0);
    for (auto& line: lums){
        for (auto& lum: line) {
            lum.setIr(pow(lum.Ir() / coef, gamma));
            lum.setIg(pow(lum.Ig() / coef, gamma));
            lum.setIb(pow(lum.Ib() / coef, gamma));
        }
    }
}

void Camera::raytrace(QImage &img, std::function<void (size_t)> notify, std::function<void()> callback){
    double pixwidth = viewport.sw / img.width(),
            pixheight = viewport.sh / img.height();
    size_t pw = img.width(), ph = img.height();
    if (qual == rough){
        pixwidth *= 2;
        pixheight *= 2;
        pw /= 2;
        ph /= 2;
    } else if (qual == fine){
        pixwidth /= 2;
        pixheight /= 2;
        pw *= 2;
        ph *= 2;
    }
    if (pw == 0 || ph == 0) return;

    size_t all_pixels = pw * ph, cur_pixels = 0, percents = 0;
    vector<vector<Luminocity>> lums;

    beam b = {pcam, QVector3D(1, 0, 0)};
    Luminocity luminos = scene.getLuminocity(b, 0);

    QTime time;
    time.start();

    for (size_t i = 0; i < ph; ++i) {
        double y = -viewport.sh / 2 + (i + 0.5) * pixheight;
        lums.push_back(vector<Luminocity>());
        for (size_t j = 0; j < pw; ++j) {
            double x = -viewport.sw / 2 + (j + 0.5) * pixwidth;
            QVector3D point = getNearClipPoint(x, y);
            QVector3D dir = (point - pcam).normalized();
            beam b = {pcam, dir};
            Luminocity lum = scene.getLuminocity(b, 0);
            lums[i].push_back(lum);
            ++cur_pixels;
            size_t cp = static_cast<size_t>(round(cur_pixels * 100.0 / all_pixels));
            if (cp != percents){
                notify(cp);
                percents = cp;
            }
        }
    }
    correct(lums);
    switch (qual) {
    case rough: {
        for (size_t i = 0; i < ph; ++i){
            vector<Luminocity> &line = lums[i];
            for (size_t j = 0; j < line.size(); ++j){
                img.setPixelColor(j * 2, i * 2, line[j]);
                img.setPixelColor(j * 2, i * 2 + 1, line[j]);
                img.setPixelColor(j * 2 + 1, i * 2, line[j]);
                img.setPixelColor(j * 2 + 1, i * 2 + 1, line[j]);
            }
        }
        break;
    }
    case normal: {
        for (size_t i = 0; i < ph; ++i){
            vector<Luminocity> &line = lums[i];
            for (size_t j = 0; j < line.size(); ++j){
                img.setPixelColor(j, i, line[j]);
            }
        }
        break;
    }
    case fine: {
        Luminocity lum;
        for (size_t i = 0; i < ph / 2; ++i){
            for (size_t j = 0; j < pw / 2; ++j){
                lum = lums[i * 2][j * 2] +
                        lums[i * 2 + 1][j * 2] +
                        lums[i * 2][j * 2 + 1] +
                        lums[i * 2 + 1][j * 2 + 1];
                lum /= 4;
                img.setPixelColor(j, i, lum);
            }
        }
        break;
    }
    }
    QTime current = QTime::currentTime();
    qDebug() << "Total rendering time is" << time.secsTo(current) << "s";
    callback();
}

vector<draw_segment> Camera::renderWireframe(){
    vector<draw_segment> result;
    QMatrix4x4 mapping = mproj * mcam;

//    QVector4D start = mapping * QVector4D(0, 0, 0, 1);
//    start /= start.w();
//    QVector4D end = mapping * QVector4D(1, 0, 0, 1);
//    end /= end.w();
//    result.push_back({
//                         QVector2D(start),
//                         QVector2D(end),
//                         x_color
//                     });
//    end = mapping * QVector4D(0, 1, 0, 1);
//    end /= end.w();
//    result.push_back({
//                         QVector2D(start),
//                         QVector2D(end),
//                         y_color
//                     });
//    end = mapping * QVector4D(0, 0, 1, 1);
//    end /= end.w();
//    result.push_back({
//                         QVector2D(start),
//                         QVector2D(end),
//                         z_color
//                     });

    const vector<shared_ptr<Primitive>>& primitives = scene.getPrimitives();
    for (size_t i = 0; i < primitives.size(); ++i) {
        vector<pair<QVector3D, QVector3D>> segments = primitives[i]->getSegments();
        for (size_t j = 0; j < segments.size(); ++j) {
            QVector4D start = segments[j].first.toVector4D();
            start.setW(1);
            start = mapping * start;
            start /= start.w();
            QVector4D end = segments[j].second.toVector4D();
            end.setW(1);
            end = mapping * end;
            end /= end.w();
            bool ok;
            pair<QVector4D, QVector4D> clipped = clip(start, end, ok);
            if (!ok) continue;
            result.push_back({
                                 QVector2D(clipped.first),
                                 QVector2D(clipped.second),
                                 default_color
                             });
        }
    }
    return result;
}

pair<QVector4D, QVector4D> Camera::clip(QVector4D start, QVector4D end, bool &ok){
    pair<QVector4D, QVector4D> out;
    out.first = start;
    out.second = end;
    if ((start.x() < -1 && end.x() < -1) ||
            (start.x() > 1 && end.x() > 1) ||
            (start.y() < -1 && end.y() < -1) ||
            (start.y() > 1 && end.y() > 1) ||
            (start.z() < 0 && end.z() < 0) ||
            (start.z() > 1 && end.z() > 1)){
        ok = false;
        return out;
    }
    ok = true;
    double dx = out.second.x() - out.first.x();
    double dy = out.second.y() - out.first.y();
    double dz = out.second.z() - out.first.z();
    if (out.first.x() < -1){
        double alpha = (out.second.x() + 1) / dx;
        QVector4D start(-1, out.second.y() - dy * alpha, out.second.z() - dz * alpha, 1);
        out.first = start;
    }
    if (out.second.x() < -1){
        double alpha = (out.first.x() + 1) / dx;
        QVector4D end(-1, out.first.y() - dy * alpha, out.first.z() - dz * alpha, 1);
        out.second = end;
    }
    if (out.first.x() > 1){
        double alpha = (1 - out.second.x()) / dx;
        QVector4D start(1, out.second.y() + dy * alpha, out.second.z() + dz * alpha, 1);
        out.first = start;
    }
    if (out.second.x() > 1){
        double alpha = (1 - out.first.x()) / dx;
        QVector4D end(1, out.first.y() + dy * alpha, out.first.z() + dz * alpha, 1);
        out.second = end;
    }
    if (out.first.y() < -1){
        double alpha = (out.second.y() + 1) / dy;
        QVector4D start(out.second.x() - dx * alpha, -1, out.second.z() - dz * alpha, 1);
        out.first = start;
    }
    if (out.second.y() < -1){
        double alpha = (out.first.y() + 1) / dy;
        QVector4D end(out.first.x() - dx * alpha, -1, out.first.z() - dz * alpha, 1);
        out.second = end;
    }
    if (out.first.y() > 1){
        double alpha = (1 - out.second.y()) / dy;
        QVector4D start(out.second.x() + dx * alpha, 1, out.second.z() + dz * alpha, 1);
        out.first = start;
    }
    if (out.second.y() > 1){
        double alpha = (1 - out.first.y()) / dy;
        QVector4D end(out.first.x() + dx * alpha, 1, out.first.z() + dz * alpha, 1);
        out.second = end;
    }
    if (out.first.z() < 0){
        double alpha = (out.second.z()) / dz;
        QVector4D start(out.second.x() - dx * alpha, out.second.y() - dy * alpha, 0, 1);
        out.first = start;
    }
    if (out.second.z() < 0){
        double alpha = (out.first.z()) / dz;
        QVector4D end(out.first.x() - dx * alpha, out.first.y() - dy * alpha, 0, 1);
        out.second = end;
    }
    if (out.first.z() > 1){
        double alpha = (1 - out.second.z()) / dz;
        QVector4D start(out.second.x() + dx * alpha, out.second.y() + dy * alpha, 1, 1);
        out.first = start;
    }
    if (out.second.z() > 1){
        double alpha = (1 - out.first.z()) / dz;
        QVector4D end(out.first.x() + dx * alpha, out.first.y() + dy * alpha, 1, 1);
        out.second = end;
    }
    return out;
}

QVector2D Camera::getCenter(){
    QMatrix4x4 mapping = mproj * mcam;
    QVector4D c = mapping * QVector4D(0, 0, 0, 1);
    c /= c.w();
    return QVector2D(c);
}

vector<draw_segment> Camera::getIlluminants(){
    vector<draw_segment> out;
    QMatrix4x4 mapping = mproj * mcam;
    for(auto &i: scene.getIlluminants()){
        draw_segment s;
        QVector4D vec = i.pos.toVector4D();
        vec.setW(1);
        vec  = mapping * vec;
        vec /= vec.w();
        s.start = vec.toVector2D();
        s.color = i.luminocity;
        out.push_back(s);
    }
    return out;
}
