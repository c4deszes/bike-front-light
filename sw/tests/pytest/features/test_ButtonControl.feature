# Expect cycle counter to be 0 initially

# Expect cycle counter to increase with each press

# Expect cycle counter to reset after cycle_max has been reached
Feature: Button control

    Scenario: Initial counter value
        Given off setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Off' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '0' on 'LineMaster'
        And I wait '100' milliseconds
        And I enable 'PSU' output
        And I wait '500' milliseconds
        
        # Then the signal 'FrontLightStatus.CycleCounter' should be '0'
