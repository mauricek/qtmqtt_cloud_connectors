#include "alibabaprovider.h"
#include "ui_alibabaconfiguration.h"

#include "sign_api.h"

#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QSslKey>

static iotx_mqtt_region_types_t regionStringToType(const QString &region)
{
    if (region == QStringLiteral("Shanghai"))
        return IOTX_CLOUD_REGION_SHANGHAI;
    else if (region == QStringLiteral("Singapore"))
        return IOTX_CLOUD_REGION_SINGAPORE;
    else if (region == QStringLiteral("Japan"))
        return IOTX_CLOUD_REGION_JAPAN;
    else if (region == QStringLiteral("USA West"))
        return IOTX_CLOUD_REGION_USA_WEST;
    else if (region == QStringLiteral("Germany"))
        return IOTX_CLOUD_REGION_GERMANY;
    else if (region == QStringLiteral("Custom"))
        return IOTX_CLOUD_REGION_CUSTOM;
    return IOTX_CLOUD_DOMAIN_MAX;
}

AlibabaProvider::AlibabaProvider()
    : ProviderBase()
    , alibabaUi(new Ui::AlibabaConfiguration)
{
    m_widget = new QWidget;
    alibabaUi->setupUi(m_widget);
}

QWidget *AlibabaProvider::configuration()
{
    return m_widget;
}

ProviderConnectionBase *AlibabaProvider::connectToProvider()
{
    const QString productKey = alibabaUi->productKeyEdit->text();
    const QString productSecret = alibabaUi->productSecretEdit->text();
    const QString deviceId = alibabaUi->deviceIdEdit->text();
    const QString deviceSecret = alibabaUi->deviceSecretEdit->text();
    const QString regionText = alibabaUi->regionComboBox->currentText();

    if (productKey.isEmpty() || productSecret.isEmpty()
            || deviceId.isEmpty() || deviceSecret.isEmpty())
        return nullptr;

    iotx_dev_meta_info_t deviceInfo;
    qstrcpy(deviceInfo.product_key, productKey.toLocal8Bit().constData());
    qstrcpy(deviceInfo.product_secret, productSecret.toLocal8Bit().constData());
    qstrcpy(deviceInfo.device_name, deviceId.toLocal8Bit().constData());
    qstrcpy(deviceInfo.device_secret, deviceSecret.toLocal8Bit().constData());

    iotx_sign_mqtt_t signInfo;
    int32_t result = IOT_Sign_MQTT(regionStringToType(regionText), &deviceInfo, &signInfo);

    if (result != 0) {
        qDebug() << "Could not create sign information";
        return nullptr;
    }

    auto con = new AlibabaConnection();
    con->productKey = productKey;
    con->deviceId = deviceId;

    con->m_client.setKeepAlive(10000);
    con->m_client.setHostname(QString::fromLocal8Bit(signInfo.hostname));
    con->m_client.setPort(signInfo.port);
    con->m_client.setClientId(QString::fromLocal8Bit(signInfo.clientid));
    con->m_client.setUsername(QString::fromLocal8Bit(signInfo.username));
    con->m_client.setPassword(QString::fromLocal8Bit(signInfo.password));

    con->m_client.connectToHost();
    return con;
}

QString AlibabaConnection::defaultPublishTopic()
{
    return QStringLiteral("/")
            + productKey
            + QStringLiteral("/")
            + deviceId
            + QStringLiteral("/user/update/");
}

QString AlibabaConnection::defaultSubscriptionTopic()
{
    // Only 1 level allowed
    //return QStringLiteral("/broadcast/") + productKey + QStringLiteral("/<sometopic>");
    return QStringLiteral("/")
            + productKey
            + QStringLiteral("/")
            + deviceId
            + QStringLiteral("/user/get");
}

QString AlibabaConnection::windowTitle()
{
    return QStringLiteral("Alibaba Cloud IoT Platform Connection");
}

AlibabaConnection::AlibabaConnection(QObject *parent)
    : ProviderConnectionBase (parent)
{

}
