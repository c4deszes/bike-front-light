Controller
==========

ATSAMC21E18A-MNT is used as the microcontroller for the simple reasons of being in stock, having the
smallest footprint and software support available.

Pinout
------

.. list-table:: Pinout
    :header-rows: 1

    * - Pin
      - Pad
      - Peripheral
      - Function
      - Description

    * - PA02
      - 3
      - DAC
      - VOUT
      - LED Analog control

    * - PA05
      - 8
      - EIC
      - EXTINT[7]
      - Button

    * - PA07
      - 6
      - ADC
      - AIN[5]
      - Temperature sensor

    * - PA16
      - 17
      - TCC2
      - WO[0]
      - LED PWM control

    * - PA17
      - 18
      - TCC2
      - WO[1]
      - LED Frequency control

    * - PA19
      - 20
      - GPIO
      - GPIO[19]
      - LED Fault output

    * - PA22
      - 21
      - GPIO
      - GPIO[22]
      - LIN Transceiver Chip Select

    * - PA23
      - 22
      - GPIO
      - GPIO[23]
      - LIN Transceiver Transmit enable / Fault

    * - PA24
      - 23
      - SERCOM3
      - PAD2 (TX)
      - LIN Transceiver Transmit

    * - PA25
      - 24
      - SERCOM3
      - PAD3 (RX)
      - LIN Transceiver Receive

    * - PA30
      - 31
      - SWD
      - SWCLK
      - Serial-Wire-Debug clock

    * - PA31
      - 32
      - SWD
      - SWDIO
      - Serial-Wire-Debug data in/out
