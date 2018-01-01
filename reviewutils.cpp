#include "utils.h"

#include <QtSql>

int
getOverdueItemCount()
{
  QSqlQuery query;
  query.prepare("SELECT count(*) FROM cards "
                "WHERE julianday('now') - julianday(`LastReviewed`)"
                "    >= `DaysBetweenReviews`");
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  query.first();
  return query.value(0).toInt();
}
