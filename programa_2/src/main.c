#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_if.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>

// Configura tus credenciales Wi-Fi
#define WIFI_SSID "Redmi Note 13"
#define WIFI_PASS "890890890"

// LED conectado al alias led0
#define LED_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

// Semáforo para esperar IP
static struct k_sem ip_ready;

static void wifi_handler(struct net_mgmt_event_callback *cb,
                         uint32_t event, struct net_if *iface)
{
	if (event == NET_EVENT_IPV4_ADDR_ADD) {
		char ip[NET_IPV4_ADDR_LEN];
		net_addr_ntop(AF_INET,
		              &iface->config.ip.ipv4->unicast[0].address.in_addr,
		              ip, sizeof(ip));
		printf("IP obtenida: %s\n", ip);
		gpio_pin_set_dt(&led, 1);  // Enciende LED
		k_sem_give(&ip_ready);     // Señala que IP está lista
	}
}

void main(void)
{
	struct net_if *iface = net_if_get_default();

	// Inicializa LED
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	// Prepara evento para IP
	static struct net_mgmt_event_callback cb;
	k_sem_init(&ip_ready, 0, 1);
	net_mgmt_init_event_callback(&cb, wifi_handler, NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&cb);

	// Conexión Wi-Fi
	struct wifi_connect_req_params params = {
		.ssid = WIFI_SSID,
		.ssid_length = strlen(WIFI_SSID),
		.psk = WIFI_PASS,
		.psk_length = strlen(WIFI_PASS),
		.security = WIFI_SECURITY_TYPE_PSK,
	};
	printf("Conectando a Wi-Fi...\n");
	net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));

	// Espera hasta obtener IP (máx 10 segundos)
	if (k_sem_take(&ip_ready, K_SECONDS(10)) == 0) {
		printf("¡Conectado con éxito!\n");
	} else {
		printf("Error: no se obtuvo IP.\n");
	}
}

