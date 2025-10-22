# Expect room temperature sensing
Feature: Temperature monitoring

    Scenario: Temperature probing
        Given on setup

        When I enable schedule 'FrontLight' on 'LineMaster'
        And I wait '500' milliseconds

        Then I expect signal 'FrontLightDebug.DriveTemperature' to be within '15' and '25'
