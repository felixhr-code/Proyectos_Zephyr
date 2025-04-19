#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_ip.h>
#include <string.h>

#define WIFI_SSID "Redmi Note 13"
#define WIFI_PASS "890890890"
#define TIMEOUT_MS 10000

static struct net_mgmt_event_callback wifi_cb;
static bool conectado = false;

static void wifi_event_handler(struct net_mgmt_event_callback *cb,
                               uint32_t event, struct net_if *iface)
{
	if (event == NET_EVENT_IPV4_ADDR_ADD) {
		struct net_if_addr *ifaddr = net_if_ipv4_get_global_addr(iface, NET_ADDR_DHCP);
		if (ifaddr) {
			char ip[NET_IPV4_ADDR_LEN];
			net_addr_ntop(AF_INET, &ifaddr->address.in_addr, ip, sizeof(ip));
			printk("¡Conectado a Wi-Fi! IP: %s\n", ip);
			conectado = true;
		}
	}
}

void main(void)
{
	printk("Iniciando conexión Wi-Fi (ESP32 + Zephyr)...\n");

	struct net_if *iface = net_if_get_default();

	net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler,
	                             NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&wifi_cb);

	struct wifi_connect_req_params wifi = {
		.ssid = WIFI_SSID,
		.ssid_length = strlen(WIFI_SSID),
		.psk = WIFI_PASS,
		.psk_length = strlen(WIFI_PASS),
		.security = WIFI_SECURITY_TYPE_PSK,
		.channel = WIFI_CHANNEL_ANY,
		.timeout = SYS_FOREVER_MS,
	};

	int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi, sizeof(wifi));
	if (ret != 0) {
		printk("Error al conectar: %d\n", ret);
		return;
	}

	printk("Esperando dirección IP...\n");

	for (int i = 0; i < TIMEOUT_MS / 500_

