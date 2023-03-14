Name: ClamAV-GUI
Summary: Front-End for ClamAV
Version: 0.2.3
Release: 1
License: GPL-3.0
Group: Productivity/Security
BuildRoot: %{_tmppath}/build-root-%{name}
Source0: ClamAV-GUI-0.2.3.tgz
Distribution: OpenSuSE
Vendor: Wuselsoft
Url: http://kde-apps.org/content/show.php/ClamAV-GUI?content=170782
BuildRequires: libqt4-devel pkgconfig(QtCore) update-desktop-files

Requires: libQtCore.so.4()(64bit) libQtGui.so.4()(64bit) libc.so.6()(64bit) libc.so.6(GLIBC_2.2.5)(64bit) libgcc_s.so.1()(64bit) libgcc_s.so.1(GCC_3.0)(64bit) libm.so.6()(64bit) libpthread.so.0()(64bit) libstdc++.so.6()(64bit) libstdc++.so.6(CXXABI_1.3)(64bit) libstdc++.so.6(GLIBCXX_3.4)(64bit) libQtNetwork.so.4()(64bit)

%description
A graphical Front-End for the ClamAV Anti-Virus Scanner

%prep
%setup -q

%build
qmake
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" \
make

%install
make install INSTALL_ROOT=%{buildroot}
%post
%desktop_database_post

%postun
%desktop_database_postun

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
/usr/bin/clamav-gui
%{_datadir}/applications/clamav-gui
%{_datadir}/applications/clamav-gui.desktop
%{_datadir}/applications/clamav-gui/clamav-gui-de_DE.qm
%{_datadir}/applications/clamav-gui/clamav-gui-en_GB.qm
%{_datadir}/applications/clamav-gui/clamav-gui.png
%{_datadir}/icons/hicolor
%{_datadir}/icons/hicolor*
%{_datadir}/icons/hicolor/16x16/apps/clamav-gui.png
%{_datadir}/icons/hicolor/32x32/apps/clamav-gui.png
%{_datadir}/icons/hicolor/128x128/apps/clamav-gui.png
%{_datadir}/icons/hicolor/22x22/apps/clamav-gui.png
%{_datadir}/icons/hicolor/256x256/apps/clamav-gui.png
%{_datadir}/icons/hicolor/48x48/apps/clamav-gui.png
%{_datadir}/icons/hicolor/64x64/apps/clamav-gui.png
%{_mandir}/de
%{_mandir}/en_GB
%{_mandir}/de/man1
%{_mandir}/en_GB/man1
%{_mandir}/de/man1/clamav-gui.1.gz
%{_mandir}/en_GB/man1/clamav-gui.1.gz

%changelog
* Tue Aug 11 2015 Version 0.2.3
- Clean up of the Makefile. No install.sh script needed - just do make; make install to install the application

* Thu Aug 6 2015 Version 0.2.3
- Added some visual effects to show that the app is busy (Update, Scanner ...)

* Sun Aug 2 2015 Version 0.2.1. Package rebuild with Easy RPM Builder
- Added a search function for the Log-Files (search/continue/jump to infected files/jump to errors)
- Added a Service Menu for Konqueror / Dolphin

* Wed Jul 29 2015 Version 0.1.2. Package rebuild with Easy RPM Builder
- fixed a bug at startup to prefent the application being startet multiple times
- fixed some issues regarding logging of freshclam

* Tue Jul 28 2015 Version 0.1.0. Package rebuild with Easy RPM Builder
- initial package build
