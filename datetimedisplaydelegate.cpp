#include "datetimedisplaydelegate.h"
#include "utils.h"

DateTimeDisplayDelegate::DateTimeDisplayDelegate()
{

}

QString DateTimeDisplayDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(locale);
    return getFancyDateTimeString(convertToDateTime(value.toString()));
}
