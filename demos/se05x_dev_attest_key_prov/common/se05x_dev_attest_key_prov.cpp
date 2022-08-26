/*
 *
 * Copyright 2021 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <errno.h>
#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxEnsure.h>
#include <nxLog_App.h>
#include <stdio.h>
#include <string.h>

/* clang-format off */
const uint8_t keyPairData[] = {
    0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13,
    0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02,
    0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
    0x03, 0x01, 0x07, 0x04, 0x6D, 0x30, 0x6B, 0x02,
    0x01, 0x01, 0x04, 0x20,

    /* Private key */
    0xaa, 0xb6, 0x00, 0xae, 0x8a, 0xe8, 0xaa, 0xb7,
    0xd7, 0x36, 0x27, 0xc2, 0x17, 0xb7, 0xc2, 0x04,
    0x70, 0x9c, 0xa6, 0x94, 0x6a, 0xf5, 0xf2, 0xf7,
    0x53, 0x08, 0x33, 0xa5, 0x2b, 0x44, 0xfb, 0xff,

    0xA1, 0x44, 0x03, 0x42, 0x00,

    /* Public key */
    0x04, 0x46, 0x3a, 0xc6, 0x93, 0x42, 0x91, 0x0a,
    0x0e, 0x55, 0x88, 0xfc, 0x6f, 0xf5, 0x6b, 0xb6,
    0x3e, 0x62, 0xec, 0xce, 0xcb, 0x14, 0x8f, 0x7d,
    0x4e, 0xb0, 0x3e, 0xe5, 0x52, 0x60, 0x14, 0x15,
    0x76, 0x7d, 0x16, 0xa5, 0xc6, 0x63, 0xf7, 0x93,
    0xe4, 0x91, 0x23, 0x26, 0x0b, 0x82, 0x97, 0xa7,
    0xcd, 0x7e, 0x7c, 0xfc, 0x7b, 0x31, 0x6b, 0x39,
    0xd9, 0x8e, 0x90, 0xd2, 0x93, 0x77, 0x73, 0x8e,
    0x82,
};

