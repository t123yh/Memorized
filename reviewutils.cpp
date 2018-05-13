#include "utils.h"

#include <QtSql>

int
getOverdueItemCount(int groupId, int userId)
{
  QSqlQuery query;
  QString queryText("SELECT count(*) FROM user_cards AS uc "
                    "INNER JOIN `cards` ON `cards`.`Id` = uc.`CardId` "
                    "WHERE (cast(julianday('now') + 0.5 as integer) - "
                    "cast(julianday(uc.`LastReviewed`) + 0.5 as integer))"
                    "    >= uc.`DaysBetweenReviews`"
                    " AND uc.`UserId` = :userId");

  if (groupId == GROUP_ALL) {
    query.prepare(queryText);
  } else {
    query.prepare(queryText + " AND `cards`.`GroupId` = :gid");
    query.bindValue(":gid", groupId);
  }
  query.bindValue(":userId", userId);
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  query.first();
  return query.value(0).toInt();
}
