;
; Python-Ogre setup script
;
[Setup]
AppName=Python-Ogre
AppVerName=Python-Ogre 1.0RC1
DefaultDirName=C:\PythonOgre
DefaultGroupName=Python-Ogre
OutputBaseFilename=PythonOgreInstaller
OutputDir=C:\temp
SourceDir=C:\development\PythonOgreRelease
VersionInfoDescription=First Installer Release of Python-Ogre
VersionInfoVersion=1.0.0
AllowNoIcons=true
AppPublisher=OpenSource
AppPublisherURL=http://www.python-ogre.org
AppSupportURL=http://www.python-ogre.org
AppUpdatesURL=http://www.python-ogre.org
AppVersion=1.0.0
LicenseFile=LICENSE.GPL
Compression=lzma
InfoBeforeFile=InstallWarning.rtf
InfoAfterFile=postinstall.rtf
SolidCompression=true


; Now for the files
;
AppCopyright=LPGL
VersionInfoCompany=OpenSource
VersionInfoTextVersion=1.0.0
VersionInfoCopyright=PythonOgre Development Team
RestartIfNeededByRun=false
UninstallDisplayName=PythonOgre
[Files]
; base files, demos and tools
source: *; DestDir: {app}
source: plugins\*; DestDir: {app}\plugins
source: tools\*; DestDir: {app}\tools
source: demos\*; DestDir: {app}\demos; Flags: recursesubdirs
source: docs\*; DestDir: {app}\docs; Flags: recursesubdirs


; the python modules - in the event we find python
Source: packages_2.5\*; DestDir: {code:GetPythonSiteDir}; Check: SelectedPython('2.5'); Flags: recursesubdirs
Source: packages_2.4\*; DestDir: {code:GetPythonSiteDir}; Check: SelectedPython('2.4'); Flags: recursesubdirs
; the python modules again - if we don't find python we install both versions in the app directory
Source: packages_2.5\*; DestDir: {app}\packages_2.5; Check: NoPythonFound; Flags: recursesubdirs
Source: packages_2.4\*; DestDir: {app}\packages_2.4; Check: NoPythonFound; Flags: recursesubdirs
;
; we need to remove the .pyc files when we uninstall
;
[UninstallDelete]
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\renderer\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\renderer\OGRE\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\io\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\io\OIS\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\gui\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\gui\CEGUI\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\sound\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\sound\OgreAL\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OgreRefApp\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\ODE\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OgreOde\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OgreNewt\*.pyc

;
; I REALLY DON'T like doing this but if people have used a previous version of Python-Ogre
; I need to remove it to force any existing source code to fail and hence have them
; update to the new directory structure..
; being very conservative in this!!!!!!!!!!!!!
;
[InstallDelete]
Type: files; Name: {code:GetPythonSiteDir}\Ogre\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OIS\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\CEGUI\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreOde\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreAL\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\ODE\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\FMOD\*.pyd

Type: files; Name: {code:GetPythonSiteDir}\Ogre\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OIS\*.dll
Type: files; Name: {code:GetPythonSiteDir}\CEGUI\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreOde\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreAL\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp\*.dll
Type: files; Name: {code:GetPythonSiteDir}\ODE\*.dll
Type: files; Name: {code:GetPythonSiteDir}\FMOD\*.dll

Type: files; Name: {code:GetPythonSiteDir}\Ogre\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OIS\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\CEGUI\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreOde\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreAL\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\ODE\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\FMOD\_*.p*

Type: files; Name: {code:GetPythonSiteDir}\Ogre\sf_*.*

Type: files; Name: {code:GetPythonSiteDir}\Ogre
Type: files; Name: {code:GetPythonSiteDir}\OIS
Type: files; Name: {code:GetPythonSiteDir}\CEGUI
Type: files; Name: {code:GetPythonSiteDir}\OgreOde
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt
Type: files; Name: {code:GetPythonSiteDir}\OgreAL
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp
Type: files; Name: {code:GetPythonSiteDir}\ODE
Type: files; Name: {code:GetPythonSiteDir}\FMOD

