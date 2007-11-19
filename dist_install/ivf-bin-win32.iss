[Dirs]
Name: {app}\bin
Name: {app}\lib
[Files]
Source: ..\dist\bin\*.*; DestDir: {app}\bin; Flags: recursesubdirs createallsubdirs
Source: ..\dist\include\*.*; DestDir: {app}\include; Flags: recursesubdirs createallsubdirs
Source: ..\dist\lib\*.*; DestDir: {app}\lib; Flags: recursesubdirs createallsubdirs
Source: ..\dist\utils\*.*; DestDir: {app}\utils; Flags: recursesubdirs createallsubdirs
Source: ..\dist\doc\*.*; DestDir: {app}\doc; Flags: recursesubdirs createallsubdirs
Source: ..\dist\examples\*.*; DestDir: {app}\examples; Flags: recursesubdirs createallsubdirs
Source: ..\dist\*.*; DestDir: {app}
[Setup]
AppCopyright=Copyright © 2006 Division of Structural Mechanics
AppName=Interactive Visualisation Framework
AppVerName=Interactive Visualisation Framework {#IvfVersion}
;AppVerName=Interactive Visualisation Framework 1.0.0
DefaultDirName={pf}\Structural Mechanics\ivf
DefaultGroupName=Structural Mechanics
UsePreviousAppDir=no
UsePreviousGroup=no
;Compression=lzma
;SolidCompression=yes
DisableDirPage=no
WizardImageFile=compiler:WIZMODERNIMAGE-IS.BMP
LicenseFile=..\COPYING
[Icons]
Name: {group}\Ivf++ User guide; Filename: {app}\doc\ivfguide.pdf; WorkingDir: {app}\doc
Name: {group}\Ivf++ Reference (help); Filename: {app}\doc\html\ivf.chm; WorkingDir: {app}\doc\html
Name: {group}\Ivf++ Reference (html); Filename: {app}\doc\html\index.html; WorkingDir: {app}\doc\html
Name: {group}\Examples README; Filename: {app}\README.EXAMPLES.txt; WorkingDir: {app}
Name: {group}\Application generator; Filename: {app}\utils\ivfappgen.cmd; Parameters: gui; WorkingDir: {app}\utils
Name: {group}\Class generator; Filename: {app}\utils\ivfclassgen.cmd; Parameters: gui; WorkingDir: {app}\utils
