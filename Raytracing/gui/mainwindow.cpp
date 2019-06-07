#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::folder = "FIT_16206_Molochev_Raytracing_Data/";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->resize(width() - 18, height() - 82);

    percents = new QLabel(this);
    percents->setText("Select view mode.");
    ui->statusBar->addPermanentWidget(percents);

    double ratio = static_cast<double>(ui->widget->width()) / ui->widget->height();
    camera.setAspectRatio(ratio);

    timerix = new QTimer(this);
    connect(timerix, SIGNAL(timeout()), this, SLOT(incX()));
    timeriy = new QTimer(this);
    connect(timeriy, SIGNAL(timeout()), this, SLOT(incY()));
    timerdx = new QTimer(this);
    connect(timerdx, SIGNAL(timeout()), this, SLOT(decX()));
    timerdy = new QTimer(this);
    connect(timerdy, SIGNAL(timeout()), this, SLOT(decY()));

//    QVector3D c1(0, -0.7, 0), c2(0, 0.7, 0),
//            i1(-1, 1.5, 0), i2(-1, -1.5, 0), i3(-1, 0, 1.5), i4(-1, 0, -1.5);
//    Luminocity l1(0, 0, 1), l2(0, 1, 0), l3(1, 1, 1), l4(1, 0, 0),
//            am(QColor(25, 25, 25));
//    shared_ptr<Primitive> s1 = make_shared<Sphere>(c1, 0.5), s2 = make_shared<Sphere>(c2, 0.5);
//    reflection_params d, m;
//    d.cr = d.cg = d.cb = 0.1;
//    m.cr = m.cg = m.cb = 0.99;
//    s1->setOpticCharacteristics(d, m, 2000);
//    s2->setOpticCharacteristics(d, m, 2000);
//    camera.getScene().setDepth(10);
//    camera.setQuality(high);
//    camera.getScene().setAmbient(am);
//    camera.getScene().addPrimitive(s1);
//    camera.getScene().addPrimitive(s2);
//    camera.getScene().addIlluminant({i1, l1});
//    camera.getScene().addIlluminant({i2, l2});
//    camera.getScene().addIlluminant({i3, l3});
//    camera.getScene().addIlluminant({i4, l4});
//    camera.init();

    wireframe = QImage(ui->widget->width(), ui->widget->height(), QImage::Format_RGB888);
    showWireframe(camera.renderWireframe());
}

int MainWindow::getUbyX(double x){
    return static_cast<int>(ui->widget->width() * (x + 1) / 2 + 0.5);
}

int MainWindow::getVbyY(double y){
    return static_cast<int>(ui->widget->height() * (y + 1) / 2 + 0.5);
}

void MainWindow::showWireframe(vector<draw_segment> segments){
    wireframe.fill(Qt::white);
    for (auto &s: segments){
            double startx = s.start.x();
            double starty = s.start.y();
            double endx = s.end.x();
            double endy = s.end.y();
            PaintingUtils::drawLine(wireframe, getUbyX(startx), getVbyY(starty),
                                    getUbyX(endx), getVbyY(endy), s.color);
    }
    for (auto &i: camera.getIlluminants()){
        double x = i.start.x();
        double y = i.start.y();
        PaintingUtils::drawCircle(wireframe, getUbyX(x), getVbyY(y), 2, i.color);
    }
    ui->widget->setImage(wireframe);
}

