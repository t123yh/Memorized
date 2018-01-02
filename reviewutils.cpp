#include "utils.h"

#include <QtSql>

int
getOverdueItemCount()
{
  QSqlQuery query;
  query.prepare("SELECT count(*) FROM cards "
                "WHERE (cast(julianday('now') + 0.5 as integer) - cast(julianday(`LastReviewed`) + 0.5 as integer))"
                "    >= `DaysBetweenReviews`");
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  query.first();
  return query.value(0).toInt();
}
