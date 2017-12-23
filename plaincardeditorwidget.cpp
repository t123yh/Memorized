#include "plaincardeditorwidget.h"
#include "ui_plaincardeditorwidget.h"
#include "cardeditorwidget.h"
#include "utils.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTextEdit>

PlainCardEditorWidget::PlainCardEditorWidget(QWidget* parent)
  : CardEditorWidget(parent)
  , ui(new Ui::PlainCardEditorWidget)
{
  ui->setupUi(this);
  connect(ui->te_Front, SIGNAL(textChanged()), this, SIGNAL(valueChanged()));
  connect(ui->te_Back, SIGNAL(textChanged()), this, SIGNAL(valueChanged()));
  connect(ui->te_Notes, SIGNAL(textChanged()), this, SIGNAL(valueChanged()));
}

PlainCardEditorWidget::~PlainCardEditorWidget()
{
  delete ui;
}

void
PlainCardEditorWidget::loadData(const QJsonDocument& data)
{
  auto rootObj = data.object();
  ui->te_Front->setPlainText(getStringItem(rootObj, "front"));
  ui->te_Back->setPlainText(getStringItem(rootObj, "back"));
  ui->te_Notes->setPlainText(getStringItem(rootObj, "notes"));
}

QJsonDocument
PlainCardEditorWidget::getData()
{
  QJsonObject result;
  result.insert("front", ui->te_Front->toPlainText());
  result.insert("back", ui->te_Back->toPlainText());
  result.insert("notes", ui->te_Notes->toPlainText());
  return QJsonDocument(result);
}
