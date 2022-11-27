#include "u2f_crypto.h"
#include "esp_log.h"
#include "atca_iface.h"
#include "atca_basic.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static const char *TAG = "tidalU2F";


arbitrary_size_container get_attestation_certificate() {
    unsigned char attestation_der[] = {
    0x30, 0x82, 0x02, 0x4a, 0x30, 0x82, 0x01, 0xef, 0x02, 0x14, 0x11, 0xca,
    0xcc, 0x12, 0x88, 0x94, 0x28, 0x5c, 0xa8, 0x14, 0xc5, 0xd7, 0x9b, 0xb6,
    0x6f, 0x29, 0xb4, 0x73, 0xb8, 0xdd, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86,
    0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x81, 0xa5, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x47, 0x42, 0x31, 0x0f,
    0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x06, 0x4c, 0x6f, 0x6e,
    0x64, 0x6f, 0x6e, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x07,
    0x0c, 0x06, 0x4c, 0x6f, 0x6e, 0x64, 0x6f, 0x6e, 0x31, 0x22, 0x30, 0x20,
    0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x19, 0x45, 0x6c, 0x65, 0x63, 0x74,
    0x72, 0x6f, 0x6d, 0x61, 0x67, 0x6e, 0x65, 0x74, 0x69, 0x63, 0x20, 0x46,
    0x69, 0x65, 0x6c, 0x64, 0x20, 0x4c, 0x74, 0x64, 0x31, 0x12, 0x30, 0x10,
    0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x09, 0x42, 0x61, 0x64, 0x67, 0x65,
    0x54, 0x65, 0x61, 0x6d, 0x31, 0x1a, 0x30, 0x18, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x0c, 0x11, 0x54, 0x69, 0x44, 0x41, 0x4c, 0x20, 0x41, 0x74, 0x74,
    0x65, 0x73, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x31, 0x20, 0x30, 0x1e,
    0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16,
    0x11, 0x62, 0x61, 0x64, 0x67, 0x65, 0x40, 0x65, 0x6d, 0x66, 0x63, 0x61,
    0x6d, 0x70, 0x2e, 0x6f, 0x72, 0x67, 0x30, 0x20, 0x17, 0x0d, 0x32, 0x32,
    0x31, 0x30, 0x33, 0x30, 0x31, 0x37, 0x34, 0x32, 0x34, 0x34, 0x5a, 0x18,
    0x0f, 0x32, 0x30, 0x35, 0x37, 0x31, 0x30, 0x33, 0x30, 0x31, 0x37, 0x34,
    0x32, 0x34, 0x34, 0x5a, 0x30, 0x81, 0xa5, 0x31, 0x0b, 0x30, 0x09, 0x06,
    0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x47, 0x42, 0x31, 0x0f, 0x30, 0x0d,
    0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x06, 0x4c, 0x6f, 0x6e, 0x64, 0x6f,
    0x6e, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x06,
    0x4c, 0x6f, 0x6e, 0x64, 0x6f, 0x6e, 0x31, 0x22, 0x30, 0x20, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x19, 0x45, 0x6c, 0x65, 0x63, 0x74, 0x72, 0x6f,
    0x6d, 0x61, 0x67, 0x6e, 0x65, 0x74, 0x69, 0x63, 0x20, 0x46, 0x69, 0x65,
    0x6c, 0x64, 0x20, 0x4c, 0x74, 0x64, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03,
    0x55, 0x04, 0x0b, 0x0c, 0x09, 0x42, 0x61, 0x64, 0x67, 0x65, 0x54, 0x65,
    0x61, 0x6d, 0x31, 0x1a, 0x30, 0x18, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
    0x11, 0x54, 0x69, 0x44, 0x41, 0x4c, 0x20, 0x41, 0x74, 0x74, 0x65, 0x73,
    0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x31, 0x20, 0x30, 0x1e, 0x06, 0x09,
    0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x11, 0x62,
    0x61, 0x64, 0x67, 0x65, 0x40, 0x65, 0x6d, 0x66, 0x63, 0x61, 0x6d, 0x70,
    0x2e, 0x6f, 0x72, 0x67, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86,
    0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x70, 0x33, 0xf2, 0x6e, 0x2d,
    0xc2, 0xc0, 0xd1, 0xf8, 0xa7, 0x03, 0x3e, 0xa6, 0x4e, 0x47, 0x1a, 0xe1,
    0x02, 0xfe, 0xf3, 0xdb, 0xc9, 0xbd, 0xb7, 0x3e, 0x69, 0x59, 0xa1, 0xa5,
    0x49, 0x8b, 0x8e, 0x9a, 0x2a, 0xbd, 0xb2, 0xf5, 0xa8, 0xcb, 0xa0, 0x4a,
    0xb4, 0x6a, 0x84, 0x32, 0x09, 0x80, 0x95, 0x21, 0x99, 0x3d, 0x29, 0xe1,
    0xe0, 0xc2, 0xdf, 0xbd, 0xd5, 0x08, 0x22, 0x7a, 0xd8, 0x2d, 0xe2, 0x30,
    0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03,
    0x49, 0x00, 0x30, 0x46, 0x02, 0x21, 0x00, 0xd6, 0x6b, 0x12, 0x6c, 0xa9,
    0xf4, 0x56, 0x80, 0xf5, 0x58, 0xb9, 0xc3, 0xdf, 0xd5, 0xb2, 0xdf, 0x49,
    0x25, 0x44, 0x55, 0x81, 0xa8, 0x96, 0x88, 0x6b, 0x49, 0x03, 0x0b, 0xdb,
    0xa6, 0x75, 0x7f, 0x02, 0x21, 0x00, 0xa2, 0x46, 0x8d, 0xbd, 0x42, 0xda,
    0x2d, 0xda, 0x09, 0x0d, 0x68, 0xd8, 0x1b, 0x9f, 0x00, 0xea, 0xef, 0xfa,
    0x2f, 0x77, 0x1b, 0xc7, 0xe2, 0x36, 0x8c, 0x0b, 0xae, 0x34, 0x6b, 0x8c,
    0x4d, 0xc5
    };
    unsigned int attestation_der_len = 590;

    ESP_LOGI(TAG, "Allocating %d bytes", attestation_der_len);
    arbitrary_size_container response_data = {
        .size=attestation_der_len,
        .data=malloc(attestation_der_len),
    };
    memcpy(response_data.data, &attestation_der, attestation_der_len);
    return response_data;
}

arbitrary_size_container get_signature(uint8_t handle, uint8_t signature_input[]) {
    uint8_t digest[32] = { 0 };
    ESP_LOGI(TAG, "Calculating digest");
    atcab_hw_sha2_256(signature_input, sizeof(signature_input), digest);
    ESP_LOGI(TAG, "Allocating %d bytes", 73);
    arbitrary_size_container response_data = {
        .size=64,
        .data=malloc(64),
    };
    ESP_LOGI(TAG, "Signing data");
    atcab_sign(handle, &digest, response_data.data);
    //memset(response_data.data, 0xF0, 73);
    return response_data;
}

void set_counter(uint8_t handle, uint32_t *target) {
    //atcab_counter_increment(handle, target);
}

void set_pubkey(uint8_t handle, uint8_t *target) {
    atcab_genkey(handle, target);
}

uint8_t allocate_handle() {
    return 6;
}