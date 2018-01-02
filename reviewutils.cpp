#include "utils.h"

#include <QtSql>

int
getOverdueItemCount()
{
  QSqlQuery query;
  query.prepare("SELECT count(*) FROM cards "
                "WHERE (round(julianday('now') + 0.5) - round(julianday(`LastReviewed`) + 0.5))"
                "    >= `DaysBetweenReviews`");
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  query.first();
  return query.value(0).toInt();
}
