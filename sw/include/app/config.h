#if !defined(APP_CONFIG_H_)
#define APP_CONFIG_H_

/* --------------- Features ------------------- */
#define FEATURE_SYSTEM_TIME_INIT 2000

#define FEATURE_COMM_ENABLE_DEBUG_SIGNALS 1

#define FEATURE_COMM_LIGHTREQUEST_TIMEOUT 4000

#define FEATURE_CONTROL_BUTTON_CYCLES 4

/* --------------- Settings ------------------- */
/* Brightness curve points */
#define CONFIG_BRIGHTNESS_CURVE_CUTOFF_X 1
#define CONFIG_BRIGHTNESS_CURVE_CUTOFF_Y 1
#define CONFIG_BRIGHTNESS_CURVE_MAX_X 1000
#define CONFIG_BRIGHTNESS_CURVE_MAX_Y 1000

/* Fixed brightness levels */
#define CONFIG_BRIGHTNESS_STANDARD_MIN_LEVEL 100
#define CONFIG_BRIGHTNESS_EMERGENCY_LEVEL 50
#define CONFIG_BRIGHTNESS_SAFETY_LEVEL 200

/* Strobe light settings */
// TODO: should emergency mode use different values? usually there the brightness is low
//       alternatively this could be a ratio instead of offset (e.g.: 1.0 -> CurrentTarget, 0.5 -> 50% of the target)
#define CONFIG_BRIGHTNESS_STROBE_LOW_OFFSET 700
#define CONFIG_BRIGHTNESS_STROBE_HIGH_OFFSET 0          // note: when disabled high offset is constantly applied, so it should be 0 for most cases

// Single strobe frequency: 30Hz
#define CONFIG_BRIGHTNESS_STROBE_SINGLE_ON_TIME 300
#define CONFIG_BRIGHTNESS_STROBE_SINGLE_OFF_TIME 300

#define CONFIG_STROBE_SOURCE_DISABLED 0
#define CONFIG_STROBE_SOURCE_INTERNAL_SINGLE 1
//TODO: external positive and negative

#define CONFIG_DEFAULT_STROBE_SOURCE CONFIG_STROBE_SOURCE_DISABLED
#define CONFIG_PRIMARY_STROBE_SOURCE CONFIG_STROBE_SOURCE_INTERNAL_SINGLE
#define CONFIG_SAFETY_STROBE_SOURCE CONFIG_STROBE_SOURCE_DISABLED
#define CONFIG_EMERGENCY_STROBE_SOURCE CONFIG_STROBE_SOURCE_DISABLED

#endif // APP_CONFIG_H_
