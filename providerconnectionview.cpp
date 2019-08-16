#include "providerconnectionview.h"
#include "ui_providerconnectionview.h"

ProviderConnectionView::ProviderConnectionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProviderConnectionView)
{
    ui->setupUi(this);

    ui->statusTable->setColumnCount(5);
    ui->statusTable->setHorizontalHeaderLabels(QStringList()
                                               << QStringLiteral("Direction")
                                               << QStringLiteral("Type")
                                               << QStringLiteral("Topic")
                                               << QStringLiteral("ID")
                                               << QStringLiteral("Content"));
}

ProviderConnectionView::~ProviderConnectionView()
{
    delete ui;
}

void ProviderConnectionView::setProviderConnection(ProviderConnectionBase *p)
{
    m_connection = p;
    if (p ==nullptr)
        return;

    const auto publishTopic = p->defaultPublishTopic();
    ui->publishTopicEdit->setText(publishTopic);
    ui->publishTopicEdit->setPlaceholderText(publishTopic);

    const auto subTopic = p->defaultSubscriptionTopic();
    ui->subscriptionTopicEdit->setText(subTopic);
    ui->subscriptionTopicEdit->setPlaceholderText(subTopic);

    // Connections for updating / inserting into the table
    connect(ui->publishButton, &QPushButton::clicked, this, [this] () {
        const QString topic = ui->publishTopicEdit->text();
        const QByteArray payload = ui->publishPayloadEdit->toPlainText().toLocal8Bit();
        m_connection->publish(topic, payload);
    });

    connect(ui->subscriptionButton, &QPushButton::clicked, this, [this] () {
        m_connection->subscribe(ui->subscriptionTopicEdit->text(), 1);
    });
    connect(m_connection, &ProviderConnectionBase::statusUpdate, this, &ProviderConnectionView::insertStatus);
    setWindowTitle(m_connection->windowTitle());
}

void ProviderConnectionView::insertStatus(const QString &direction,
                                          const QString &type,
                                          const QString &topic,
                                          const QString &id,
                                          const QString &content)
{
    ui->statusTable->insertRow(0);
    ui->statusTable->setItem(0, 0, new QTableWidgetItem(direction));
    ui->statusTable->setItem(0, 1, new QTableWidgetItem(type));
    ui->statusTable->setItem(0, 2, new QTableWidgetItem(topic));
    ui->statusTable->setItem(0, 3, new QTableWidgetItem(id));
    ui->statusTable->setItem(0, 4, new QTableWidgetItem(content));
}
