#include "providerselector.h"
#include "alibabaprovider.h"
#include "awsprovider.h"
#include "azureprovider.h"
#include "googleprovider.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProviderSelector w;
#ifdef WITH_AWS
    w.addProvider(QStringLiteral("AWS Iot Core"), new AwsProvider());
#endif
#ifdef WITH_AZURE
    w.addProvider(QStringLiteral("Azure IoT Hub"), new AzureProvider());
#endif
#ifdef WITH_GOOGLE
    w.addProvider(QStringLiteral("Google Cloud IoT Core"), new GoogleProvider());
#endif
#ifdef WITH_ALIBABA
    w.addProvider(QStringLiteral("Alibaba Cloud IoT Platform"), new AlibabaProvider());
#endif
    w.show();

    return a.exec();
}
