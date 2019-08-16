#include "googleprovider.h"
#include "ui_googleconfiguration.h"
#include "3rdparty/google/jwtcreation.h"

#include <QLabel>
#include <QFileDialog>

GoogleProvider::GoogleProvider()
    : ProviderBase()
    , m_googleUi(new Ui::GoogleConfiguration)
{
    m_widget = new QWidget;
    m_googleUi->setupUi(m_widget);

    connect(m_googleUi->rootCertButton, &QPushButton::clicked, this, [this]() {
        auto fn = QFileDialog::getOpenFileName(m_widget,
                                               QStringLiteral("Open root ca"),
                                               QString(),
                                               QStringLiteral("Root CA (*.crt *.pem)"));
        if (!fn.isEmpty())
            m_googleUi->rootCertEdit->setText(fn);
    });

    connect(m_googleUi->localCertButton, &QPushButton::clicked, this, [this]() {
        auto fn = QFileDialog::getOpenFileName(m_widget,
                                               QStringLiteral("Open device certificate"),
                                               QString(),
                                               QStringLiteral("Certificate Files (*.pem *.crt)"));
        if (!fn.isEmpty())
            m_googleUi->localCertEdit->setText(fn);
    });

}

QWidget *GoogleProvider::configuration()
{
    return m_widget;
}

ProviderConnectionBase *GoogleProvider::connectToProvider()
{

    // Input
    const QString rootCAPath = m_googleUi->rootCertEdit->text();
    // Created by: openssl req -x509 -newkey rsa:2048 -keyout rsa_private.pem -nodes -out rsa_cert.pem -subj "/CN=unused"
    const QString deviceCertPath = m_googleUi->localCertEdit->text();
    // projects/PROJECT_ID/locations/REGION/registries/REGISTRY_ID/devices/DEVICE_ID
    const QString projectId = m_googleUi->projectEdit->text().toLower();
    const QString region = m_googleUi->regionBox->currentText();
    const QString registryId = m_googleUi->registryEdit->text();
    const QString deviceId = m_googleUi->deviceIdEdit->text();

    // Generate constants
    const QString clientId = QStringLiteral("projects/") + projectId
                           + QStringLiteral("/locations/") + region
                           + QStringLiteral("/registries/") + registryId
                           + QStringLiteral("/devices/") + deviceId;
    const QString googleiotHostName = QStringLiteral("mqtt.googleapis.com");
    const char *jwt = CreateJwt(deviceCertPath.toLocal8Bit().constData(),
                                projectId.toLocal8Bit().constData(),
                                "RS256");
    const QString password = QByteArray(jwt);

    if (password.isEmpty())
        return nullptr;

    auto con = new GoogleConnection();
    con->deviceId = deviceId;

    con->m_client.setKeepAlive(60);
    con->m_client.setCleanSession(false);
    con->m_client.setPort(8883);
    con->m_client.setHostname(googleiotHostName);
    con->m_client.setClientId(clientId);
    con->m_client.setPassword(password);

    auto caCerts = QSslCertificate::fromPath(rootCAPath);

    QSslConfiguration sslConf;
    sslConf.setCaCertificates(caCerts);

    con->m_client.connectToHostEncrypted(sslConf);

    return con;
}

QString GoogleConnection::defaultPublishTopic()
{
    return QStringLiteral("/devices/") + deviceId + QStringLiteral("/events");
}

QString GoogleConnection::defaultSubscriptionTopic()
{
    return QStringLiteral("/devices/") + deviceId + QStringLiteral("/commands/#");
}

QString GoogleConnection::windowTitle()
{
    return "Google IoT Core Connection";
}

GoogleConnection::GoogleConnection(QObject *parent)
    : ProviderConnectionBase (parent)
{

}
