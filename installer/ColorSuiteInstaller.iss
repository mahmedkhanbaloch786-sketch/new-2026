; Inno Setup template — compile only after building and testing the OFX bundle.
#define ProductName "Beauty Skin Retouching - ColorSuite OFX"
#define ProductVersion "0.4.0"
#define BundlePath "..\dist\ColorSuite.ofx.bundle"

[Setup]
AppId={{B8A67B31-7F5C-4F62-9E9E-0F3B8C0A0300}
AppName={#ProductName}
AppVersion={#ProductVersion}
DefaultDirName={commoncf}\OFX\Plugins\ColorSuite.ofx.bundle
DisableProgramGroupPage=yes
OutputDir=.
OutputBaseFilename=ColorSuite_Setup_x64
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=admin
WizardStyle=modern
Compression=lzma2
SolidCompression=yes

[Files]
; This line expects the compiled plugin bundle. The installer must not package a fake binary.
Source: "{#BundlePath}\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs ignoreversion

[Code]
function InitializeSetup(): Boolean;
begin
  if not FileExists(ExpandConstant('{#BundlePath}\Contents\Info.plist')) then
  begin
    MsgBox('ColorSuite OFX bundle not found. Build the plugin first, then compile this installer.', mbError, MB_OK);
    Result := False;
  end
  else
    Result := True;
end;
