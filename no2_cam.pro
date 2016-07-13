CONFIG += c++11

TEMPLATE = subdirs

SUBDIRS += \
    core \
    gui \
    main \
    mock_up \
    tests

main.depends = core gui
mock_up.depends = core gui
tests.depends = core
