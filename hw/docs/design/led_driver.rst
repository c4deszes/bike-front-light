LED driver design
=================

.. jupyter-execute::
    :hide-code:

    %config InlineBackend.figure_format = 'svg'
    import schemdraw
    from schemdraw import elements as elm
    from schemdraw import flow
    import numpy as np
    from matplotlib import pyplot
    from IPython.display import Latex
    from UliEngineering.Electronics.Resistors import power_dissipated_in_resistor_by_current, normalize_numeric

Challenges
----------

* Minimizing PCB footprint while being efficient and having adequate thermals
* Inclusion of different protection and diagnostic functions

Driver choice
-------------

Based on previous experience with Infineon LITIX devices the decision is to use something from it's
Power series ICs.

.. list-table::
    :header-rows: 1

    * - Device
      - Package
      - Topology
      - Max. current
      - Short circuit prot.
      - Short circuit det.
      - Open circuit det.
      - PWM
      - Analog control

    * - TLD5045EJ
      - SOIC-8 (w/ thermal pad)
      - Buck
      - 700mA (100mA minimum current)
      - Yes
      - No
      - Yes
      - Yes
      - No

    * - ILD8150
      - SOIC-8 (w/ thermal pad)
      - Buck
      - 1.5A
      - No
      - No
      - No
      - Yes
      - Yes

    * - TLD5097EP
      - PG-TSDSO-14
      - Boost, Buck, SEPIC, Flyback
      - N/A
      - Yes
      - No?
      - Yes
      - Yes
      - Yes

    * - TLD5098EP
      - PG-TSDSO-14
      - Boost, Buck, SEPIC, Flyback
      - N/A
      - Yes
      - Yes (non-distinct)
      - Yes (non-distinct)
      - Yes
      - Yes

    * - TLD5099EP
      - PG-TSDSO-14
      - Boost, Buck, SEPIC, Flyback (no buck example)
      - N/A
      - Yes
      - Yes (non-distinct)
      - Yes (non-distinct)
      - Yes
      - Yes

    * - TLD6098-1EP
      - PG-TSDSO-14
      - Boost, Buck, SEPIC, Flyback (no buck example)
      - N/A
      - Yes
      - Yes (non-distinct)
      - Yes (non-distinct)
      - Yes
      - Yes

Based on this information it's hard to decide, as a preliminary TLD6098-1EP is out of the question
as it's really meant for 48V applications.

TLD509x devices are pin compatible. TLD5099 only brings additional PWM logic compared to TLD5098
and the TLD5097 seems to be missing short circuit detection.

The decision is to design for TLD509x in a buck topology and also do another design with ILD8150.
The efficiency and thermals can then be evaluated which will be the basis for Gen2.

LEDs
----

The original part is a 3.45mm x 3.45mm white LED with a glass or plastic lens. The measured current
at maximum battery charge is around 700mA with a forward voltage of 2.9V.

Based on this information it's likely one of the following parts:

SST-12-W50S-A120-H4502: 5000K, 285lm
SST-12-W57S-A120-H4572: 5700K, 285lm
SST-12-W65S-A120-H5652: 6500K, 305lm

.. jupyter-execute::
    :hide-code:

    vf_led = '2.9V'
    i_led = '700mA'

TLD509x design calculations
---------------------------

LED current
~~~~~~~~~~~

Firstly the resistance of the feedback resistor sets the LED current.

.. jupyter-execute::
    :hide-code:

    tld509x_vref = '300mV'
    tld509x_rsense = normalize_numeric(tld509x_vref) / normalize_numeric(i_led)

    Latex("\\begin{gathered}"
          f"R_{{sense}} = \\frac{{V_{{ref}}}}{{I_{{out}}}} = {tld509x_rsense*1000:.01f}mΩ\\\\"
          "\end{gathered}")

ILD8150 design calculations
---------------------------

LED current
~~~~~~~~~~~

The LED current is set the same way as in the TLD509x design but the reference voltage is different,
to achieve the same target current.

.. jupyter-execute::
    :hide-code:

    ild8150_vref = '360mV'
    ild8150_rsense = normalize_numeric(ild8150_vref) / normalize_numeric(i_led)
    ild8150_psense = normalize_numeric(i_led) * normalize_numeric(i_led) * ild8150_rsense

    Latex("\\begin{gathered}"
          f"R_{{sense}} = \\frac{{V_{{ref}}}}{{I_{{out}}}} = {ild8150_rsense*1000:.01f}mΩ\\\\"
          f"P_{{sense}} = {ild8150_psense*1000:.01f}mW\\\\"
          "\end{gathered}")

Inductor value
~~~~~~~~~~~~~~

.. jupyter-execute::
    :hide-code:

    v_in = ['9V']
    t_delay = '400ns'

    f_sw = '300kHz'
    v_csh = '390mV'
    v_csl = '330mV'

    lowest_inductor = 100       # 100H

    for x in v_in:
      l_term1 = normalize_numeric(ild8150_rsense) * (normalize_numeric(vf_led) - normalize_numeric(x) * normalize_numeric(t_delay) * normalize_numeric(f_sw)) / \
                (normalize_numeric(f_sw) * (normalize_numeric(v_csh) - normalize_numeric(v_csl)))
      l_term2 = normalize_numeric(ild8150_rsense) * (normalize_numeric(vf_led) * normalize_numeric(vf_led)) / \
                (normalize_numeric(x) * normalize_numeric(f_sw) * (normalize_numeric(v_csh) - normalize_numeric(v_csl)))

      l_ind = l_term1 - l_term2
      if l_ind < lowest_inductor:
        lowest_inductor = l_ind

    Latex("\\begin{gathered}"
          f"L_{{min}} = {lowest_inductor*1000000:.02f}uH\\\\"
          "\end{gathered}")
