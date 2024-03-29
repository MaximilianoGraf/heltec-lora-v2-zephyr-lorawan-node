/*
 * Class A LoRaWAN sample application
 *
 * Copyright (c) 2020 Manivannan Sadhasivam <mani@kernel.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/lora.h>
/* Customize based on network configuration */

#define LORAWAN_DEV_EUI			{ 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05,\
					  0xE4, 0x41 }
#define LORAWAN_JOIN_EUI		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
					  0x00, 0x00 }

#define LORAWAN_APP_KEY			{ 0x37, 0xa1, 0xec, 0x8c, 0x17, 0x9b,\
					  0x82, 0xa9, 0x3f, 0x68, 0x05, 0x9a,\
					  0x1b, 0xe4, 0xc4, 0xeb }

#define DELAY K_MSEC(5000)

#define CONFIG_LORAMAC_REGION_US915 1

#define LOG_LEVEL 4
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lorawan_class_a);

char data[] = {0xca, 0xfe, 0xc0, 0xc0, 0xff};

static void dl_callback(uint8_t port, bool data_pending,
			int16_t rssi, int8_t snr,
			uint8_t len, const uint8_t *data)
{
	LOG_INF("Port %d, Pending %d, RSSI %ddB, SNR %ddBm", port, data_pending, rssi, snr);
	if (data) {
		LOG_HEXDUMP_INF(data, len, "Payload: ");
	}
}

static void lorwan_datarate_changed(enum lorawan_datarate dr)
{
	uint8_t unused, max_size;

	lorawan_get_payload_sizes(&unused, &max_size);
	LOG_INF("New Datarate: DR_%d, Max Payload %d", dr, max_size);
}

int main(void)
{
	const struct device *lora_dev;
	struct lorawan_join_config join_cfg;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t join_eui[] = LORAWAN_JOIN_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;
	int ret;

	struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = dl_callback
	};

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		LOG_ERR("%s: device not ready.", lora_dev->name);
		return 0;
	}

	// struct lora_modem_config config;
	// config.frequency = 868000000;
	// config.bandwidth = BW_125_KHZ;
	// config.datarate = SF_7;
	// config.preamble_len = 8;
	// config.coding_rate = CR_4_5;
	// config.iq_inverted = false;
	// config.public_network = true;
	// config.tx_power = 4;
	// config.tx = true;

	// ret = lora_config(lora_dev, &config);
	// if (ret < 0) {
	// 	LOG_ERR("LoRa config failed");
	// 	return;
	// }

#if defined(CONFIG_LORAMAC_REGION_US915)
	/* If more than one region Kconfig is selected, app should set region
	 * before calling lorawan_start()
	 */
	ret = lorawan_set_region(LORAWAN_REGION_US915);
	if (ret < 0) {
		LOG_ERR("lorawan_set_region failed: %d", ret);
		return 0;
	}
#endif
#if defined(CONFIG_LORAMAC_REGION_EU868)
	/* If more than one region Kconfig is selected, app should set region
	 * before calling lorawan_start()
	 */
	ret = lorawan_set_region(LORAWAN_REGION_EU868);
	if (ret < 0) {
		LOG_ERR("lorawan_set_region failed: %d", ret);
		return 0;
	}
#endif

	ret = lorawan_start();
	if (ret < 0) {
		LOG_ERR("lorawan_start failed: %d", ret);
		return 0;
	}

	lorawan_register_downlink_callback(&downlink_cb);
	lorawan_register_dr_changed_callback(lorwan_datarate_changed);

	join_cfg.mode = LORAWAN_ACT_OTAA;
	join_cfg.dev_eui = dev_eui;
	join_cfg.otaa.join_eui = join_eui;
	join_cfg.otaa.app_key = app_key;
	join_cfg.otaa.nwk_key = app_key;
	join_cfg.otaa.dev_nonce = 0u;

	LOG_INF("Joining network over OTAA");
	ret = lorawan_join(&join_cfg);
	if (ret < 0) {
		LOG_ERR("lorawan_join_network failed: %d", ret);
		return 0;
	}

#ifdef CONFIG_LORAWAN_APP_CLOCK_SYNC
	lorawan_clock_sync_run();
#endif

	LOG_INF("Sending data...");
	while (1) {
		ret = lorawan_send(2, data, sizeof(data),
				   LORAWAN_MSG_CONFIRMED);

		/*
		 * Note: The stack may return -EAGAIN if the provided data
		 * length exceeds the maximum possible one for the region and
		 * datarate. But since we are just sending the same data here,
		 * we'll just continue.
		 */
		if (ret == -EAGAIN) {
			LOG_ERR("lorawan_send failed: %d. Continuing...", ret);
			k_sleep(DELAY);
			continue;
		}

		if (ret < 0) {
			LOG_ERR("lorawan_send failed: %d", ret);
			continue;
		}

		LOG_INF("Data sent!");
		k_sleep(DELAY);
	}
}
