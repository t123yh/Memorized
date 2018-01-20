#include "utils.h"

#include "sundown/buffer.h"
#include "sundown/html.h"
#include "sundown/markdown.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTime>

QString
getCurrentDateTime()
{
  return toSqlTime(QDateTime::currentDateTime());
}

QString
toSqlTime(QDateTime dt)
{
  return dt.toString("yyyy-MM-ddTHH:mm:ss");
}

void
Crash(const char* reason)
{
  qFatal(reason);
}

void
Crash(const QString& reason)
{
  qFatal(reason.toLatin1().data());
}

QDateTime
convertToDateTime(const QString& str)
{
  return QDateTime::fromString(str, Qt::ISODate);
}

QString
getFancyDateTimeString(const QDateTime& dt)
{
  auto current = QDateTime::currentDateTime();
  qint64 secs = dt.secsTo(current);
  if (secs < 60 * 2) {
    return QObject::tr("just now");
  } else if (secs < 60 * 60 * 2) {
    return QObject::tr("%1m ago").arg(secs / 60);
  } else if (secs < 60 * 60 * 24) {
    return QObject::tr("%1h ago").arg(secs / 60 / 60);
  } else if (dt.date().daysTo(current.date()) == 1) {
    return QObject::tr("yesterday");
  } else if (secs < 60 * 60 * 24 * 30) {
    return QObject::tr("%1d ago").arg(dt.date().daysTo(current.date()));
  } else if (dt.date().year() == current.date().year()) {
    return dt.toString("MM-dd");
  } else {
    return dt.toString("yyyy-MM-dd");
  }
}

// from https://stackoverflow.com/a/26804761/5597563
QJsonObject
ObjectFromString(const QString& in)
{
  QJsonObject obj;

  QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());
  qDebug() << in;

  // check validity of the document
  if (!doc.isNull()) {
    if (doc.isObject()) {
      obj = doc.object();
    } else {
      Crash("Document is not an object");
    }
  } else {
    Crash("Invalid JSON...");
  }

  return obj;
}

QString
getStringItem(const QJsonObject& obj, const char* name)
{
  QJsonObject::const_iterator iter = obj.constFind(name);
  if (iter == obj.end())
    return "";
  else
    return iter->toString();
}

QString
renderMarkdown(const QString& source)
{
  auto input = source.toUtf8();
  sd_callbacks callbacks;
  html_renderopt options;
  buf* ob;
  ob = bufnew(input.size() * 2);

  sdhtml_renderer(&callbacks, &options, 0);
  sd_markdown* md = sd_markdown_new(MKDEXT_SUPERSCRIPT, 16, &callbacks, &options);

  sd_markdown_render(ob, (unsigned char*)input.data(), input.size(), md);
  QByteArray result((char*)ob->data, ob->size);

  sd_markdown_free(md);
  bufrelease(ob);

  return QString::fromUtf8(result);
}
