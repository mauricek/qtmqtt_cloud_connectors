#ifndef GOOGLEPROVIDER_H
#define GOOGLEPROVIDER_H

#include "providerbase.h"
namespace Ui {
class GoogleConfiguration;
}

class GoogleProvider : public ProviderBase
{
public:
    GoogleProvider();

    QWidget *configuration() override;
    ProviderConnectionBase *connectToProvider() override;
private:
    Ui::GoogleConfiguration *m_googleUi;
    QWidget *m_widget;
};

class GoogleConnection : public ProviderConnectionBase
{
public:
    explicit GoogleConnection(QObject *parent = nullptr);

    QString defaultPublishTopic() override;
    QString defaultSubscriptionTopic() override;
    QString windowTitle() override;

    QString deviceId;
};

#endif // GOOGLEPROVIDER_H