const uint8_t dev_attest_cert[491] = {
    0x30, 0x82, 0x01, 0xe7, 0x30, 0x82, 0x01, 0x8e, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x08, 0x69, 0xcd, 0xf1, 0x0d, 0xe9, 0xe5,
    0x4e, 0xd1, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x3d, 0x31, 0x25, 0x30, 0x23, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x1c, 0x4d, 0x61, 0x74, 0x74, 0x65, 0x72, 0x20, 0x44, 0x65, 0x76, 0x20, 0x50, 0x41, 0x49, 0x20,
    0x30, 0x78, 0x46, 0x46, 0x46, 0x31, 0x20, 0x6e, 0x6f, 0x20, 0x50, 0x49, 0x44, 0x31, 0x14, 0x30, 0x12, 0x06, 0x0a, 0x2b, 0x06,
    0x01, 0x04, 0x01, 0x82, 0xa2, 0x7c, 0x02, 0x01, 0x0c, 0x04, 0x46, 0x46, 0x46, 0x31, 0x30, 0x20, 0x17, 0x0d, 0x32, 0x32, 0x30,
    0x32, 0x30, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a, 0x18, 0x0f, 0x39, 0x39, 0x39, 0x39, 0x31, 0x32, 0x33, 0x31, 0x32,
    0x33, 0x35, 0x39, 0x35, 0x39, 0x5a, 0x30, 0x53, 0x31, 0x25, 0x30, 0x23, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x1c, 0x4d, 0x61,
    0x74, 0x74, 0x65, 0x72, 0x20, 0x44, 0x65, 0x76, 0x20, 0x44, 0x41, 0x43, 0x20, 0x30, 0x78, 0x46, 0x46, 0x46, 0x31, 0x2f, 0x30,
    0x78, 0x38, 0x30, 0x30, 0x31, 0x31, 0x14, 0x30, 0x12, 0x06, 0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0xa2, 0x7c, 0x02, 0x01,
    0x0c, 0x04, 0x46, 0x46, 0x46, 0x31, 0x31, 0x14, 0x30, 0x12, 0x06, 0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0xa2, 0x7c, 0x02,
    0x02, 0x0c, 0x04, 0x38, 0x30, 0x30, 0x31, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06,
    0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x46, 0x3a, 0xc6, 0x93, 0x42, 0x91, 0x0a, 0x0e,
    0x55, 0x88, 0xfc, 0x6f, 0xf5, 0x6b, 0xb6, 0x3e, 0x62, 0xec, 0xce, 0xcb, 0x14, 0x8f, 0x7d, 0x4e, 0xb0, 0x3e, 0xe5, 0x52, 0x60,
    0x14, 0x15, 0x76, 0x7d, 0x16, 0xa5, 0xc6, 0x63, 0xf7, 0x93, 0xe4, 0x91, 0x23, 0x26, 0x0b, 0x82, 0x97, 0xa7, 0xcd, 0x7e, 0x7c,
    0xfc, 0x7b, 0x31, 0x6b, 0x39, 0xd9, 0x8e, 0x90, 0xd2, 0x93, 0x77, 0x73, 0x8e, 0x82, 0xa3, 0x60, 0x30, 0x5e, 0x30, 0x0c, 0x06,
    0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x02, 0x30, 0x00, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff,
    0x04, 0x04, 0x03, 0x02, 0x07, 0x80, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x88, 0xdd, 0xe7, 0xb3,
    0x00, 0x38, 0x29, 0x32, 0xcf, 0xf7, 0x34, 0xc0, 0x46, 0x24, 0x81, 0x0f, 0x44, 0x16, 0x8a, 0x6f, 0x30, 0x1f, 0x06, 0x03, 0x55,
    0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x63, 0x54, 0x0e, 0x47, 0xf6, 0x4b, 0x1c, 0x38, 0xd1, 0x38, 0x84, 0xa4, 0x62,
    0xd1, 0x6c, 0x19, 0x5d, 0x8f, 0xfb, 0x3c, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x47,
    0x00, 0x30, 0x44, 0x02, 0x20, 0x01, 0x27, 0xa2, 0x7b, 0x4b, 0x44, 0x61, 0x0e, 0xe2, 0xfc, 0xdc, 0x4d, 0x2b, 0x78, 0x85, 0x56,
    0x36, 0x60, 0xbc, 0x0f, 0x76, 0xf1, 0x72, 0x19, 0xed, 0x6a, 0x08, 0xdf, 0xb2, 0xb3, 0xc1, 0xcd, 0x02, 0x20, 0x6b, 0x59, 0xe0,
    0xaf, 0x45, 0xf3, 0xeb, 0x2a, 0x85, 0xb9, 0x19, 0xd3, 0x57, 0x31, 0x52, 0x8c, 0x60, 0x28, 0xc4, 0x15, 0x23, 0x95, 0x45, 0xe1,
    0x08, 0xe4, 0xe5, 0x4e, 0x70, 0x97, 0x13, 0x53,
};


