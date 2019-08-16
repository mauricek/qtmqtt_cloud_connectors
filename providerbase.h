#ifndef PROVIDERBASE_H
#define PROVIDERBASE_H

#include <QMqttClient>
#include <QObject>
#include <QWidget>
#include <QtMqtt/QMqttTopicName>

class ProviderConnectionBase : public QObject
{
    Q_OBJECT
public:
    explicit ProviderConnectionBase(QObject *parent = nullptr);

    virtual QString defaultPublishTopic() = 0;
    virtual QString defaultSubscriptionTopic() = 0;

    virtual QString windowTitle() = 0;
    virtual qint32 publish(const QMqttTopicName &topic, const QByteArray &content, quint8 qos = 1);
    virtual bool subscribe(const QMqttTopicFilter &topic, quint8 qos = 0);
signals:
    void statusUpdate(const QString &direction,
                      const QString &type,
                      const QString &topic,
                      const QString &id,
                      const QString &content);

public:
    QMqttClient m_client;
    QSharedPointer<QMqttSubscription> m_sub;
};

class ProviderBase : public QObject
{
    Q_OBJECT
public:
    explicit ProviderBase(QObject *parent = nullptr);

    virtual QWidget *configuration() = 0;
    virtual ProviderConnectionBase *connectToProvider() = 0;
};

#endif // PROVIDERBASE_H
