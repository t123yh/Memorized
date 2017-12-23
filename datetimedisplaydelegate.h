#ifndef DATETIMEDISPLAYDELEGATE_H
#define DATETIMEDISPLAYDELEGATE_H

#include <QStyledItemDelegate>
#include <QString>

class DateTimeDisplayDelegate : public QStyledItemDelegate
{
public:
    DateTimeDisplayDelegate();
    virtual QString displayText(const QVariant &value, const QLocale &locale) const override;
};

#endif // DATETIMEDISPLAYDELEGATE_H
