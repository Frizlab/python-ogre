;
; Python-Ogre setup script
;
[Setup]
AppName=Python-Ogre
AppVerName=Python-Ogre 1.4.0
DefaultDirName=c:\PythonOgre
DefaultGroupName=Python-Ogre
SolidCompression=true
OutputBaseFilename=PythonOgreInstaller
OutputDir=c:\temp
SourceDir=c:\development\PythonOgreRelease
VersionInfoDescription=First Installer Release of Python-Ogre
VersionInfoVersion=1.4.0
AllowNoIcons=true
AppPublisher=OpenSource
AppPublisherURL=http://www.python-ogre.org
AppSupportURL=http://www.python-ogre.org
AppUpdatesURL=http://www.python-ogre.org
AppVersion=1.4.0
LicenseFile=LICENSE.GPL


; Now for the files
;
[Files]
; base files, demos and tools
source: *; DestDir: {app}
source: plugins\*; DestDir: {app}\plugins
source: tools\*; DestDir: {app}\tools
source: demos\*; DestDir: {app}\demos; Flags: recursesubdirs

; the python modules - in the event we find python
Source: packages_2.5\*; DestDir: {code:GetPythonSiteDir}; Check: FoundPython('2.5'); Flags: recursesubdirs
Source: packages_2.4\*; DestDir: {code:GetPythonSiteDir}; Check: FoundPython('2.4'); Flags: recursesubdirs
; the python modules again - if we don't find python we install both versions in the app directory
Source: packages_2.5\*; DestDir: {app}\packages_2.5; Check: NoPythonFound; Flags: recursesubdirs
Source: packages_2.4\*; DestDir: {app}\packages_2.4; Check: NoPythonFound; Flags: recursesubdirs

[Icons]
Name: {group}\Ogre Demos\Smoke; Filename: {code:GetPythonExe}; Parameters: Demo_Smoke.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\CEGUI Demos\GUI; Filename: {code:GetPythonExe}; Parameters: Demo_Gui.py; WorkingDir: {app}\Demos\CEGUI
Name: {group}\ODE Demos\Scenes; Filename: {code:GetPythonExe}; Parameters: Demo_Scenes.py; WorkingDir: {app}\Demos\OgreOde
Name: {group}\Newton Demos\Basics; Filename: {code:GetPythonExe}; Parameters: Demo01_TheBasics.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\Uninstall Python-Ogre; Filename: {uninstallexe}


[Code]
var
  Page: TInputOptionWizardPage;
  NumPythonVersions : Integer;
  PythonVersions : array of String;
  PythonVersion : String;

function NoPythonFound(): Boolean;
begin
	if NumPythonVersions = 0 then begin
		Result := True;
	end else begin
		Result := False;
	end;
end;

function FoundPython (  ver: String ) : Boolean;
var
	I : Integer;
begin
	Result := False;
	for I:=0 to GetArrayLength(PythonVersions)-1 do
		begin
		if PythonVersions[I] = ver then Result := True;
		end;
end;



function GetInstalledPythonVersions() : array of String;
begin
	RegGetSubkeyNames(HKEY_LOCAL_MACHINE, 'SOFTWARE\Python\PythonCore', Result);
end;

function GetPythonInstallPath(Ver: String) : String;
begin
	RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Python\PythonCore\' + Ver + '\InstallPath', '', Result);
end;

function GetPythonSiteDir(Param: String) : String;
begin
	Result := GetPythonInstallPath ( PythonVersion ) + '\Lib\site-packages';
end;

function GetPythonExe ( Param: String) : String;
begin
	Result := GetPythonInstallPath ( PythonVersion ) + '\python.exe';
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := False;
  if PageID = Page.ID then
  begin
	if NumPythonVersions = 1 then begin
		Result := True;
		MsgBox('Unable to Find Python', mbError, MB_OK);
		MsgBox(PythonVersion, mbInformation, MB_OK);
	end;
	if NumPythonVersions = 0 then begin
		MsgBox('Unable to Find Python', mbError, MB_OK);
	end;

  end;
end;
function NextButtonClick(PageID: Integer): Boolean;
begin
	Result := True;

	if PageID = Page.ID then begin
		PythonVersion := PythonVersions[Page.SelectedValueIndex]
	end;
end;

procedure InitializeWizard();
var
	I: Integer;
begin
	PythonVersions := GetInstalledPythonVersions();
	NumPythonVersions := GetArrayLength(PythonVersions);
	if NumPythonVersions = 1 then
		begin
			PythonVersion := PythonVersions[0]
		end;

	Page := CreateInputOptionPage(wpLicense,
	  'Select Python Version', 'Multiple Version of Python Found - Select one',
	  'Please select the version of Python you would like to have Python-Ogre installed for..',
	  True, False);
	for I:=0 to GetArrayLength(PythonVersions)-1 do begin
		Page.Add ('Python Version ' + PythonVersions[I] );
		Page.Values[I] := False;
	end;
	Page.Values[I-1] := True;

end;
