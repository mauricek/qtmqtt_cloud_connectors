#include "providerselector.h"

#include "providerconnectionview.h"
#include "ui_providerselector.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(ProviderBase *)

ProviderSelector::ProviderSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProviderSelector)
{
    ui->setupUi(this);

    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this](const QString &) {
        auto provider = ui->comboBox->currentData().value<ProviderBase *>();
        static QWidget *active = nullptr;
        if (active) {
            ui->verticalLayout->removeWidget(active);
            active->setParent(nullptr);
        }
        active = provider->configuration();
        ui->verticalLayout->insertWidget(2, active);
    });

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        auto provider = ui->comboBox->currentData().value<ProviderBase *>();
        auto connection = provider->connectToProvider();
        if (!connection) {
            QMessageBox::warning(this, QStringLiteral("Error"),
                                 QStringLiteral("Insufficient information provided. Please fill out"
                                                "all required items!"));
            return;
        }
        auto connectionView = new ProviderConnectionView();
        connectionView->setProviderConnection(connection);
        connectionView->show();
    });
}

ProviderSelector::~ProviderSelector()
{
    delete ui;
}

void ProviderSelector::addProvider(const QString &name, ProviderBase *provider)
{
    ui->comboBox->addItem(name, QVariant::fromValue(static_cast<ProviderBase *>(provider)));
}