void MainWindow::startRender(){
    ui->actionOpen_render_settings->setEnabled(false);
    ui->actionOpen_scene_file->setEnabled(false);
    ui->actionSelect_view->setEnabled(false);
    ui->actionInit_camera->setEnabled(false);
    ui->actionRender_settings->setEnabled(false);
    percents->setText("Rendering: 0%");
    rendered = QImage(ui->widget->width(), ui->widget->height(), QImage::Format_RGB888);
    t = new RenderThread([this](size_t p){
        percents->setText("Rendering: " + QString::number(p) + "%");
    },
    rendered, camera);
    connect(t, SIGNAL(ended()), this, SLOT(callback()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    t->start();
}

void MainWindow::callback(){
    ui->actionSelect_view->setEnabled(true);
    percents->setText("Rendering done.");
    isRendered = true;
    isRendering = false;
    ui->widget->setImage(rendered);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timerix;
    delete timeriy;
    delete timerdx;
    delete timerdy;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    double ratio = static_cast<double>(ui->widget->width()) / ui->widget->height();
    camera.setAspectRatio(ratio);
    wireframe = QImage(ui->widget->width(), ui->widget->height(), QImage::Format_RGB888);
    if (!isRendering && !isRendered){
        showWireframe(camera.renderWireframe());
    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    QMainWindow::paintEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent *event){
    if (isRendered || isRendering) return;
    if (isCtrl){
        camera.moveForward(-event->delta() / 1200.0);
    } else {
        camera.scaleFocus(-event->delta() / 1200.0);
    }
    showWireframe(camera.renderWireframe());
}

void MainWindow::incX(){
    camera.moveRight(0.05);
    showWireframe(camera.renderWireframe());
}

void MainWindow::decX(){
    camera.moveRight(-0.05);
    showWireframe(camera.renderWireframe());
}

void MainWindow::incY(){
    camera.moveUp(-0.05);
    showWireframe(camera.renderWireframe());
}

void MainWindow::decY(){
    camera.moveUp(0.05);
    showWireframe(camera.renderWireframe());
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (isRendered || isRendering) return;
    switch (event->key()) {
    case Qt::Key_Control: {
        isCtrl = true;
        break;
    }
    case Qt::Key_Left: {
        timerdx->start(5);
        break;
    }
    case Qt::Key_Right: {
        timerix->start(5);
        break;
    }
    case Qt::Key_Down: {
        timerdy->start(5);
        break;
    }
    case Qt::Key_Up: {
        timeriy->start(5);
        break;
    }
    default: {
        return;
    }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if (isRendered || isRendering) return;
    switch (event->key()) {
    case Qt::Key_Control: {
        isCtrl = false;
        break;
    }
    case Qt::Key_Left: {
        timerdx->stop();
        break;
    }
    case Qt::Key_Right: {
        timerix->stop();
        break;
    }
    case Qt::Key_Down: {
        timerdy->stop();
        break;
    }
    case Qt::Key_Up: {
        timeriy->stop();
        break;
    }
    default: {
        return;
    }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    from = QPoint(event->x(), event->y());
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (isRendered || isRendering) return;
    QPoint to = QPoint(event->x(), event->y());
    int x = to.x() - from.x();
    int y = to.y() - from.y();
    from = to;
    camera.rotate(x * M_PI / 180, y * M_PI / 180);
    showWireframe(camera.renderWireframe());
}

void MainWindow::on_actionRender_toggled(bool arg1)
{
    if (prog_toggle) return;
    prog_toggle = true;
    if (!arg1) {
        ui->actionRender->toggle();
        prog_toggle = false;
    } else {
        ui->actionSelect_view->toggle();
        prog_toggle = false;
        isRendering = true;
        setMinimumSize(size());
        setMaximumSize(size());
        startRender();
    }
}

void MainWindow::on_actionSelect_view_toggled(bool arg1)
{
    if (prog_toggle) return;
    prog_toggle = true;
    if (!arg1) {
        ui->actionSelect_view->toggle();
        prog_toggle = false;
    } else {
        ui->actionRender->toggle();
        prog_toggle = false;
        isRendered = false;
        ui->actionOpen_render_settings->setEnabled(true);
        ui->actionOpen_scene_file->setEnabled(true);
        ui->actionInit_camera->setEnabled(true);
        ui->actionRender_settings->setEnabled(true);
        percents->setText("Select view mode.");
        setMinimumSize(0, 0);
        setMaximumSize(16777215, 16777215);
        showWireframe(camera.renderWireframe());
    }
}

QString MainWindow::readLineWithoutComments(QTextStream *in){
    QString line;
    do {
        if (in->atEnd()) throw 0;
        line = in->readLine();
        line = line.trimmed();
    } while (line.startsWith("//") || line.isEmpty());
    return line.left(line.indexOf("//"));
}

vector<double> MainWindow::parseDoublesFromStrig(QString str){
    vector<double> numbers;
    QStringList list = str.split(" ");
    bool ok;
    for(int i = 0; i < list.size(); ++i){
        double num = list[i].toDouble(&ok);
        if (!ok) throw 0;
        numbers.push_back(num);
    }
    return numbers;
}

QColor MainWindow::parseColorFromString(QString str){
    QStringList list = str.split(" ");
    if (list.size() < 3) throw 0;
    bool ok;
    int first = list[0].toInt(&ok);
    if (!ok) throw 0;
    int second = list[1].toInt(&ok);
    if (!ok) throw 0;
    int third = list[2].toInt(&ok);
    if (!ok) throw 0;
    if (first < 0 || second < 0 || third < 0
            || first > 255 || second > 255 || third > 255)
        throw 0;
    return QColor(first, second, third);
}

int MainWindow::parseIntFromString(QString str){
    vector<int> numbers;
    QStringList list = str.split(" ");
    bool ok;
    if (list.size() != 1) throw 0;
    int num = list[0].toInt(&ok);
    if (!ok) throw 0;
    return num;
}

void MainWindow::parseParamsFromString(QString str, QVector3D &point, QColor &color){
    QStringList list = str.split(" ");
    if (list.size() != 6) throw 0;
    bool ok;
    double numd = list[0].toDouble(&ok);
    if (!ok) throw 0;
    point.setX(numd);
    numd = list[1].toDouble(&ok);
    if (!ok) throw 0;
    point.setY(numd);
    numd = list[2].toDouble(&ok);
    if (!ok) throw 0;
    point.setZ(numd);
    int num = list[3].toInt(&ok);
    if (!ok || num < 0 || num > 255) throw 0;
    color.setRed(num);
    num = list[4].toInt(&ok);
    if (!ok || num < 0 || num > 255) throw 0;
    color.setGreen(num);
    num = list[5].toInt(&ok);
    if (!ok || num < 0 || num > 255) throw 0;
    color.setBlue(num);
}

void MainWindow::on_actionOpen_scene_file_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open scene file", folder, "*.scene");
    if (filepath == nullptr) return;
    QFile file(filepath);
    file.open(QFile::ReadOnly);
    QTextStream *in = new QTextStream(&file);
    try {
        QColor ambient = parseColorFromString(readLineWithoutComments(in));
        camera.getScene().setAmbient(ambient);
        int lum_num = parseIntFromString(readLineWithoutComments(in));
        camera.getScene().clearIlluminants();
        for (int i = 0; i < lum_num; ++i) {
            QVector3D point;
            QColor color;
            parseParamsFromString(readLineWithoutComments(in), point, color);
            camera.getScene().addIlluminant({point, color});
        }
        camera.getScene().clearPrimitives();
        while (!in->atEnd()){
            shared_ptr<Primitive> ptr;
            QString name;
            try {
                name = readLineWithoutComments(in);
            } catch(...) { break; }
            if (name == "SPHERE"){
                vector<double> point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D c(point[0], point[1], point[2]);
                vector<double> rad = parseDoublesFromStrig(readLineWithoutComments(in));
                if (rad.size() != 1) throw 0;
                ptr = make_shared<Sphere>(c, rad[0]);
            } else if (name == "BOX") {
                vector<double> point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D pmin(point[0], point[1], point[2]);
                point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D pmax(point[0], point[1], point[2]);
                ptr = make_shared<Box>(pmin, pmax);
            } else if (name == "TRIANGLE") {
                vector<double> point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p1(point[0], point[1], point[2]);
                point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p2(point[0], point[1], point[2]);
                point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p3(point[0], point[1], point[2]);
                ptr = make_shared<Triangle>(p1, p2, p3);
            } else if (name == "QUADRANGLE") {
                vector<double> point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p1(point[0], point[1], point[2]);
                point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p2(point[0], point[1], point[2]);
                point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p3(point[0], point[1], point[2]);
                point = parseDoublesFromStrig(readLineWithoutComments(in));
                if (point.size() != 3) throw 0;
                QVector3D p4(point[0], point[1], point[2]);
                ptr = make_shared<Quadrangle>(p1, p2, p3, p4);
            } else throw 0;
            vector<double> opt = parseDoublesFromStrig(readLineWithoutComments(in));
            if (opt.size() != 7) throw 0;
            reflection_params diff, mirr;
            diff.cr = opt[0];
            diff.cg = opt[1];
            diff.cb = opt[2];
            mirr.cr = opt[3];
            mirr.cg = opt[4];
            mirr.cb = opt[5];
            ptr->setOpticCharacteristics(diff, mirr, opt[6]);
            camera.getScene().addPrimitive(ptr);
        }
        camera.init();
        if (!isRendering && !isRendered){
            showWireframe(camera.renderWireframe());
        }
    } catch (...) {
        QMessageBox msg;
                msg.setText("Error");
                msg.setInformativeText("Incorrect format of scene file");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.exec();
                return;
    }
}

void MainWindow::on_actionOpen_render_settings_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open render file", folder, "*.render");
    if (filepath == nullptr) return;
    QFile file(filepath);
    file.open(QFile::ReadOnly);
    QTextStream *in = new QTextStream(&file);
    try {
        QColor fone = parseColorFromString(readLineWithoutComments(in));
        camera.getScene().setFoneLuminocity(fone);
        vector<double> gamma = parseDoublesFromStrig(readLineWithoutComments(in));
        if (gamma.size() != 1) throw 0;
        camera.setGamma(gamma[0]);
        int depth = parseIntFromString(readLineWithoutComments(in));
        camera.getScene().setDepth(depth);
        QString quality = readLineWithoutComments(in);
        if (quality == "ROUGH"){
            camera.setQuality(rough);
        } else if (quality == "NORMAL") {
            camera.setQuality(normal);
        } else if (quality == "FINE") {
            camera.setQuality(fine);
        } else throw 0;
        vector<double> point = parseDoublesFromStrig(readLineWithoutComments(in));
        if (point.size() != 3) throw 0;
        QVector3D pcam(point[0], point[1], point[2]);
        point = parseDoublesFromStrig(readLineWithoutComments(in));
        if (point.size() != 3) throw 0;
        QVector3D pview(point[0], point[1], point[2]);
        point = parseDoublesFromStrig(readLineWithoutComments(in));
        if (point.size() != 3) throw 0;
        QVector3D vup(point[0], point[1], point[2]);
        camera.setCamParams(pcam, pview, vup);
        point = parseDoublesFromStrig(readLineWithoutComments(in));
        if (point.size() != 2) throw 0;
        viewport_params params;
        params.zn = point[0];
        params.zf = point[1];
        point = parseDoublesFromStrig(readLineWithoutComments(in));
        if (point.size() != 2) throw 0;
        params.sw = point[0];
        params.sh = point[1];
        camera.setViewport(params);
        camera.setAspectRatio(static_cast<double>(ui->widget->width()) / ui->widget->height());
        if (!isRendering && !isRendered){
            showWireframe(camera.renderWireframe());
        }
    } catch (...) {
        QMessageBox msg;
                msg.setText("Error");
                msg.setInformativeText("Incorrect format of scene file");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.exec();
                return;
    }
}

void MainWindow::on_actionInit_camera_triggered()
{
    camera.init();
    showWireframe(camera.renderWireframe());
}

void MainWindow::on_actionSave_image_triggered()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Save file as...", folder + "untitled.png", ".png");
    if (filepath == nullptr) return;
    QFile file(filepath);
    if (isRendered) rendered.save(&file, "PNG");
    else wireframe.save(&file, "PNG");
}

void MainWindow::on_actionSave_render_settings_triggered()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Save file as...", folder + "untitled.render", ".render");
    if (filepath == nullptr) return;
    QFile file(filepath);
    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    QColor fone = camera.getScene().getFoneLuminocity();
    out << fone.red() << " " << fone.green() << " " << fone.blue() << "\r\n";
    out << camera.getGamma() << "\r\n";
    out << camera.getScene().getDepth() << "\r\n";
    switch (camera.getQuality()){
    case rough:{
        out << "ROUGH\r\n";
        break;
    }
    case normal: {
        out << "NORMAL\r\n";
        break;
    }
    case fine: {
        out << "FINE\r\n";
        break;
    }
    }
    vector<QVector3D> cams = camera.getCamParams();
    out << cams[0].x() << " " << cams[0].y() << " " << cams[0].z() << "\r\n";
    out << cams[1].x() << " " << cams[1].y() << " " << cams[1].z() << "\r\n";
    out << cams[2].x() << " " << cams[2].y() << " " << cams[2].z() << "\r\n";
    viewport_params pars = camera.getViewport();
    out << pars.zn << " " << pars.zf << "\r\n";
    out << pars.sw << " " << pars.sh << "\r\n";
    file.close();
}

void MainWindow::on_actionRender_settings_triggered()
{
    if (isRendered || isRendering) return;
    RenderSettingsDialog dialog;
    dialog.setFone(camera.getScene().getFoneLuminocity());
    vector<QVector3D> cam = camera.getCamParams();
    dialog.setCamParams(cam[0], cam[1], cam[2]);
    dialog.setViewport(camera.getViewport());
    dialog.setQuality(camera.getQuality());
    dialog.setGamma(camera.getGamma());
    dialog.setDepth(camera.getScene().getDepth());
    dialog.exec();
    if (dialog.isAccepted()){
        camera.getScene().setFoneLuminocity(dialog.getFone());
        cam = dialog.getCamParams();
        camera.setCamParams(cam[0], cam[1], cam[2]);
        camera.setViewport(dialog.getViewport());
        camera.setQuality(dialog.getQuality());
        camera.setGamma(dialog.getGamma());
        camera.getScene().setDepth(dialog.getDepth());
        showWireframe(camera.renderWireframe());
    }
}

void MainWindow::on_actionAbout_triggered()
{
    About about;
    about.exec();
}