const uint8_t cert_declaration[541] = {
        0x30, 0x82, 0x02, 0x19, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x02, 0xa0, 0x82, 0x02, 0x0a, 0x30,
        0x82, 0x02, 0x06, 0x02, 0x01, 0x03, 0x31, 0x0d, 0x30, 0x0b, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02,
        0x01, 0x30, 0x82, 0x01, 0x71, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01, 0xa0, 0x82, 0x01, 0x62,
        0x04, 0x82, 0x01, 0x5e, 0x15, 0x24, 0x00, 0x01, 0x25, 0x01, 0xf1, 0xff, 0x36, 0x02, 0x05, 0x00, 0x80, 0x05, 0x01, 0x80,
        0x05, 0x02, 0x80, 0x05, 0x03, 0x80, 0x05, 0x04, 0x80, 0x05, 0x05, 0x80, 0x05, 0x06, 0x80, 0x05, 0x07, 0x80, 0x05, 0x08,
        0x80, 0x05, 0x09, 0x80, 0x05, 0x0a, 0x80, 0x05, 0x0b, 0x80, 0x05, 0x0c, 0x80, 0x05, 0x0d, 0x80, 0x05, 0x0e, 0x80, 0x05,
        0x0f, 0x80, 0x05, 0x10, 0x80, 0x05, 0x11, 0x80, 0x05, 0x12, 0x80, 0x05, 0x13, 0x80, 0x05, 0x14, 0x80, 0x05, 0x15, 0x80,
        0x05, 0x16, 0x80, 0x05, 0x17, 0x80, 0x05, 0x18, 0x80, 0x05, 0x19, 0x80, 0x05, 0x1a, 0x80, 0x05, 0x1b, 0x80, 0x05, 0x1c,
        0x80, 0x05, 0x1d, 0x80, 0x05, 0x1e, 0x80, 0x05, 0x1f, 0x80, 0x05, 0x20, 0x80, 0x05, 0x21, 0x80, 0x05, 0x22, 0x80, 0x05,
        0x23, 0x80, 0x05, 0x24, 0x80, 0x05, 0x25, 0x80, 0x05, 0x26, 0x80, 0x05, 0x27, 0x80, 0x05, 0x28, 0x80, 0x05, 0x29, 0x80,
        0x05, 0x2a, 0x80, 0x05, 0x2b, 0x80, 0x05, 0x2c, 0x80, 0x05, 0x2d, 0x80, 0x05, 0x2e, 0x80, 0x05, 0x2f, 0x80, 0x05, 0x30,
        0x80, 0x05, 0x31, 0x80, 0x05, 0x32, 0x80, 0x05, 0x33, 0x80, 0x05, 0x34, 0x80, 0x05, 0x35, 0x80, 0x05, 0x36, 0x80, 0x05,
        0x37, 0x80, 0x05, 0x38, 0x80, 0x05, 0x39, 0x80, 0x05, 0x3a, 0x80, 0x05, 0x3b, 0x80, 0x05, 0x3c, 0x80, 0x05, 0x3d, 0x80,
        0x05, 0x3e, 0x80, 0x05, 0x3f, 0x80, 0x05, 0x40, 0x80, 0x05, 0x41, 0x80, 0x05, 0x42, 0x80, 0x05, 0x43, 0x80, 0x05, 0x44,
        0x80, 0x05, 0x45, 0x80, 0x05, 0x46, 0x80, 0x05, 0x47, 0x80, 0x05, 0x48, 0x80, 0x05, 0x49, 0x80, 0x05, 0x4a, 0x80, 0x05,
        0x4b, 0x80, 0x05, 0x4c, 0x80, 0x05, 0x4d, 0x80, 0x05, 0x4e, 0x80, 0x05, 0x4f, 0x80, 0x05, 0x50, 0x80, 0x05, 0x51, 0x80,
        0x05, 0x52, 0x80, 0x05, 0x53, 0x80, 0x05, 0x54, 0x80, 0x05, 0x55, 0x80, 0x05, 0x56, 0x80, 0x05, 0x57, 0x80, 0x05, 0x58,
        0x80, 0x05, 0x59, 0x80, 0x05, 0x5a, 0x80, 0x05, 0x5b, 0x80, 0x05, 0x5c, 0x80, 0x05, 0x5d, 0x80, 0x05, 0x5e, 0x80, 0x05,
        0x5f, 0x80, 0x05, 0x60, 0x80, 0x05, 0x61, 0x80, 0x05, 0x62, 0x80, 0x05, 0x63, 0x80, 0x18, 0x24, 0x03, 0x16, 0x2c, 0x04,
        0x13, 0x5a, 0x49, 0x47, 0x32, 0x30, 0x31, 0x34, 0x32, 0x5a, 0x42, 0x33, 0x33, 0x30, 0x30, 0x30, 0x33, 0x2d, 0x32, 0x34,
        0x24, 0x05, 0x00, 0x24, 0x06, 0x00, 0x25, 0x07, 0x94, 0x26, 0x24, 0x08, 0x00, 0x18, 0x31, 0x7d, 0x30, 0x7b, 0x02, 0x01,
        0x03, 0x80, 0x14, 0x62, 0xfa, 0x82, 0x33, 0x59, 0xac, 0xfa, 0xa9, 0x96, 0x3e, 0x1c, 0xfa, 0x14, 0x0a, 0xdd, 0xf5, 0x04,
        0xf3, 0x71, 0x60, 0x30, 0x0b, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x30, 0x0a, 0x06, 0x08,
        0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x04, 0x47, 0x30, 0x45, 0x02, 0x20, 0x24, 0xe5, 0xd1, 0xf4, 0x7a, 0x7d,
        0x7b, 0x0d, 0x20, 0x6a, 0x26, 0xef, 0x69, 0x9b, 0x7c, 0x97, 0x57, 0xb7, 0x2d, 0x46, 0x90, 0x89, 0xde, 0x31, 0x92, 0xe6,
        0x78, 0xc7, 0x45, 0xe7, 0xf6, 0x0c, 0x02, 0x21, 0x00, 0xf8, 0xaa, 0x2f, 0xa7, 0x11, 0xfc, 0xb7, 0x9b, 0x97, 0xe3, 0x97,
        0xce, 0xda, 0x66, 0x7b, 0xae, 0x46, 0x4e, 0x2b, 0xd3, 0xff, 0xdf, 0xc3, 0xcc, 0xed, 0x7a, 0xa8, 0xca, 0x5f, 0x4c, 0x1a,
        0x7c,
    };

