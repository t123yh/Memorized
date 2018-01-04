#include "plaincardreviewwidget.h"
#include "ui_plaincardreviewwidget.h"
#include "utils.h"

#include <QDebug>
#include <QStringBuilder>

PlainCardReviewWidget::PlainCardReviewWidget(QWidget* parent)
  : CardReviewWidget(parent)
  , ui(new Ui::PlainCardReviewWidget)
{
  ui->setupUi(this);
  backShown = false;

#define INIT_BTN(_X_)                                                          \
  {                                                                            \
    connect(ui->rb_##_X_,                                                      \
            &QRadioButton::clicked,                                            \
            this,                                                              \
            &PlainCardReviewWidget::reviewed);                                 \
    ratingButtonGroup.addButton(ui->rb_##_X_);                                 \
  }

  INIT_BTN(Forgot);
  INIT_BTN(Fuzzy);
  INIT_BTN(Fair);
  INIT_BTN(Good);
}

PlainCardReviewWidget::~PlainCardReviewWidget()
{
  delete ui;
}

void
PlainCardReviewWidget::loadData(const QString& description,
                                const QJsonDocument& data)
{
  ui->lb_Name->setText(description);
  auto rootObj = data.object();
  currentFront = getStringItem(rootObj, "front");
  currentBack = getStringItem(rootObj, "back");
  currentNotes = getStringItem(rootObj, "notes");
  showContent();
}

double
PlainCardReviewWidget::getPerformanceRating()
{
  if (ui->rb_Forgot->isChecked()) {
    return 0;
  } else if (ui->rb_Fuzzy->isChecked()) {
    return 0.3;
  } else if (ui->rb_Fair->isChecked()) {
    return 0.7;
  } else if (ui->rb_Good->isChecked()) {
    return 1;
  }
  return -1;
}

void
PlainCardReviewWidget::setPerformanceRating(double performanceRating)
{
  resetPerformanceRating();
  if (performanceRating >= 0 && performanceRating < 0.15) {
    ui->rb_Forgot->setChecked(true);
  } else if (performanceRating < 0.5) {
    ui->rb_Fuzzy->setChecked(true);
  } else if (performanceRating < 0.85) {
    ui->rb_Fair->setChecked(true);
  } else {
    ui->rb_Good->setChecked(true);
  }
}

void
PlainCardReviewWidget::resetPerformanceRating()
{
  ratingButtonGroup.setExclusive(false);
  auto currentBtn = ratingButtonGroup.checkedButton();
  if (currentBtn != NULL) {
    currentBtn->setChecked(false);
  }
  ratingButtonGroup.setExclusive(true);
}

void
PlainCardReviewWidget::showContent()
{
  QString str;
  if (!currentFront.isEmpty()) {
    str += QString("<b>Front</b><br/>");
    str += QString(currentFront);
  }
  if (backShown) {
    if (!currentBack.isEmpty()) {
      str += QString("<br/><b>Back</b><br/>");
      str += currentBack;
    }
    if (!currentNotes.isEmpty()) {
      str += QString("<br/><b>Notes</b><br/>");
      str += currentNotes;
    }
  }
  if (!str.isEmpty()) {
    ui->tb_Content->setText(str);
  } else {
    ui->tb_Content->setText(tr("<i>No content available.</i>"));
  }
}

void
PlainCardReviewWidget::on_btn_ShowBackSide_clicked()
{
  backShown = !backShown;
  if (backShown) {
    ui->btn_ShowBackSide->setText(tr("Hide Back"));
  } else {
    ui->btn_ShowBackSide->setText(tr("Show Back"));
  }
  showContent();
}
