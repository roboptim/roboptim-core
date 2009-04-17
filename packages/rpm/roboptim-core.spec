Name: roboptim-core
Summary: Generic optimization library.
Vendor: FIXME
Version: 0.1
Release: 1
License: LGPL
URL: http://roboptim.sourceforge.net/
Group: Development/Tools
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}-%{release}-buildroot
Prefix: /usr

%description
The generic optimization library allows user to independently solve
numerical problems with one of the many available solvers.

%prep
%setup -q

%configure --docdir=%{_datadir}/doc/%{name}-%{version}
%build
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_datadir}/doc/%{name}-%{version}/*
%{_libdir}/*
%{_includedir}/*

%changelog
