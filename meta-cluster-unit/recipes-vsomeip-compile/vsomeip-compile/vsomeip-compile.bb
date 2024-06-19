SUMMARY = "vsomeip-compile"
DESCRIPTION = "Recipe to compile a code with vsomeip library and install it on the target"

LICENSE = "CLOSED"

SRC_URI = "file://main.cpp"

S = "${WORKDIR}"

DEPENDS = "vsomeip boost"

do_compile() {
    ${CXX} ${CXXFLAGS} ${LDFLAGS} -o main_vsomeip main.cpp -lvsomeip3 -lboost_system -lboost_log -lboost_thread
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 main_vsomeip ${D}${bindir}
}