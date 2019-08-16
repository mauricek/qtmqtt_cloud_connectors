#ifndef PROVIDERCONNECTIONVIEW_H
#define PROVIDERCONNECTIONVIEW_H

#include "providerbase.h"
#include <QWidget>

namespace Ui {
class ProviderConnectionView;
}

class ProviderConnectionView : public QWidget
{
    Q_OBJECT

public:
    explicit ProviderConnectionView(QWidget *parent = nullptr);
    ~ProviderConnectionView();

    void setProviderConnection(ProviderConnectionBase *p);
private:
    void insertStatus(const QString &direction, const QString &type, const QString &topic, const QString &id, const QString &content);
    Ui::ProviderConnectionView *ui;
    ProviderConnectionBase *m_connection;
};

#endif // PROVIDERCONNECTIONVIEW_H
