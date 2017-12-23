#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

enum CardType {
    PLAIN_CARD = 1
};

QString getCurrentDateTime();

QDateTime convertToDateTime(const QString& str);

QString getFancyDateTimeString(const QDateTime& dt);

QJsonObject ObjectFromString(const QString& in);

void
Crash(const QString& reason);

void
Crash(const char* reason);

QString
getStringItem(const QJsonObject& obj, const char* name);

#endif // CONSTANTS_H
