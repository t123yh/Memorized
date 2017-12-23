#include "plaincardreviewwidget.h"
#include "ui_plaincardreviewwidget.h"
#include "utils.h"

#include <QStringBuilder>

PlainCardReviewWidget::PlainCardReviewWidget(QWidget *parent) :
    CardReviewWidget(parent),
    ui(new Ui::PlainCardReviewWidget)
{
    ui->setupUi(this);
    backShown = false;
}

PlainCardReviewWidget::~PlainCardReviewWidget()
{
    delete ui;
}

void PlainCardReviewWidget::loadData(const QJsonDocument &data)
{
  auto rootObj = data.object();
  currentFront = getStringItem(rootObj, "front");
  currentBack = getStringItem(rootObj, "back");
  currentNotes = getStringItem(rootObj, "notes");
}

double PlainCardReviewWidget::getPerformanceRating()
{
   return 0;
}

void PlainCardReviewWidget::showContent()
{
    QString str;
    str += QString("<b>Front</b><br/>");
    str += QString(currentFront);
    if (backShown) {
        str += QString("<br/><b>Back</b><br/>");
        str += currentBack;
        str += QString("<br/><b>Notes</b><br/>");
        str += currentNotes;
    }
    ui->tb_Content->setText(str);
}


