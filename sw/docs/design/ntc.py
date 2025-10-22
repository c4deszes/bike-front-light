import numpy as np

# Constants
VCC = 3.3  # Supply voltage
R_SERIES = 10000  # Series resistor value in ohms
ADC_MAX = 4095  # 12-bit ADC maximum value
BETA = 4250  # Beta coefficient
R_NOMINAL = 10000  # Nominal resistance at 25°C
T_NOMINAL = 25 + 273.15  # Nominal temperature in Kelvin (25°C)

# Function to calculate thermistor resistance at a given temperature
def thermistor_resistance(temperature_celsius):
    temperature_kelvin = temperature_celsius + 273.15
    resistance = R_NOMINAL * np.exp(BETA * (1 / temperature_kelvin - 1 / T_NOMINAL))
    return resistance

# Function to calculate ADC value from thermistor resistance
def adc_value_from_resistance(resistance):
    v_thermistor = VCC * resistance / (R_SERIES + resistance)
    adc_value = int((v_thermistor / VCC) * ADC_MAX)
    return adc_value

# Generate lookup table
temperatures_celsius = np.linspace(-40, 80, num=13)  # 17 points from -40°C to 125°C
lookup_table = []

for temp in temperatures_celsius:
    resistance = thermistor_resistance(temp)
    adc_val = adc_value_from_resistance(resistance)
    lookup_table.append((int(temp), adc_val))

# Print lookup table
for temp, adc in lookup_table:
    print(f"Temperature: {temp}°C, ADC Value: {adc}")
