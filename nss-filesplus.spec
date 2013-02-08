Summary: glibc plugin to read additional local files
Name: nss-filesplus
Version: 0.51
Release: 1%{?dist}
License: LGPLv2+

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
changed using the /etc/filesplus/nss-filesplus.cfg config file.

This version does NOT read /etc/filesplus/nss-filesplus.cfg.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
mkdir -m755 -p ${RPM_BUILD_ROOT}/%{_lib}
install -m755 libnss_filesplus.so.2 ${RPM_BUILD_ROOT}/%{_lib}

mkdir -m755 -p ${RPM_BUILD_ROOT}/etc/filesplus
install -m644 nss-filesplus.cfg ${RPM_BUILD_ROOT}/etc/filesplus

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc README
/%{_lib}/*
%attr(0644,root,root) %config(noreplace) /etc/filesplus/nss-filesplus.cfg


