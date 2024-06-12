SUMMARY = "the implementation of SOME/IP"

# The Swiss Army Knife of embedded Linux. it slices, it dices, it makes Julian Fries, You know you want it ;).

LICENSE = "CLOSED"

DEPENDS = "boost"

SRCREV = "55284c518f805a2e0194e23770f7c0f2b352ed64"
SRC_URI = "git://github.com/COVESA/vsomeip.git;protocol=https;branch=master"
    
S = "${WORKDIR}/git"

inherit cmake  

FILES:${PN} += "${bindir}/vsomeipd ${sysconfdir}/${BPN}"
FILES:${PN}-dev += "${libdir}/cmake"


do_install:append() {
    mv ${D}/usr/etc ${D}
}
