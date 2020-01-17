TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Efi_libs/dsp.c \
        Efi_libs/file.c \
        Efi_libs/std.c \
        Efi_libs/str.c \
        Efi_libs/utils.c \
        main.c

HEADERS += \
    Efi_libs/Headers/dsp.h \
    Efi_libs/Headers/file.h \
    Efi_libs/Headers/std.h \
    Efi_libs/Headers/str.h \
    Efi_libs/Headers/utils.h

DISTFILES += \
    real_rec.txt
