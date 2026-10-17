target_sources_ifdef(
    CONFIG_OUR_DRIVER
    app
    PRIVATE
    our_driver.c
)

target_sources_ifdef(
    CONFIG_OUR_DRIVER_SHELL
    app
    PRIVATE
    our_driver_shell.c
)