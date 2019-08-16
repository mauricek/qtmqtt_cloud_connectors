#ifndef AZUREPROVIDER_H
#define AZUREPROVIDER_H

#include "providerbase.h"
namespace Ui {
class AzureConfiguration;
}

class AzureProvider : public ProviderBase
{
public:
    AzureProvider();

    QWidget *configuration() override;
    ProviderConnectionBase *connectToProvider() override;
private:
    Ui::AzureConfiguration *m_azureUi;
    QWidget *m_widget;
};

class AzureConnection : public ProviderConnectionBase
{
public:
    explicit AzureConnection(QObject *parent = nullptr);

    QString defaultPublishTopic() override;
    QString defaultSubscriptionTopic() override;
    QString windowTitle() override;

    QString deviceId;
};

#endif // AZUREPROVIDER_H
