#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>

#define GPIO0_NODE DT_NODELABEL(gpio0)
#define LED_PIN 2

void main(void){

	const struct device *gpio_dev = DEVICE_DT_GET(GPIO0_NODE);
	
	if (!device_is_ready(gpio_dev)){
		printk("No se puede escontrar el dispositivo GPIO\n");
		return;
	} 

	int ret = gpio_pin_configure(gpio_dev, LED_PIN, GPIO_OUTPUT_ACTIVE);
	if (ret <0){
		printk("Error al configurar el pin GPIO \n",ret);
		return;
	}
	while (1){
		gpio_pin_toggle(gpio_dev, LED_PIN);
		k_msleep(500);
	}
}
