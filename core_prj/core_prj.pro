TEMPLATE = subdirs

CONFIG += c++11
CONFIG += ordered

SUBDIRS += core \
    core_tst

tst.depends = core_src
