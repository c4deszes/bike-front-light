SECTIONS
{
    . = 0x0003D000;   /* Flash address where you want this structure */

    .config_data :
    {
        KEEP(*(.config_data))
    }
}
