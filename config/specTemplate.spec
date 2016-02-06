%define _package __package__
%define _packagename __packagename__
%define _version __version__
%define _release __release__
%define _prefix  __prefix__
%define _sources_dir __sources_dir__
%define _tmppath /tmp
%define _packagedir __packagedir__
%define _os __os__
%define _platform __platform__
%define _project __project__
%define _author __author__
%define _summary __summary__
%define _url __url__
#%define _buildarch __buildarch__
#%define _includedirs __includedirs__

#
# SWATCH Specfile template
#
Name: %{_packagename} 
Version: %{_version} 
Release: %{_release} 
Packager: %{_author}
Summary: %{_summary}
License: BSD License
Group: CACTUS
Source: %{_source}
URL: %{_url} 
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot 
Prefix: %{_prefix}

# TODO: include requirements
# References: 
# cmsos template file: https://svnweb.cern.ch/trac/cmsos/browser/trunk/config/spec.template
# cmsos RPM rules: https://svnweb.cern.ch/trac/cmsos/browser/trunk/config/mfRPM.rules
# Notice the _require and __require__ interplay with PACKAGE_REQUIRED_PACKAGE_LIST
# AMC13 example
#%define requires_uhal_version() cactuscore-uhal-uhal >= 2.3.0, cactuscore-uhal-uhal < 2.4.0
#BuildRequires: %requires_uhal_version
#Requires: %requires_uhal_version


%description
__description__

#
# Devel RPM specified attributes (extension to binary rpm with include files)
#
%package -n %{_packagename}-debuginfo
Summary:  Debuginfo package for %{_summary}
Group:    Applications/XDAQ

%description -n %{_packagename}-debuginfo
__description__


#%prep

#%build

%install 
# copy files to RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_prefix}/{bin,lib,include,etc}
mkdir -p $RPM_BUILD_ROOT/usr/lib/debug%{_prefix}/{bin,lib}
mkdir -p $RPM_BUILD_ROOT/usr/src/debug/%{_packagename}-%{_version}/

if [ -d %{_packagedir}/bin ]; then
  cd %{_packagedir}/bin; \
# find . -name "*" -exec install -D -m 755 {} $RPM_BUILD_ROOT/%{_prefix}/bin/{} \;
  find . -name "*" -exec $SWATCH_ROOT/config/install.sh {} %{_prefix}/bin/{} 755 $RPM_BUILD_ROOT %{_packagedir} %{_packagename} %{_version} %{_prefix}/include '%{_includedirs}' \;
fi

if [ -d %{_packagedir}/lib ]; then
  cd %{_packagedir}/lib; \
#  find . -name ".svn" -prune -o -name "*" -exec install -D -m 644 {} $RPM_BUILD_ROOT/%{_prefix}/lib/{} \;
  find . -name "*" -exec $SWATCH_ROOT/config/install.sh {} %{_prefix}/lib/{} 655 $RPM_BUILD_ROOT %{_packagedir} %{_packagename} %{_version} %{_prefix}/include '%{_includedirs}' \;
fi


if [ -d %{_packagedir}/include ]; then
  cd %{_packagedir}/include; \
  find . \( -name "*.hpp"  -o -name "*.hxx" \)  -exec install -D -m 644 {} $RPM_BUILD_ROOT/%{_prefix}/include/{} \;
fi


if [ -d %{_packagedir}/etc ]; then
  cd %{_packagedir}/etc; \
  find . -name ".svn" -prune -o -name "*" -exec install -D -m 644 {} $RPM_BUILD_ROOT/%{_prefix}/etc/{} \;
fi

#cp -rp %{_sources_dir}/* $RPM_BUILD_ROOT%{_prefix}/.


#create debug.source - SLC6 beardy wierdo "feature"
cd %{_packagedir}
#find include -name '*.hpp' -o -name '*.hxx' -fprintf rpm/debug.include "%p\0"
#find src -name '*.cpp' -o -name '*.cxx' -fprintf rpm/debug.source "%p\0"
#find src include -name '*.h' -print > rpm/debug.source -o -name '*.cc' -print > rpm/debug.source

# Copy all sources and include files for debug RPMs
cat %{_packagedir}/rpm/debug.source | sort -z -u | egrep -v -z '(<internal>|<built-in>)$' | ( cpio -pd0mL --quiet "$RPM_BUILD_ROOT/usr/src/debug/%{_packagename}-%{_version}" )
cat %{_packagedir}/rpm/debug.include | sort -z -u | egrep -v -z '(<internal>|<built-in>)$' | ( cpio -pd0mL --quiet "$RPM_BUILD_ROOT/usr/src/debug/%{_packagename}-%{_version}" )
# correct permissions on the created directories
cd "$RPM_BUILD_ROOT/usr/src/debug/"
find ./ -type d -exec chmod 755 {} \;

%clean

%post 

%postun 

%files 
%defattr(-, root, root, -) 
%{_prefix}/bin
%{_prefix}/lib
%{_prefix}/etc
%{_prefix}/include

#
# Files that go in the debuginfo RPM
#
%files -n %{_packagename}-debuginfo
%defattr(-,root,root,-)
/usr/lib/debug
/usr/src/debug