Type: dirifempty; Name: {code:GetPythonSiteDir}\Ogre
Type: dirifempty; Name: {code:GetPythonSiteDir}\OIS
Type: dirifempty; Name: {code:GetPythonSiteDir}\CEGUI
Type: dirifempty; Name: {code:GetPythonSiteDir}\OgreOde
Type: dirifempty; Name: {code:GetPythonSiteDir}\OgreNewt
Type: dirifempty; Name: {code:GetPythonSiteDir}\OgreAL
Type: dirifempty; Name: {code:GetPythonSiteDir}\OgreRefApp
Type: dirifempty; Name: {code:GetPythonSiteDir}\ODE

[Icons]
Name: {group}\Ogre Demos\Bezier; Filename: {code:GetPythonExe}; Parameters: Demo_Bezier.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\BSP; Filename: {code:GetPythonExe}; Parameters: Demo_BSP.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\BSP_Collision; Filename: {code:GetPythonExe}; Parameters: Demo_BspCollision.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Camera Tracking; Filename: {code:GetPythonExe}; Parameters: Demo_CameraTracking.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\CEL Shading; Filename: {code:GetPythonExe}; Parameters: Demo_CelShading.py; WorkingDir: {app}\Demos\Ogre
;Name: {group}\Ogre Demos\Compositor; Filename: {code:GetPythonExe}; Parameters: Demo_Compositor.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\ENV Map; Filename: {code:GetPythonExe}; Parameters: Demo_EnvMap.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Facial; Filename: {code:GetPythonExe}; Parameters: Demo_Facial.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Fresnel; Filename: {code:GetPythonExe}; Parameters: Demo_Fresnel.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Grass; Filename: {code:GetPythonExe}; Parameters: Demo_Grass.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Lightinge; Filename: {code:GetPythonExe}; Parameters: Demo_Lighting.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Particles; Filename: {code:GetPythonExe}; Parameters: Demo_ParticleFX.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Render to Texture; Filename: {code:GetPythonExe}; Parameters: Demo_RenderToTexture.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Shadows; Filename: {code:GetPythonExe}; Parameters: Demo_Shadows_Org.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Animation 1; Filename: {code:GetPythonExe}; Parameters: Demo_SkeletalAnimation.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Animation 2; Filename: {code:GetPythonExe}; Parameters: Demo_SkeletonAnimation.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Sky Box; Filename: {code:GetPythonExe}; Parameters: Demo_SkyBox.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Sky Dome; Filename: {code:GetPythonExe}; Parameters: Demo_SkyDome.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Sky Plane; Filename: {code:GetPythonExe}; Parameters: Demo_SkyPlane.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Smoke; Filename: {code:GetPythonExe}; Parameters: Demo_Smoke.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Terrain; Filename: {code:GetPythonExe}; Parameters: Demo_Terrain.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Textures; Filename: {code:GetPythonExe}; Parameters: Demo_TextureFX.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Transparency; Filename: {code:GetPythonExe}; Parameters: Demo_Transparency.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\WX Gui; Filename: {code:GetPythonExe}; Parameters: Demo_WX.py; WorkingDir: {app}\Demos\Ogre

Name: {group}\CEGUI Demos\GUI; Filename: {code:GetPythonExe}; Parameters: Demo_Gui.py; WorkingDir: {app}\Demos\CEGUI
Name: {group}\CEGUI Demos\Drag and Drop; Filename: {code:GetPythonExe}; Parameters: Demo_DragnDrop.py; WorkingDir: {app}\Demos\CEGUI
Name: {group}\CEGUI Demos\Gui 2; Filename: {code:GetPythonExe}; Parameters: Demo_NewGui.py; WorkingDir: {app}\Demos\CEGUI
Name: {group}\CEGUI Demos\Fish; Filename: {code:GetPythonExe}; Parameters: Demo_FishNoWx.py; WorkingDir: {app}\Demos\CEGUI
Name: {group}\ODE Demos\Multiple Scenes; Filename: {code:GetPythonExe}; Parameters: Demo_Scenes.py; WorkingDir: {app}\Demos\OgreOde
Name: {group}\OgreAL Demos\Basic Sound; Filename: {code:GetPythonExe}; Parameters: Demo_RenderToTexture.py; WorkingDir: {app}\Demos\OgreAL

