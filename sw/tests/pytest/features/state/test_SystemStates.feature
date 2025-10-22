Feature: System States

    # Normal mode into Safety
    # Safety into Normal

    # Emergency Safety ?

    Scenario: POR into Off
        Given off setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Off' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '0' on 'LineMaster'
        And I wait '100' milliseconds
        And I enable 'PSU' output

        Then the brightness should be '0'

    Scenario: POR into Adaptive
        Given off setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Adaptive' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '60' on 'LineMaster'
        And I wait '100' milliseconds
        And I enable 'PSU' output

        Then the brightness should be '60'

    Scenario: POR into Standard
        Given off setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Standard' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '60' on 'LineMaster'
        And I wait '100' milliseconds
        And I enable 'PSU' output

        Then the brightness should be '60'

    Scenario: POR into Emergency
        Given off setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Emergency' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '0' on 'LineMaster'
        And I wait '100' milliseconds
        And I enable 'PSU' output

        Then the brightness should be '30'

    Scenario: POR into Safety
        Given off setup

        When I enable 'PSU' output
        And I wait '5' seconds

        Then the brightness should be 'SafetyBrightness'

    Scenario: Normal into Safety
        Given on setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Standard' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '10' on 'LineMaster'

        Then the brightness should be '20'

        When I disable schedule on 'LineMaster'
        And I wait '5' seconds

        Then the brightness should be 'SafetyBrightness'