/* clang-format on */

/* Device attestation key ids */
#define DEV_ATTESTATION_KEY_SE05X_ID 0x7D300000
#define DEV_ATTESTATION_CERT_SE05X_ID 0x7D300001

/* Device attestation key ids (Used with internal sign) */
#define CD_DEV_ATTESTATION_KEY_SE05X_ID 0x7D300002
#define CD_ATTEST_INTERNAL_SIGN_TBS_ID 0x7D300003
#define NOCSR_DEV_ATTESTATION_KEY_SE05X_ID 0x7D300004
#define NOCSR_ATTEST_INTERNAL_SIGN_TBS_ID 0x7D300005

/* Device attestation data ids (for Cert decl) */
#define CD_START_CONTAINER_SE05X_ID 0x7D300006
#define CD_CERT_DECLARATION_TAG_SE05X_ID 0x7D300007
#define CD_CERT_DECLARATION_LEN_SE05X_ID 0x7D300008
#define CD_CERT_DECLARATION_DATA_SE05X_ID 0x7D300009
#define CD_ATTEST_NONCE_TAG_SE05X_ID 0x7D30000A
#define CD_ATTEST_NONCE_LEN_SE05X_ID 0x7D30000B
#define CD_ATTEST_NONCE_DATA_SE05X_ID 0x7D30000C
#define CD_TIME_STAMP_TAG_SE05X_ID 0x7D30000D
#define CD_TIME_STAMP_LEN_SE05X_ID 0x7D30000E
#define CD_TIME_STAMP_DATA_SE05X_ID 0x7D30000F
#define CD_END_CONTAINER_SE05X_ID 0x7D300010
#define CD_ATTEST_CHALLENGE_SE05X_ID 0x7D300011

