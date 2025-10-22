Feature: Operation status

    Scenario: Normal operation
        Given on setup

        When I enable schedule 'FrontLightSchedule' on 'LineMaster'
        And I set master signal 'LightSynchronization.LightMode' to 'Off' on 'LineMaster'
        And I set master signal 'LightSynchronization.TargetBrightness' to '0' on 'LineMaster'

        And I request operation status of 'FrontLight' on 'LineMaster'
        And I wait '1' seconds

        Then the operation status of 'FrontLight' should be 'Ok' on 'LineMaster'

    # TODO: test short/open circuit op status
