Feature: Software Number

    Scenario: Get Software number
        Given on setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Off' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '0' on 'LineMaster'

        And I request software version of 'FrontLight' on 'LineMaster'
        And I wait '1' seconds

        Then the software version of 'FrontLight' should be '0.1.0' on 'LineMaster'