/* Device attestation data ids (for CSR) */
#define NOCSR_START_CONTAINER_SE05X_ID 0x7D300012
#define NOCSR_CSR_TAG_SE05X_ID 0x7D300013
#define NOCSR_CSR_LEN_SE05X_ID 0x7D300014
#define NOCSR_CSR_DATA_SE05X_ID 0x7D300015
#define NOCSR_CSR_NONCE_TAG_SE05X_ID 0x7D300016
#define NOCSR_CSR_NONCE_LEN_SE05X_ID 0x7D300017
#define NOCSR_CSR_NONCE_DATA_SE05X_ID 0x7D300018
#define NOCSR_END_CONTAINER_SE05X_ID 0x7D300019
#define NOCSR_ATTEST_CHALLENGE_SE05X_ID 0x7D30001A

ex_sss_boot_ctx_t gex_sss_chip_ctx;

static sss_status_t se05x_set_key(const uint8_t *buffer, size_t bufferLen,
                                  size_t bitLen, sss_key_part_t keyPart,
                                  sss_cipher_type_t cipherType, uint32_t keyId,
                                  void *options, size_t optionsLen) {
  sss_status_t status = kStatus_SSS_Success;
  sss_object_t keyObj;

  status = sss_key_object_init(&keyObj, &gex_sss_chip_ctx.ks);
  if (status != kStatus_SSS_Success) {
    printf("Error in sss_key_object_init \n");
    return status;
  }

  status =
      sss_key_object_allocate_handle(&keyObj, keyId, keyPart, cipherType,
                                     bufferLen, kKeyObject_Mode_Persistent);
  if (status != kStatus_SSS_Success) {
    printf("Error in sss_key_object_allocate_handle \n");
    return status;
  }

  status = sss_key_store_set_key(&gex_sss_chip_ctx.ks, &keyObj, buffer,
                                 bufferLen, bitLen, options, optionsLen);
  if (status != kStatus_SSS_Success) {
    printf("Error in sss_key_store_set_key \n");
  }

  return status;
}

#if SSS_HAVE_APPLET_SE051_H
void fill_tbs_buffer(uint8_t *buffer, uint32_t keyid, uint8_t *offset) {
  buffer[*offset + 0] = (uint8_t)(keyid >> 24) & 0xFF;
  buffer[*offset + 1] = (uint8_t)(keyid >> 16) & 0xFF;
  buffer[*offset + 2] = (uint8_t)(keyid >> 8) & 0xFF;
  buffer[*offset + 3] = (uint8_t)keyid & 0xFF;
  *offset = (uint8_t)(*offset + 4);
}
#endif

