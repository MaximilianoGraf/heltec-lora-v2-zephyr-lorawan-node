#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/* 1 : /soc/rtc@3ff48000:
 * Supported:
 *    - /soc/uart@3ff40000
 *    - /soc/spi@3ff65000
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_5[5];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_5[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 5, 6, DEVICE_HANDLE_ENDS };

/* 2 : /soc/gpio/gpio@3ff44800:
 * Supported:
 *    - /soc/spi@3ff65000/lora@0
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_66[4];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_66[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 7, DEVICE_HANDLE_ENDS };

/* 3 : /soc/gpio/gpio@3ff44000:
 * Supported:
 *    - /soc/spi@3ff65000
 *    - /soc/spi@3ff65000/lora@0
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_11[5];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_11[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 6, 7, DEVICE_HANDLE_ENDS };

/* 4 : /soc/trng@3ff75144:
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_50[3];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_50[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 5 : /soc/uart@3ff40000:
 * Direct Dependencies:
 *    - /soc/rtc@3ff48000
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_51[4];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_51[] = { 1, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 6 : /soc/spi@3ff65000:
 * Direct Dependencies:
 *    - /soc/rtc@3ff48000
 *    - /soc/gpio/gpio@3ff44000
 * Supported:
 *    - /soc/spi@3ff65000/lora@0
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_65[6];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_65[] = { 1, 3, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 7, DEVICE_HANDLE_ENDS };

/* 7 : /soc/spi@3ff65000/lora@0:
 * Direct Dependencies:
 *    - /soc/gpio/gpio@3ff44800
 *    - /soc/gpio/gpio@3ff44000
 *    - /soc/spi@3ff65000
 */
extern const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2"))) __devicehdl_dts_ord_67[6];
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_67[] = { 2, 3, 6, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };
