adcToTempTable = [
    401, 547, 748, 1020, 1374, 1807, 2295, 2785, 3221, 3560, 3793, 3938, 4019
] 

tempTable = [
    -40, -30, -20, -10, 0, 10, 20, 30, 40, 50,
    60, 70, 80]

def TMON_Calculate(adcValue):
    if adcValue <= adcToTempTable[0]:
        return tempTable[0]
    if adcValue >= adcToTempTable[12]:
        return tempTable[12]

    index = 0
    while adcValue > adcToTempTable[index + 1]:
        index += 1

    lowerAdcValue = adcToTempTable[index]
    upperAdcValue = adcToTempTable[index + 1]
    lowerTemp = tempTable[index]
    upperTemp = tempTable[index + 1]

    temperature = lowerTemp + ((upperTemp - lowerTemp) * (adcValue - lowerAdcValue)) / (upperAdcValue - lowerAdcValue)

    return temperature

print(TMON_Calculate(220 * 16))