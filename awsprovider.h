#ifndef AWSPROVIDER_H
#define AWSPROVIDER_H

#include "providerbase.h"
namespace Ui {
class AwsConfiguration;
}

class AwsProvider : public ProviderBase
{
public:
    AwsProvider();

    QWidget *configuration() override;
    ProviderConnectionBase *connectToProvider() override;
private:
    Ui::AwsConfiguration *awsUi;
    QWidget *m_widget;
};

class AwsConnection : public ProviderConnectionBase
{
public:
    explicit AwsConnection(QObject *parent = nullptr);

    QString defaultPublishTopic() override;
    QString defaultSubscriptionTopic() override;
    QString windowTitle() override;
};

#endif // AWSPROVIDER_H
