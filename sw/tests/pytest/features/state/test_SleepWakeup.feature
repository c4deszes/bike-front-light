# Expect wakeup on POR

# Expect sleep when requesting shutdown

# Expect off when requesting idle??

Feature: Sleep Wakeup

    Scenario: Wakeup (POR)
        Given off setup

        When I turn on the PSU
        And I wait '500' milliseconds
        And I request 'FrontLightStatus' on 'LineMaster'

        #Then the response to 'SpeedStatus' should be valid

    Scenario: Idle-Wakeup
        Given on setup

        When I send idle request on 'LineMaster'

        #Then the PSU current shall be less than '1mA'

    Scenario: Shutdown-Wakeup
        Given on setup

        When I send shutdown request on 'LineMaster'
        And I wait '500' milliseconds

        # Then the PSU current shall be less than '500uA'

        When I send wakeup request on 'LineMaster'
        And I wait '500' milliseconds
        And I request 'FrontLightStatus' on 'LineMaster'
        
        #Then the PSU current shall be more than '10mA'
        #And the response to 'FrontLightStatus' should be valid
