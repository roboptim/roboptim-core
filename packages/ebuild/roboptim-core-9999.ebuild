# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit git

EGIT_REPO_URI="git://FIXME"

DESCRIPTION="Generic optimization library."
HOMEPAGE="http://FIXME/"
SRC_URI=""

LICENSE="LGPL-3"
SLOT="0"
KEYWORDS="~x86"

DEPEND=""
RDEPEND="${DEPEND}"

src_compile() {
        test -x "./bootstrap" && ./bootstrap || die "bootstrap failed"
        econf $(use_with ice) || die "econf failed"
	emake FLAGS="${CFLAGS}" || die "compile failed"
}

src_install() {
        emake DESTDIR="${D}" install || die "make install failed"
}
