/*
 * Class A LoRaWAN sample application
 */

#include <zephyr/device.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/drivers/gpio.h>

/* Customize based on network configuration */

#define LORAWAN_DEV_EUI			{ 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0xE4, 0x41 }

#define LORAWAN_JOIN_EUI		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

#define LORAWAN_APP_KEY			{ 0x37, 0xA1, 0xEC, 0x8C, 0x17, 0x9B, 0x82, 0xA9, 0x3F, 0x68, 0x05, 0x9A, 0x1B, 0xE4, 0xC4, 0xEB }

#define DELAY K_MSEC(5000)
#define LORAWAN_JOIN_RETRY_DELAY K_MSEC(2000)

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lorawan_class_a);

/* Dataset */

char original_data[] = {0xca, 0xfe, 0xc0, 0xc0, 0xff};
char modified_data[] = {0xc0, 0xc0, 0x10, 0xc0, 0xff};

char sensor_data_orig[] = {0xDB, 0x0F, 0x49, 0x41, 0xDB, 0x0F, 0x49, 0x40, 0x01, 0x00};
char sensor_data_mod[] = {0xDC, 0x10, 0x4A, 0x42, 0xDC, 0x10, 0x4A, 0x41, 0x02, 0x00};

char* data = sensor_data_orig;

/* The devicetree node identifier for the "sw0" alias. */
#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

/* The devicetree node identifier for the "lora0" alias. */
#define LORA_NODE DT_ALIAS(lora0)
#if !DT_NODE_HAS_STATUS(LORA_NODE, okay)
#error "Unsupported board: lora0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	//next message payload will change
	data = sensor_data_mod;
}

/* A build error on this line means your board is unsupported. */
static struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);


static void dl_callback(uint8_t port, bool data_pending,
			int16_t rssi, int8_t snr,
			uint8_t len, const uint8_t *data)
{
	LOG_INF("Port %d, Pending %d, RSSI %ddB, SNR %ddBm", port, data_pending, rssi, snr);
	if (data != NULL) 
	{
		
		LOG_HEXDUMP_INF(data, len, "Payload: ");
		
		//TODO this is the correct place to decode incoming messages and act. 
		if (memcmp(data, modified_data, sizeof(modified_data)) == 0)
		{
			LOG_INF(" 0xCOCO10C0FF command received ");
			gpio_pin_toggle_dt(&led);
		}
		else
		{
			LOG_INF(" unknown command received ");
		}
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
	int ret;

	//button config
	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	//led config
	if (led.port && !device_is_ready(led.port)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}
	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

	//lora device config
	const struct device *lora_dev;
	struct lorawan_join_config join_cfg;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t join_eui[] = LORAWAN_JOIN_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;	
	int joined = -1;

	struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = dl_callback
	};

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		LOG_ERR("%s: device not ready.", lora_dev->name);
		return 0;
	}

	//lorawan config
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
	if (ret < 0) {0
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
	
	//joining using a "retry forever policy" to avoid gateway and network issues.
	while (joined < 0)
	{
		joined = lorawan_join(&join_cfg);
		if (joined < 0) 
		{
			LOG_ERR("lorawan_join_network failed: %d ", ret);
			k_sleep(LORAWAN_JOIN_RETRY_DELAY);		
			LOG_INF("lorawan_join_network retrying");
		}
		else
		{
			LOG_INF("Joining network over OTAA OK");
		}
	}


#ifdef CONFIG_LORAWAN_APP_CLOCK_SYNC
	lorawan_clock_sync_run();
#endif

	LOG_INF("Starting superloop");
	while (1) {

		//reading node data
		
		//TODO
		
		//sending data
		ret = lorawan_send(2, data, sizeof(sensor_data_orig), LORAWAN_MSG_CONFIRMED);

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

		//restoring data, if the button is presseed, then the payload will change
		data = sensor_data_orig;

		//TODO entering to low power mode	
		k_sleep(DELAY);
	}
}