void se05x_dev_attest_key_prov(void) {
  sss_status_t status = kStatus_SSS_Success;
  const char *portName = nullptr;

  memset(&gex_sss_chip_ctx, 0, sizeof(gex_sss_chip_ctx));

  status = ex_sss_boot_connectstring(0, NULL, &portName);
  if (kStatus_SSS_Success != status) {
    printf("se05x error: %s\n", "ex_sss_boot_connectstring failed");
    return;
  }

  status = ex_sss_boot_open(&gex_sss_chip_ctx, portName);
  if (kStatus_SSS_Success != status) {
    printf("se05x error: %s\n", "ex_sss_boot_open failed");
    return;
  }

  status = ex_sss_key_store_and_object_init(&gex_sss_chip_ctx);
  if (kStatus_SSS_Success != status) {
    printf("se05x error: %s\n", "ex_sss_key_store_and_object_init failed");
    return;
  }

  /* Set device attestation keyPair */
  printf("Set DA key at location - %02x \n", DEV_ATTESTATION_KEY_SE05X_ID);
  status = se05x_set_key(keyPairData, sizeof(keyPairData), 256,
                         kSSS_KeyPart_Pair, kSSS_CipherType_EC_NIST_P,
                         DEV_ATTESTATION_KEY_SE05X_ID, NULL, 0);
  if (status != kStatus_SSS_Success) {
    printf("Error in se05x_set_key \n");
    return;
  }

  /* Set device attestation cert */
  printf("Set DA cert at location - %02x \n", DEV_ATTESTATION_CERT_SE05X_ID);
  status = se05x_set_key(dev_attest_cert, sizeof(dev_attest_cert),
                         sizeof(dev_attest_cert) * 8, kSSS_KeyPart_Default,
                         kSSS_CipherType_Certificate,
                         DEV_ATTESTATION_CERT_SE05X_ID, NULL, 0);
  if (status != kStatus_SSS_Success) {
    printf("Error in se05x_set_key \n");
    return;
  }

  /* Set Device attestation keyPair, cert (Used for internal signing) */
#if SSS_HAVE_APPLET_SE051_H
  {
    uint8_t tempBuf[2] = {
        0,
    };
    uint8_t cd_tbsData[128] = {
        0,
    };
    uint8_t csr_tbsData[128] = {
        0,
    };
    uint8_t offset = 0;

    static sss_policy_u commonPol;
    commonPol.type = KPolicy_Common;
    commonPol.auth_obj_id = 0;
    commonPol.policy.common.req_Sm = 0;
    commonPol.policy.common.can_Delete = 1;
    commonPol.policy.common.can_Read = 0;
    commonPol.policy.common.can_Write = 1;

    static sss_policy_u cd_intSign_withPol;
    cd_intSign_withPol.type = KPolicy_Internal_Sign;
    cd_intSign_withPol.auth_obj_id = 0;
    cd_intSign_withPol.policy.tbsItemList.tbsItemList_KeyId =
        CD_ATTEST_INTERNAL_SIGN_TBS_ID;

    static sss_policy_u csr_intSign_withPol;
    csr_intSign_withPol.type = KPolicy_Internal_Sign;
    csr_intSign_withPol.auth_obj_id = 0;
    csr_intSign_withPol.policy.tbsItemList.tbsItemList_KeyId =
        NOCSR_ATTEST_INTERNAL_SIGN_TBS_ID;

    static sss_policy_u asymmkey_withPol;
    asymmkey_withPol.type = KPolicy_Asym_Key;
    asymmkey_withPol.auth_obj_id = 0;
    asymmkey_withPol.policy.asymmkey.can_Sign = 1;
    asymmkey_withPol.policy.asymmkey.can_Verify = 1;

    sss_policy_t cd_policy_for_ec_key;
    cd_policy_for_ec_key.nPolicies = 3;
    cd_policy_for_ec_key.policies[0] = &commonPol;
    cd_policy_for_ec_key.policies[1] = &cd_intSign_withPol;
    cd_policy_for_ec_key.policies[2] = &asymmkey_withPol;

    sss_policy_t csr_policy_for_ec_key;
    csr_policy_for_ec_key.nPolicies = 3;
    csr_policy_for_ec_key.policies[0] = &commonPol;
    csr_policy_for_ec_key.policies[1] = &csr_intSign_withPol;
    csr_policy_for_ec_key.policies[2] = &asymmkey_withPol;

    printf("Start provisioning data for DA internal sign (to be used only with "
           "SE051H) \n");

    /* Create tbs binFile data (CD signing) */
    fill_tbs_buffer(cd_tbsData, CD_START_CONTAINER_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_CERT_DECLARATION_TAG_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_CERT_DECLARATION_LEN_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_CERT_DECLARATION_DATA_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_ATTEST_NONCE_TAG_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_ATTEST_NONCE_LEN_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_ATTEST_NONCE_DATA_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_TIME_STAMP_TAG_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_TIME_STAMP_LEN_SE05X_ID, &offset);
    // fill_tbs_buffer(cd_tbsData, CD_TIME_STAMP_DATA_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_END_CONTAINER_SE05X_ID, &offset);
    fill_tbs_buffer(cd_tbsData, CD_ATTEST_CHALLENGE_SE05X_ID, &offset);

    /* Set tbs binFile */
    printf("Set TBS item list binFile at location - %02x \n",
           CD_ATTEST_INTERNAL_SIGN_TBS_ID);
    status = se05x_set_key(cd_tbsData, offset, offset * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_ATTEST_INTERNAL_SIGN_TBS_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Set device attestation keyPair (For internal sign) */
    printf("Set device attestation keyPair (For internal sign) - %02x \n",
           CD_DEV_ATTESTATION_KEY_SE05X_ID);
    status = se05x_set_key(keyPairData, sizeof(keyPairData), 256,
                           kSSS_KeyPart_Pair, kSSS_CipherType_EC_NIST_P,
                           CD_DEV_ATTESTATION_KEY_SE05X_ID,
                           &cd_policy_for_ec_key, sizeof(cd_policy_for_ec_key));
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Create tbs binFile data (NOCSR signing) */
    offset = 0;
    fill_tbs_buffer(csr_tbsData, NOCSR_START_CONTAINER_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_CSR_TAG_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_CSR_LEN_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_CSR_DATA_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_CSR_NONCE_TAG_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_CSR_NONCE_LEN_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_CSR_NONCE_DATA_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_END_CONTAINER_SE05X_ID, &offset);
    fill_tbs_buffer(csr_tbsData, NOCSR_ATTEST_CHALLENGE_SE05X_ID, &offset);

    /* Set tbs binFile */
    printf("Set TBS item list binFile at location - %02x \n",
           NOCSR_ATTEST_INTERNAL_SIGN_TBS_ID);
    status = se05x_set_key(csr_tbsData, offset, offset * 8,
                           kSSS_KeyPart_Default, kSSS_CipherType_Certificate,
                           NOCSR_ATTEST_INTERNAL_SIGN_TBS_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Set device attestation keyPair (For internal sign) */
    printf("Set device attestation keyPair (For internal sign) - %02x \n",
           NOCSR_DEV_ATTESTATION_KEY_SE05X_ID);
    status = se05x_set_key(
        keyPairData, sizeof(keyPairData), 256, kSSS_KeyPart_Pair,
        kSSS_CipherType_EC_NIST_P, NOCSR_DEV_ATTESTATION_KEY_SE05X_ID,
        &csr_policy_for_ec_key, sizeof(csr_policy_for_ec_key));
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Attestation sign message (CD) - Create required binary objects for DA
     * sign */

    /* Set start container */
    printf("Create binary file for start container at location - %02x \n",
           CD_START_CONTAINER_SE05X_ID);
    tempBuf[0] = 0x15;
    status = se05x_set_key(tempBuf, 1, 1 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_START_CONTAINER_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Set cert declaration tag */
    printf("Create binary file for cert decl tag at location - %02x \n",
           CD_CERT_DECLARATION_TAG_SE05X_ID);
    tempBuf[0] = 0x31; /* Control Byte */
    tempBuf[1] = 0x01; /* Tag */
    status = se05x_set_key(tempBuf, 2, 2 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_CERT_DECLARATION_TAG_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* Set cert declaration length */
    printf("Create binary file for cert decl length at location - %02x \n",
           CD_CERT_DECLARATION_LEN_SE05X_ID);
    tempBuf[0] = (uint8_t)(sizeof(cert_declaration) & 0xFF);
    tempBuf[1] = (uint8_t)((sizeof(cert_declaration) >> 8) & 0xFF);
    status = se05x_set_key(tempBuf, 2, 2 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_CERT_DECLARATION_LEN_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* Set cert declaration data */
    printf("Create binary file for cert decl data at location - %02x \n",
           CD_CERT_DECLARATION_DATA_SE05X_ID);
    status = se05x_set_key(cert_declaration, sizeof(cert_declaration),
                           sizeof(cert_declaration) * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_CERT_DECLARATION_DATA_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Set attestation nonce tag */
    printf("Create binary file for attestation nonce tag at location - %02x \n",
           CD_ATTEST_NONCE_TAG_SE05X_ID);
    tempBuf[0] = 0x30; /* Control Byte */
    tempBuf[1] = 0x02; /* Tag */
    status = se05x_set_key(tempBuf, 2, 2 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_ATTEST_NONCE_TAG_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* Set attestation nonce length */
    printf(
        "Create binary file for attestation nonce length at location - %02x \n",
        CD_ATTEST_NONCE_LEN_SE05X_ID);
    tempBuf[0] = 32; /* Nonce length */
    status = se05x_set_key(tempBuf, 1, 1 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_ATTEST_NONCE_LEN_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* Attestation nonce data is set during runtime at location -
     * ATTEST_NONCE_DATA_SE05X_ID */

    /* Set time stamp tag */
    printf("Create binary file for time stamp tag at location - %02x \n",
           CD_TIME_STAMP_TAG_SE05X_ID);
    tempBuf[0] = 0x24; /* Control Byte */
    tempBuf[1] = 0x03; /* Tag */
    status = se05x_set_key(tempBuf, 2, 2 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_TIME_STAMP_TAG_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* Time stamp length and data is set during runtime at location -
     * CD_TIME_STAMP_LEN_SE05X_ID and CD_TIME_STAMP_DATA_SE05X_ID*/

    /* Set end container */
    printf("Create binary file for end container at location - %02x \n",
           CD_END_CONTAINER_SE05X_ID);
    tempBuf[0] = 0x18;
    status = se05x_set_key(tempBuf, 1, 1 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           CD_END_CONTAINER_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Attestation challeng is set during run time at location
     * CD_ATTEST_CHALLENGE_SE05X_ID */

    /* Attestation sign message - Create required binary objects for NOCSR sign
     */

    /* Set start container */
    printf("Create binary file for start container at location - %02x \n",
           NOCSR_START_CONTAINER_SE05X_ID);
    tempBuf[0] = 0x15;
    status = se05x_set_key(tempBuf, 1, 1 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           NOCSR_START_CONTAINER_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Set NOCSR tag */
    printf("Create binary file for CSR tag at location - %02x \n",
           NOCSR_CSR_TAG_SE05X_ID);
    tempBuf[0] = 0x30; /* Control Byte */
    tempBuf[1] = 0x01; /* Tag */
    status = se05x_set_key(tempBuf, 2, 2 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate, NOCSR_CSR_TAG_SE05X_ID,
                           NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* NOCSR length and data will be updated at runtime at location -
     * NOCSR_CSR_LEN_SE05X_ID and NOCSR_CSR_DATA_SE05X_ID */

    /* Set nocsr nonce tag */
    printf("Create binary file for CSR nonce tag at location - %02x \n",
           NOCSR_CSR_NONCE_TAG_SE05X_ID);
    tempBuf[0] = 0x30; /* Control Byte */
    tempBuf[1] = 0x02; /* Tag */
    status = se05x_set_key(tempBuf, 2, 2 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           NOCSR_CSR_NONCE_TAG_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* Set nocsr nonce length */
    printf("Create binary file for CSR nonce length at location - %02x \n",
           NOCSR_CSR_NONCE_LEN_SE05X_ID);
    tempBuf[0] = 32; /* Nonce length */
    status = se05x_set_key(tempBuf, 1, 1 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           NOCSR_CSR_NONCE_LEN_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }
    /* NOCSR nonce data is set during runtime at location -
     * ATTEST_NONCE_DATA_SE05X_ID */

    /* Set end container */
    printf("Create binary file for end container at location - %02x \n",
           NOCSR_END_CONTAINER_SE05X_ID);
    tempBuf[0] = 0x18;
    status = se05x_set_key(tempBuf, 1, 1 * 8, kSSS_KeyPart_Default,
                           kSSS_CipherType_Certificate,
                           NOCSR_END_CONTAINER_SE05X_ID, NULL, 0);
    if (status != kStatus_SSS_Success) {
      printf("Error in se05x_set_key \n");
      return;
    }

    /* Attestation challeng is set during run time at location
     * CSR_ATTEST_CHALLENGE_SE05X_ID */
  }
#endif

  printf("Attestation key and cert Provision successful \n");
  return;
}
