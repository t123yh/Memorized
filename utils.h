#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

enum CardType
{
  PLAIN_CARD = 1
};

enum
{
  GROUP_ALL = -1
};

const int ID_ALL = -1;

QString
getCurrentDateTime();

QString
toSqlTime(QDateTime dt);

QDateTime
convertToDateTime(const QString& str);

QString
getFancyDateTimeString(const QDateTime& dt);

QJsonObject
ObjectFromString(const QString& in);

void
Crash(const QString& reason);

void
Crash(const char* reason);

QString
getStringItem(const QJsonObject& obj, const char* name);

#endif // CONSTANTS_H
