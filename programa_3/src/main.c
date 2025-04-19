#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devide.h>
#include <zephyr/decetree.h>
#include <stdio.h>

#define LED_NODE DT_NODELABEL(gpio0)
#define BUTON_NODE DT_NODELABEL(gpio1)
#define LED_PIN 25
#define BUTTON_PIN 26


static const struct device *led_dev = DEVICE_DT_GET(LED_NODE);
static const struct device *button_dev = DEVICE_DT_GET(BUTTON_NODE);

void main(void){
	if (!device_is_ready(led_dev) || !device_is_ready(button_dev)){
		printf("Dispositivo no listo\n");
		return;
	}
	
	gpio_pin_configure(led_dev, LED_PIN, GPIO_OUTPUT_ACTIVATE);
	gpio_pin_configure(button_dev, BUTTON_PIN, GPIO_INPUT);

	while(1){
		int button_state = gpio_pin_get(button_dev, BUTTON_PIN);
		gpio_pin_set(led_dev, LED_PIN, button_state);
		k_msleep(100);
	}
}

