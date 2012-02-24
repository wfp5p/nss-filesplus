Summary: glibc plugin to read additional local files
Name: nss-filesplus
Version: 0.2
Release: 1%{?dist}
License: WTFPL

Group: System Environment/Libraries
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires: /sbin/ldconfig

%define _filter_GLIBC_PRIVATE 1

%description
nss-filesplus is a plugin for the GNU Name Service Switch (NSS)
functionality of the GNU C Library (glibc) allowing group, passwd,
and shadow entries to come from another file than the standard
ones in /etc.

The default location of these files is /etc/filesplus.  This can be
changed using the /etc/nss-filesplus.cfg config file.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
install -m755 -d ${RPM_BUILD_ROOT}/%{_lib}
install -m755 libnss_filesplus.so.2 ${RPM_BUILD_ROOT}/%{_lib}

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc nss-filesplus.cfg README
/%{_lib}/*

