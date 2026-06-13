# Copyright 1999-2024 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=8

inherit desktop qmake-utils xdg

QTMIN=6.10.1

DESCRIPTION="The X2Go Qt client"
HOMEPAGE="https://wiki.x2go.org/doku.php"
SRC_URI="https://github.com/DonMartio/${PN}/archive/refs/tags/${PV}.tar.gz"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="amd64 ~x86"
IUSE="ldap"

DEPEND="
	>=dev-qt/qtbase-${QTMIN}:6=[dbus,gui,network]
	>=net-libs/libssh-0.7.5-r2
	net-print/cups
	x11-libs/libXpm
	ldap? ( net-nds/openldap:= )"
RDEPEND="${DEPEND}
	net-misc/nx"

CLIENT_BUILD="${WORKDIR}"/${P}.client_build
PLUGIN_BUILD="${WORKDIR}"/${P}.plugin_build

src_prepare() {
	default

	if ! use ldap; then
		sed -e "s/-lldap//" -i x2goclient.pro || die
		sed -e "s/#define USELDAP//" -i src/x2goclientconfig.h || die
	fi
}

src_configure() {
	qmake6 "${S}"/x2goclient.pro
}

src_install() {
	dobin ${PN}

	local size
	for size in 16 32 48 64 128 ; do
		doicon -s ${size} res/img/icons/${size}x${size}/${PN}.png
	done
	newicon -s scalable res/img/x2go-logos/x2go-logo.svg ${PN}.svg

	insinto /usr/share/pixmaps
	doins res/img/icons/${PN}.xpm

	domenu desktop/${PN}.desktop
	doman man/man?/*
}
