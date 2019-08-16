#include "awsprovider.h"
#include "ui_awsconfiguration.h"

#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QSslKey>

QByteArray readKey(const QString &fileName)
{
    QFile input(fileName);
    if (!input.exists() || !input.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open key file for reading";
        return QByteArray();
    }
    return input.readAll();
}

AwsProvider::AwsProvider()
    : ProviderBase()
    , awsUi(new Ui::AwsConfiguration)
{
    m_widget = new QWidget;
    awsUi->setupUi(m_widget);

    // File dialogs...
    connect(awsUi->rootCertButton, &QPushButton::clicked, this, [this]() {
        auto fn = QFileDialog::getOpenFileName(m_widget,
                                               QStringLiteral("Open root ca"),
                                               QString(),
                                               QStringLiteral("Root CA (*.crt)"));
        if (!fn.isEmpty())
            awsUi->rootCertEdit->setText(fn);
    });

    connect(awsUi->localCertButton, &QPushButton::clicked, this, [this]() {
        auto fn = QFileDialog::getOpenFileName(m_widget,
                                               QStringLiteral("Open device certificate"),
                                               QString(),
                                               QStringLiteral("Certificate Files (*.pem *.crt)"));
        if (!fn.isEmpty())
            awsUi->localCertEdit->setText(fn);
    });

    connect(awsUi->localKeyButton, &QPushButton::clicked, this, [this]() {
        auto fn = QFileDialog::getOpenFileName(m_widget,
                                               QStringLiteral("Open device key"),
                                               QString(),
                                               QStringLiteral("Device Key (*.key)"));
        if (!fn.isEmpty())
            awsUi->localKey->setText(fn);
    });

}

QWidget *AwsProvider::configuration()
{
    return m_widget;
}

ProviderConnectionBase *AwsProvider::connectToProvider()
{
    if (awsUi->hostEdit->text().isEmpty())
        return nullptr;

    QSslConfiguration conf;

    if (awsUi->rootCertEdit->text().isEmpty())
        return nullptr;
    auto caCert = QSslCertificate::fromPath(awsUi->rootCertEdit->text());
    if (caCert.isEmpty())
        return nullptr;
    conf.setCaCertificates(caCert);

    if (awsUi->localCertEdit->text().isEmpty())
        return nullptr;
    auto localCert = QSslCertificate::fromPath(awsUi->localCertEdit->text());
    if (localCert.isEmpty())
        return nullptr;
    conf.setLocalCertificateChain(localCert);

    if (awsUi->localKey->text().isEmpty())
        return nullptr;
    QSslKey sslkey(readKey(awsUi->localKey->text()), QSsl::Rsa);
    if (sslkey.isNull())
        return nullptr;
    conf.setPrivateKey(sslkey);

    auto con = new AwsConnection(nullptr);

    con->m_client.setKeepAlive(10000);
    con->m_client.setHostname(awsUi->hostEdit->text());
    con->m_client.setPort(8883);
    con->m_client.setClientId(awsUi->clientEdit->text());

    con->m_client.connectToHostEncrypted(conf);
    return con;
}

QString AwsConnection::defaultPublishTopic()
{
    return QStringLiteral("sdk/test/Python");
}

QString AwsConnection::defaultSubscriptionTopic()
{
    return QStringLiteral("sdk/test/Python");
}

QString AwsConnection::windowTitle()
{
    return QStringLiteral("AWS IoT Core Connection");
}

AwsConnection::AwsConnection(QObject *parent)
    : ProviderConnectionBase (parent)
{

}
