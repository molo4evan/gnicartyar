#include "rendersettingsdialog.h"
#include "ui_rendersettingsdialog.h"

RenderSettingsDialog::RenderSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderSettingsDialog)
{
    ui->setupUi(this);
}

RenderSettingsDialog::~RenderSettingsDialog()
{
    delete ui;
}

void RenderSettingsDialog::setFone(QColor color)
{
    ui->red->setValue(fone.red());
    ui->green->setValue(fone.green());
    ui->blue->setValue(fone.blue());
}

void RenderSettingsDialog::setCamParams(QVector3D pcam, QVector3D pview, QVector3D vup)
{
    ui->pcam_x->setValue(pcam.x());
    ui->pcam_y->setValue(pcam.y());
    ui->pcam_z->setValue(pcam.z());
    ui->pview_x->setValue(pview.x());
    ui->pview_y->setValue(pview.y());
    ui->pview_z->setValue(pview.z());
    ui->vup_x->setValue(vup.x());
    ui->vup_y->setValue(vup.y());
    ui->vup_z->setValue(vup.z());
}

void RenderSettingsDialog::setViewport(viewport_params params)
{
    ui->zn->setValue(params.zn);
    ui->zf->setValue(params.zf);
    ui->sw->setValue(params.sw);
    ui->sh->setValue(params.sh);
}

void RenderSettingsDialog::setQuality(quality q)
{
    switch (q){
    case rough:{
        ui->rough->setChecked(true);
        break;
    }
    case normal:{
        ui->normal->setChecked(true);
        break;
    }
    case fine:{
        ui->fine->setChecked(true);
        break;
    }
    }
}

void RenderSettingsDialog::setGamma(double g)
{
    ui->gamma->setValue(g);
}

void RenderSettingsDialog::setDepth(int depth)
{
    ui->depth->setValue(depth);
}

QColor RenderSettingsDialog::getFone()
{
    return fone;
}

std::vector<QVector3D> RenderSettingsDialog::getCamParams()
{
    return {pcam, pview, vup};
}

viewport_params RenderSettingsDialog::getViewport()
{
    return viewport;
}

quality RenderSettingsDialog::getQuality()
{
    return qual;
}

double RenderSettingsDialog::getGamma()
{
    return gamma;
}

int RenderSettingsDialog::getDepth()
{
    return depth;
}

bool RenderSettingsDialog::isAccepted(){
    return accepted;
}

void RenderSettingsDialog::on_cancel_clicked()
{
    close();
}

void RenderSettingsDialog::on_ok_clicked()
{
    fone = QColor(ui->red->value(), ui->green->value(), ui->blue->value());
    pcam = QVector3D(ui->pcam_x->value(), ui->pcam_y->value(), ui->pcam_z->value());
    pview = QVector3D(ui->pview_x->value(), ui->pview_y->value(), ui->pview_z->value());
    vup = QVector3D(ui->vup_x->value(), ui->vup_y->value(), ui->vup_z->value());
    viewport.zn = ui->zn->value();
    viewport.zf = ui->zf->value();
    viewport.sw = ui->sw->value();
    viewport.sh = ui->sh->value();
    if (ui->rough->isChecked()) qual = rough;
    else if (ui->normal->isChecked()) qual = normal;
    else qual = fine;
    gamma = ui->gamma->value();
    depth = ui->depth->value();
    accepted = true;
    close();
}
