Feature: Strobe

    Scenario: Default strobe
        Given on setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Standard' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '50' on 'LineMaster'
        And I set master signal 'FrontLightSetting.Behavior' to 'Default' on 'LineMaster'

        And I wait '5' seconds
        # TODO: somehow assert that default blink setting is used

    Scenario: Solid
        Given on setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Standard' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '50' on 'LineMaster'
        And I set master signal 'FrontLightSetting.Behavior' to 'Solid' on 'LineMaster'

        And I wait '5' seconds
        # TODO: somehow assert that not blinking

    Scenario: Blink
        Given on setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Standard' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '50' on 'LineMaster'
        And I set master signal 'FrontLightSetting.Behavior' to 'Blink' on 'LineMaster'

        And I wait '5' seconds
        # TODO: somehow assert that blinking
