#ifndef ALIBABAPROVIDER_H
#define ALIBABAPROVIDER_H

#include "providerbase.h"
namespace Ui {
class AlibabaConfiguration;
}

class AlibabaProvider : public ProviderBase
{
public:
    AlibabaProvider();

    QWidget *configuration() override;
    ProviderConnectionBase *connectToProvider() override;
private:
    Ui::AlibabaConfiguration *alibabaUi;
    QWidget *m_widget;
};

class AlibabaConnection : public ProviderConnectionBase
{
public:
    explicit AlibabaConnection(QObject *parent = nullptr);

    QString defaultPublishTopic() override;
    QString defaultSubscriptionTopic() override;
    QString windowTitle() override;

    QString productKey;
    QString deviceId;
};

#endif // ALIBABAPROVIDER_H
