#if !defined(BSP_CONFIG_H_)
#define BSP_CONFIG_H_

#define CONTROL_MODE_PWM 0
#define CONTROL_MODE_ANALOG 1

#define LED_DRIVER_PWM_FREQUENCY 1000
#define LED_DRIVER_CONTROL_MODE CONTROL_MODE_PWM

#define LED_DRIVER_STARTUP_DELAY 500        // Time to wait before enabling the driver
#define LED_DRIVER_ENABLE_DELAY 100         // Time to wait before driving the LED

#define LED_DRIVER_VMON_WINDOW_LOW 500      
#define LED_DRIVER_VMON_WINDOW_HIGH 700

#endif // BSP_CONFIG_H_
