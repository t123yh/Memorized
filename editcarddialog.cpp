#include "editcarddialog.h"
#include "plaincardeditorwidget.h"
#include "ui_editcarddialog.h"
#include "utils.h"

#include <QMessageBox>
#include <QLineEdit>

EditCardDialog::EditCardDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::EditCardDialog)
{
  ui->setupUi(this);
  ui->cmb_Type->addItem(tr("Plain Card"), PLAIN_CARD);
  ui->cmb_Type->setCurrentIndex(0);
  connect(ui->tb_Name, &QLineEdit::textEdited,
          this, &EditCardDialog::on_editor_ValueChanged);
}

EditCardDialog::~EditCardDialog()
{
  if (currentEditorWidget != NULL) {
    delete currentEditorWidget;
    currentEditorWidget = NULL;
  }
  delete ui;
}

void
EditCardDialog::setData(const QString& name,
                        CardType type,
                        const QJsonDocument& data)
{
  ui->tb_Name->setText(name);

  int typeIndex = ui->cmb_Type->findData(type);
  ui->cmb_Type->setCurrentIndex(typeIndex);

  currentInputData = data;
  if (currentEditorWidget != NULL) {
    currentEditorWidget->loadData(currentInputData);
  }
}

QString
EditCardDialog::getCurrentName()
{
  return ui->tb_Name->text();
}

CardType
EditCardDialog::getCurrentType()
{
  return (CardType)ui->cmb_Type->currentData().toInt();
}

QJsonDocument
EditCardDialog::getCurrentData()
{
  if (currentEditorWidget != NULL) {
    return currentEditorWidget->getData();
  } else {
    return QJsonDocument();
  }
}

void
EditCardDialog::placeEditorWidget()
{
  int type = ui->cmb_Type->currentData().toInt();
  if (currentEditorWidget != NULL) {
    currentEditorWidget->deleteLater();
    currentEditorWidget = NULL;
  }
  switch (type) {
    case PLAIN_CARD:
      currentEditorWidget = new PlainCardEditorWidget(this);
      break;
    default:
      Crash("Unknown card type.");
      break;
  }
  if (currentEditorWidget != NULL) {
    currentEditorWidget->loadData(currentInputData);
    connect(currentEditorWidget, &CardEditorWidget::valueChanged,
            this, &EditCardDialog::on_editor_ValueChanged);
    ui->layout_Editor->addWidget(currentEditorWidget);
  }
}

void
EditCardDialog::on_cmb_Type_currentIndexChanged(int index)
{
  Q_UNUSED(index);
  placeEditorWidget();
}

void
EditCardDialog::on_editor_ValueChanged()
{
  valueChanged = true;
}

void
EditCardDialog::on_btn_Cancel_clicked()
{
    /*
  if (this->valueChanged) {
    QMessageBox::StandardButton reply = QMessageBox::question(
      this,
      "L:D_N:dialog_ID:cancelconfirm",
      QString(tr("Are you sure you want to discard all changes?")),
      QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
      return;
    }
  }
  */
  reject();
}
