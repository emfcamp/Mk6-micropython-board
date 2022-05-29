#include <stddef.h>
#include <stdint.h>

typedef struct arbitrary_size_container {
    size_t size;
    uint8_t *data;
} arbitrary_size_container;

arbitrary_size_container get_der_certificate(uint8_t handle);
arbitrary_size_container get_signature(uint8_t signature_input[]);
void *set_pubkey(uint8_t handle, uint8_t *target);
uint8_t allocate_handle();