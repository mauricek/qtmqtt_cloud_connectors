#include "azureprovider.h"
#include "ui_azureconfiguration.h"

#include "3rdparty/azure-iot-sdk-c/certs/certs.c"
#include <QLabel>

AzureProvider::AzureProvider()
    : ProviderBase()
    , m_azureUi(new Ui::AzureConfiguration)
{
    m_widget = new QWidget;
    m_azureUi->setupUi(m_widget);
}

QWidget *AzureProvider::configuration()
{
    return m_widget;
}

ProviderConnectionBase *AzureProvider::connectToProvider()
{
    const QString token = m_azureUi->plainTextEdit->toPlainText().remove(QLatin1Char('\n'));
    if (token.isEmpty())
        return nullptr;

    auto items = token.split(QLatin1Char(';'));
    QString iotHubHost;
    QString deviceId;
    QString signature;
    for (const auto &item : items) {
        const int index = item.indexOf(QLatin1Char('='));
        const QString cat = item.left(index);
        const QString value = item.mid(index + 1);
        if (cat == QStringLiteral("HostName")) {
            iotHubHost = value;
        } else if (cat == QStringLiteral("DeviceId")) {
            deviceId = value;
        } else if (cat == QStringLiteral("SharedAccessSignature")) {
            signature = value;
        } else {
            qDebug() << "Unknown item in SAS token:" << cat << ":" << value;
        }
    }
    if (iotHubHost.isEmpty() || deviceId.isEmpty() || signature.isEmpty())
        return nullptr;

    const QByteArray caCertData(certificates);
    auto caCerts = QSslCertificate::fromData(certificates);

    QSslConfiguration sslConf;
    sslConf.setCaCertificates(caCerts);

    auto con = new AzureConnection(nullptr);
    con->deviceId = deviceId;
    con->m_client.setPort(8883);
    con->m_client.setHostname(iotHubHost);
    con->m_client.setClientId(deviceId);
    con->m_client.setUsername(iotHubHost + QStringLiteral("/") + deviceId + QStringLiteral("/?api-version=2018-06-30"));
    con->m_client.setPassword(signature);

    con->m_client.connectToHostEncrypted(sslConf);
    return con;
}

QString AzureConnection::defaultPublishTopic()
{
    return QStringLiteral("devices/") + deviceId + QStringLiteral("/messages/events/");
}

QString AzureConnection::defaultSubscriptionTopic()
{
    return QStringLiteral("devices/") + deviceId + QStringLiteral("/messages/devicebound/#");
}

QString AzureConnection::windowTitle()
{
    return "Azure IoT Hub Connection";
}

AzureConnection::AzureConnection(QObject *parent)
    : ProviderConnectionBase (parent)
{

}
