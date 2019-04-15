TEMPLATE = subdirs

CONFIG += c++11
CONFIG += ordered

SUBDIRS += \
    core_prj \
    gui \
#    main \
    mockup \
    profiling

DISTFILES += \
    .astylerc
