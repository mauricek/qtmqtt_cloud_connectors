#include <stdio.h>
#include <string.h>
#include "sign_api.h"

#define EXAMPLE_PRODUCT_KEY     "a1X2bEnP82z"
#define EXAMPLE_PRODUCT_SECRET  "7jluWm1zql7bt8qK"
#define EXAMPLE_DEVICE_NAME     "example1"
#define EXAMPLE_DEVICE_SECRET   "ga7XA6KdlEeiPXQPpRbAjOZXwG8ydgSe"

int main(int argc, char *argv[])
{
    iotx_dev_meta_info_t meta_info;
    iotx_sign_mqtt_t sign_mqtt;

    memset(&meta_info, 0, sizeof(iotx_dev_meta_info_t));
    memcpy(meta_info.product_key, EXAMPLE_PRODUCT_KEY, strlen(EXAMPLE_PRODUCT_KEY));
    memcpy(meta_info.product_secret, EXAMPLE_PRODUCT_SECRET, strlen(EXAMPLE_PRODUCT_SECRET));
    memcpy(meta_info.device_name, EXAMPLE_DEVICE_NAME, strlen(EXAMPLE_DEVICE_NAME));
    memcpy(meta_info.device_secret, EXAMPLE_DEVICE_SECRET, strlen(EXAMPLE_DEVICE_SECRET));

    IOT_Sign_MQTT(IOTX_CLOUD_REGION_SHANGHAI, &meta_info, &sign_mqtt);
    printf("sign_mqtt.hostname: %s\n", sign_mqtt.hostname);
    printf("sign_mqtt.port    : %d\n", sign_mqtt.port);
    printf("sign_mqtt.username: %s\n", sign_mqtt.username);
    printf("sign_mqtt.password: %s\n", sign_mqtt.password);
    printf("sign_mqtt.clientid: %s\n", sign_mqtt.clientid);
    return 0;
}

