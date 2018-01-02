#include "utils.h"

#include <QtSql>

int
getOverdueItemCount(int groupId)
{
  QSqlQuery query;
  QString queryText("SELECT count(*) FROM cards "
                    "WHERE (cast(julianday('now') + 0.5 as integer) - "
                    "cast(julianday(`LastReviewed`) + 0.5 as integer))"
                    "    >= `DaysBetweenReviews`");

  if (groupId == GROUP_ALL) {
    query.prepare(queryText);
  } else {
    query.prepare(queryText + " AND `GroupId` = :gid");
    query.bindValue(":gid", groupId);
  }
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  query.first();
  return query.value(0).toInt();
}
