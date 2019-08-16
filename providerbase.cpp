#include "providerbase.h"

ProviderBase::ProviderBase(QObject *parent) : QObject(parent)
{
}

ProviderConnectionBase::ProviderConnectionBase(QObject *parent)
    : QObject(parent)
{
    connect(&m_client, &QMqttClient::messageSent, this, [this](qint32 id) {
        emit this->statusUpdate(QStringLiteral("Incoming"),
                                QStringLiteral("Message"),
                                QString(),
                                QString::number(id),
                                QStringLiteral("Confirmed"));
    });
    connect(&m_client, &QMqttClient::stateChanged, this, [this](QMqttClient::ClientState s) {
        QString state;
        switch (s) {
        case QMqttClient::Disconnected: state = QStringLiteral("Disconnected"); break;
        case QMqttClient::Connecting: state = QStringLiteral("Connecting"); break;
        case QMqttClient::Connected: state = QStringLiteral("Connected"); break;
        }
        emit this->statusUpdate(QStringLiteral("General"),
                                QStringLiteral("Connection"),
                                QString(),
                                QString(),
                                state);
    });
}

qint32 ProviderConnectionBase::publish(const QMqttTopicName &topic, const QByteArray &content, quint8 qos)
{
    auto id = m_client.publish(topic, content, qos);
    emit statusUpdate(QStringLiteral("Out"),
                       QStringLiteral("Publish"),
                       topic.name(),
                       QString::number(id),
                       content);

    return id;
}

static void delayedSubscriptionDeleter(QMqttSubscription *s)
{
    s->unsubscribe();
    s->connect(s, &QMqttSubscription::stateChanged, s, [&s](QMqttSubscription::SubscriptionState state) {
        if (state == QMqttSubscription::Unsubscribed)
            s->deleteLater();
    });
}

bool ProviderConnectionBase::subscribe(const QMqttTopicFilter &topic, quint8 qos)
{
    m_sub.reset(m_client.subscribe(topic, qos), delayedSubscriptionDeleter);
    emit statusUpdate(QStringLiteral("Outgoing"),
                      QStringLiteral("Subscription"),
                      topic.filter(),
                      QString(),
                      m_sub ? QString() : QStringLiteral("Failed"));
    if (!m_sub)
        return false;

    m_sub->connect(m_sub.data(), &QMqttSubscription::stateChanged, this, [this](QMqttSubscription::SubscriptionState s) {
        QString stateString;
        switch(s) {
        case QMqttSubscription::Subscribed:
            stateString = QStringLiteral("Subscribed");
            break;
        case QMqttSubscription::SubscriptionPending:
            stateString = QStringLiteral("Pending");
            break;
        case QMqttSubscription::Unsubscribed:
            stateString = QStringLiteral("Unsubscribed");
            break;
        case QMqttSubscription::UnsubscriptionPending:
            stateString = QStringLiteral("Unsubscription Pending");
            break;
        case QMqttSubscription::Error:
            stateString = QStringLiteral("Error");
            break;
        }
        emit this->statusUpdate(QStringLiteral("Incoming"),
                                QStringLiteral("Subscription"),
                                QString(),
                                QString(),
                                stateString);
    });
    m_sub->connect(m_sub.data(), &QMqttSubscription::messageReceived, m_sub.data(),
                   [this](const QMqttMessage &msg) {
        emit this->statusUpdate(QStringLiteral("Incoming"),
                                QStringLiteral("Subscription Message"),
                                msg.topic().name(),
                                QString::number(msg.id()),
                                msg.payload());
    });
    return m_sub;
}
