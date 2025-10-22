Feature: Bootloader

    # For the current version of the front light using the ATSAMC21 MCU there's no bootloader
    # available, hence boot entries are rejected
    Scenario: Bootloader entry
        Given base setup

        When I send boot request to 'FrontLight' on 'LineMaster'
        And I wait '500' milliseconds
        
        Then boot response of 'FrontLight' shall be 'NoBootloaderPresent'
