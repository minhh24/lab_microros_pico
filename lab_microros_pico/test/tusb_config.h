#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// 1. Cấu hình Device Mode
#define CFG_TUSB_RHPORT0_MODE       (OPT_MODE_DEVICE)

// 2. Cấu hình Class CDC (Serial)
#define CFG_TUD_CDC                 1
#define CFG_TUD_CDC_RX_BUFSIZE      256
#define CFG_TUD_CDC_TX_BUFSIZE      256

// 3. Các cấu hình khác để tránh lỗi
#define CFG_TUSB_OS                 OPT_OS_NONE
#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN          __attribute__ ((aligned(4)))

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */