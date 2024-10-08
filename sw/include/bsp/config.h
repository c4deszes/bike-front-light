#if !defined(BSP_CONFIG_H_)
#define BSP_CONFIG_H_

#define CONTROL_MODE_PWM 0u
#define CONTROL_MODE_ANALOG 1u

#define LED_DRIVER_PWM_FREQUENCY 1000u
#define LED_DRIVER_CONTROL_MODE CONTROL_MODE_PWM

#define LED_DRIVER_STARTUP_DELAY 500u        // Time to wait before enabling the driver
#define LED_DRIVER_ENABLE_DELAY 100u         // Time to wait before driving the LED

#define LED_DRIVER_VMON_RATIO 0.175          // 47k / (47k + 10k)
#define LED_DRIVER_VMON_VREF 3.3
#define LED_DRIVER_VMON_SAMPLE_COUNT 5
#define LED_DRIVER_VMON_WINDOW_LOW ((uint16_t)(1.6*LED_DRIVER_VMON_RATIO/LED_DRIVER_VMON_VREF*1024))      // 1.6V * 0.175 / 3.3 * 1024
#define LED_DRIVER_VMON_WINDOW_HIGH ((uint16_t)(3.6*LED_DRIVER_VMON_RATIO/LED_DRIVER_VMON_VREF*1024))     // 3.6V * 0.175 / 3.3 * 1024

#endif // BSP_CONFIG_H_
