Feature: Uds Programming

    Scenario: Set standard level
        Given on setup

        When I write property 'Brightness_Standard_Level' of 'FrontLight' to '400' on 'UDS'
        And I wait '500' milliseconds
