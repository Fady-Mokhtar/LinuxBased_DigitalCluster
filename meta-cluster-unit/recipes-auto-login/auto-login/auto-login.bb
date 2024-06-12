SUMMARY = "A service to autologin the user"
DESCRIPTION = "A service to autologin the user for autostart for the graduation project."

LICENSE="CLOSED"
LIC_FILES_CHKSUM=""

inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE:${PN} = "auto_login_service.service"

SRC_URI += "file://auto_login_service.service"

do_install() {
    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/auto_login_service.service ${D}${systemd_unitdir}/system/
}
