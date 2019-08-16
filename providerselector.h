#ifndef PROVIDERSELECTOR_H
#define PROVIDERSELECTOR_H

#include "providerbase.h"
#include <QWidget>
#include <QHash>

namespace Ui {
class ProviderSelector;
}

class ProviderSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ProviderSelector(QWidget *parent = nullptr);
    ~ProviderSelector();

    void addProvider(const QString &name, ProviderBase *provider);

private:
    Ui::ProviderSelector *ui;
};

#endif // PROVIDERSELECTOR_H
