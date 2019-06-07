#ifndef RENDERSETTINGSDIALOG_H
#define RENDERSETTINGSDIALOG_H

#include <QDialog>
#include <QVector3D>

#include <structures/viewport_params.h>
#include <structures/quality.h>

namespace Ui {
class RenderSettingsDialog;
}

class RenderSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenderSettingsDialog(QWidget *parent = nullptr);
    ~RenderSettingsDialog();

    bool isAccepted();

    void setFone(QColor color);
    void setCamParams(QVector3D pcam, QVector3D pview, QVector3D vup);
    void setViewport(viewport_params params);
    void setQuality(quality q);
    void setGamma(double g);
    void setDepth(int depth);

    QColor getFone();
    std::vector<QVector3D> getCamParams();
    viewport_params getViewport();
    quality getQuality();
    double getGamma();
    int getDepth();

private slots:
    void on_cancel_clicked();

    void on_ok_clicked();

private:
    Ui::RenderSettingsDialog *ui;
    bool accepted = false;

    QColor fone;
    QVector3D pcam, pview, vup;
    viewport_params viewport;
    quality qual;
    double gamma;
    int depth;
};

#endif // RENDERSETTINGSDIALOG_H
