SUMMARY = "A custom image for a cluster unit."

require recipes-sato/images/core-image-sato.bb

IMAGE_INSTALL:append= " vsomeip python3 wayland cinematicexperience liberation-fonts \
                        qtbase-tools qtbase qtdeclarative qtimageformats qtmultimedia qtquickcontrols2 qtquickcontrols \
                        qtbase-plugins qtwayland qtgraphicaleffects cmake make \
                        vsomeip-compile boost mpv python3-spidev "

IMAGE_INSTALL:remove= "splash psplash"

IMAGE_LINGUAS = " "

PACKAGECONFIG_FONTS:append_pn-qtbase = " fontconfig"

LICENSE = "CLOSED"

inherit core-image

IMAGE_ROOTFS_SIZE ?= "4088200"