Name: {group}\OgreNewt Demos\Basics; Filename: {code:GetPythonExe}; Parameters: Demo01_TheBasics.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Joints; Filename: {code:GetPythonExe}; Parameters: Demo02_Joints.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Collision Callbacks; Filename: {code:GetPythonExe}; Parameters: Demo03_CollisionCallbacks.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Ray Casting; Filename: {code:GetPythonExe}; Parameters: Demo04_RayCasting.py; WorkingDir: {app}\Demos\OgreNewt

Name: {group}\Uninstall Python-Ogre; Filename: {uninstallexe}
Name: {group}\Python-Ogre API Documenatation; Filename: {app}\docs\Python-Ogre.chm
Name: {group}\Python-Ogre API (Html); Filename: {app}\docs\html\index.html



[Code]
var
  Page: TInputOptionWizardPage;
  NumPythonVersions : Integer;
  PythonVersions : array of String;
  PythonLocations : array of String;
  PythonVersion : String;
  DisplayedPythonWarning : Boolean;
  SupportedVersions : array of String;
  Force, Debug : Boolean;

function NoPythonFound(): Boolean;
begin
	if Force then
		Result := True 	// force the modules installed under app dir
	else
		if NumPythonVersions = 0 then
			Result := True
		else
			Result := False;
end;


function FoundPython (  ver: String ) : Boolean;
var
	I : Integer;
begin
	Result := False;
	if NumPythonVersions = 0 then
		Result := False
	else
		for I:=0 to GetArrayLength(PythonVersions)-1 do
			if PythonVersions[I] = ver then Result := True;
end;

function SelectedPython (  ver: String ) : Boolean;
var
	I : Integer;
begin
	if Force then // with force we install both modules under app directory
		Result := False
	else
		if PythonVersion = ver then
			Result := True
		else
			Result := False;
end;

function IsSupportedVersion ( Param: String ) : Boolean;
var
	I: Integer;
begin
	Result := True;
//	msgbox ('Checking Supported Versions ' + Param + IntToStr(GetArrayLength(SupportedVersions) ), mbInformation, MB_OK);

	for I:=0 to GetArrayLength(SupportedVersions)-1 do begin
		if Param = SupportedVersions[i] then Result := True;
//		msgbox ( 'Checked ' + SupportedVersions[i], mbInformation, MB_OK);
	end;
end;


{
Get the python versions -- we have to check two registry "areas" as python allows
the install to be "for everyone", or "just for me" which is a pain..  Further more
sometimes when you uninstall python it doesn't remove the registry entries so I
need to check the python executable exists as well
}
function GetInstalledPythonVersions() : Integer;

var
	i, count: Integer;
	p: string;
	Result1 : array of String;
