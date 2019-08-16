#include "jwtcreation.h"
#include <stdio.h>
#include <time.h>
#include <jwt.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

// This is basically a copy of
// https://github.com/GoogleCloudPlatform/cpp-docs-samples/blob/master/iot/mqtt-ciotc/mqtt_ciotc.c
// according to https://cloud.google.com/iot/docs/how-tos/mqtt-bridge

#define TRACE 0
// [START iot_mqtt_jwt]
/**
 * Calculates issued at / expiration times for JWT and places the time, as a
 * Unix timestamp, in the strings passed to the function. The time_size
 * parameter specifies the length of the string allocated for both iat and exp.
 */
static void GetIatExp(char* iat, char* exp, int time_size) {
  // TODO: Use time.google.com for iat
  time_t now_seconds = time(NULL);
  snprintf(iat, time_size, "%lu", now_seconds);
  snprintf(exp, time_size, "%lu", now_seconds + 3600);
  if (TRACE) {
    printf("IAT: %s\n", iat);
    printf("EXP: %s\n", exp);
  }
}

static int GetAlgorithmFromString(const char *algorithm) {
    if (strcmp(algorithm, "RS256") == 0) {
        return JWT_ALG_RS256;
    }
    if (strcmp(algorithm, "ES256") == 0) {
        return JWT_ALG_ES256;
    }
    return -1;
}

/**
 * Calculates a JSON Web Token (JWT) given the path to a EC private key and
 * Google Cloud project ID. Returns the JWT as a string that the caller must
 * free.
 */
char* CreateJwt(const char* ec_private_path, const char* project_id, const char *algorithm) {
  char iat_time[sizeof(time_t) * 3 + 2];
  char exp_time[sizeof(time_t) * 3 + 2];
  uint8_t* key = NULL; // Stores the Base64 encoded certificate
  size_t key_len = 0;
  jwt_t *jwt = NULL;
  int ret = 0;
  char *out = NULL;

  // Read private key from file
  FILE *fp = fopen(ec_private_path, "r");
  if (fp == (void*) NULL) {
    printf("Could not open file: %s\n", ec_private_path);
    return "";
  }
  fseek(fp, 0L, SEEK_END);
  key_len = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  key = (uint8_t*)malloc(sizeof(uint8_t) * (key_len + 1)); // certificate length + \0

  fread(key, 1, key_len, fp);
  key[key_len] = '\0';
  fclose(fp);

  // Get JWT parts
  GetIatExp(iat_time, exp_time, sizeof(iat_time));

  jwt_new(&jwt);

  // Write JWT
  ret = jwt_add_grant(jwt, "iat", iat_time);
  if (ret) {
    printf("Error setting issue timestamp: %d\n", ret);
  }
  ret = jwt_add_grant(jwt, "exp", exp_time);
  if (ret) {
    printf("Error setting expiration: %d\n", ret);
  }
  ret = jwt_add_grant(jwt, "aud", project_id);
  if (ret) {
    printf("Error adding audience: %d\n", ret);
  }

  //ret = jwt_set_alg(jwt, JWT_ALG_ES256, (const unsigned char *)key, key_len);
  //ret = jwt_set_alg(jwt, JWT_ALG_RS256, (const unsigned char *)key, key_len);
  ret = jwt_set_alg(jwt, (jwt_alg)GetAlgorithmFromString(algorithm), (const unsigned char *)key, key_len);
  if (ret) {
    printf("Error during set alg: %d\n", ret);
  }
  out = jwt_encode_str(jwt);
  if(!out) {
      perror("Error during token creation:");
  }
  // Print JWT
  if (TRACE) {
    printf("JWT: [%s]\n", out);
  }

  jwt_free(jwt);
  free(key);
  return out;
}
// [END iot_mqtt_jwt]