begin
	PythonVersions := ['',''];
	PythonLocations := ['',''];
	count := 0;
	RegGetSubkeyNames(HKEY_LOCAL_MACHINE, 'SOFTWARE\Python\PythonCore', Result1);
	// if we have versions here process them
	for i:=0 to GetArrayLength(Result1)-1 do
		if IsSupportedVersion ( Result1[i] ) then begin // make sure we support it..
			RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Python\PythonCore\' + Result1[i] + '\InstallPath', '', p);
			if FileExists ( p + '\python.exe' ) then begin
				PythonVersions[count] := Result1[i];
				PythonLocations[count] := p;	// save the location
				count := count + 1;
			end;
		end;


	if count < 2 then begin // we've only found zero, or one version so far..
		// OK so we should now have all the valid versions listed in LOCAL_MACHINE
		RegGetSubkeyNames(HKEY_CURRENT_USER, 'SOFTWARE\Python\PythonCore', Result1);
		for i:=0 to GetArrayLength(Result1)-1 do
			if not FoundPython ( Result1[i] ) then  // check we don't already know about this version
				if IsSupportedVersion (Result1[i]) then begin
					RegQueryStringValue(HKEY_CURRENT_USER, 'SOFTWARE\Python\PythonCore\' + Result1[i] + '\InstallPath', '', p);
					if FileExists ( p + '\python.exe' ) then begin
						PythonVersions[count] := Result1[i];
						PythonLocations[count] := p;
						count := count + 1;
					end;
				end;

	end;
	Result := GetArrayLength ( PythonVersions );
end;

function GetPythonInstallPath(Ver: String) : String;
var
	I : Integer;
begin
	Result := ''
	for i:=0 to NumPythonVersions -1 do begin
		if Ver = PythonVersions[i] then Result:= PythonLocations[i];
	end
end;

function GetPythonSiteDir(Param: String) : String;
begin
	Result := GetPythonInstallPath ( PythonVersion ) + 'Lib\site-packages';
end;

function GetPythonExe ( Param: String) : String;
begin
	if NumPythonVersions > 0 then
		Result := GetPythonInstallPath ( PythonVersion ) + '\python.exe'
	else
		Result := 'python.exe';
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := False;
  if PageID = Page.ID then
	if NumPythonVersions = 1 then
		Result := True;
	if NumPythonVersions = 0 then begin
		if DisplayedPythonWarning = False then
			MsgBox('Unable to Find Python - will install all versions - "python setup.py install" will need to be run', mbInformation, MB_OK);
		DisplayedPythonWarning := True;
		Result := True;
	end;
end;


function NextButtonClick(PageID: Integer): Boolean;
begin
	Result := True;
	if PageID = Page.ID then
		PythonVersion := PythonVersions[Page.SelectedValueIndex]
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo, MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
begin
	Result:= 'Python-Ogre is about to be installed....' + NewLine + NewLine +
	'Tools, demos, and documentation will be installed to:' + NewLine +
	Space + ExpandConstant('{app}') + NewLine + NewLine;
	if NoPythonFound or Force then begin
		Result := Result + 'Python was not found so the python modules will' + NewLine +
		Space + 'be installed under ' + ExpandConstant('{app}') +'.' + NewLine +
		Space + 'You will need to run "python setup.py install" to install the modules';
	end else begin
		Result := Result + 'Python Modules will be installed to:' + NewLine +
		Space + GetPythonSiteDir ( PythonVersion ) + NewLine;
	end;
end;

procedure InitializeWizard();
var
	I: Integer;
begin

	DisplayedPythonWarning := False;
	PythonVersion := '';
	SupportedVersions := ['2.4', '2.5'];
	Debug := False;
	Force := False;
	if ParamCount > 0 then
		for I:=0 to ParamCount-1 do begin
			if ParamStr( I ) = 'debug' then begin
				Debug := True;
				msgbox ('Setting Debug to True',  mbInformation, MB_OK);
			end;
			if ParamStr( I ) = 'force' then begin
				Force := True;
				msgbox ('Setting Force to True',  mbInformation, MB_OK);
			end;
		end;

	NumPythonVersions := GetInstalledPythonVersions();
	if Debug then
		msgbox ('Number Of Python Versions is ' + InttoStr( NumPythonVersions),  mbInformation, MB_OK);
	if NumPythonVersions = 1 then
		PythonVersion := PythonVersions[0];
		if Debug then
			msgbox ('Using python version ' + PythonVersion,  mbInformation, MB_OK);

	Page := CreateInputOptionPage(wpLicense,
	  'Select Python Version', 'Multiple Version of Python Found - Select one',
	  'Please select the version of Python you would like to have Python-Ogre installed for..',
	  True, False);
	if NumPythonVersions > 0 then begin
		for I:=0 to GetArrayLength(PythonVersions)-1 do begin
			Page.Add ('Python Version ' + PythonVersions[I] );
			Page.Values[I] := False;
		end;
		Page.Values[I-1] := True;
	end


end;
