;
; Python-Ogre setup script
;
[Setup]
AppName=Python-Ogre
AppVerName=Python-Ogre 1.0RC1
DefaultDirName=c:\PythonOgre
DefaultGroupName=Python-Ogre
OutputBaseFilename=PythonOgreInstaller
OutputDir=c:\temp
SourceDir=c:\development\PythonOgreRelease
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
SolidCompression=yes


; Now for the files
;
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
;Source: packages_2.5\*; DestDir: {code:GetPythonSiteDir}; Check: ReturnFalse('2.5'); Flags: recursesubdirs
;Source: packages_2.4\*; DestDir: {code:GetPythonSiteDir}; Check: ReturnFalse('2.4'); Flags: recursesubdirs
; the python modules again - if we don't find python we install both versions in the app directory
;Source: packages_2.5\*; DestDir: {app}\packages_2.5; Check: NoPythonFound; Flags: recursesubdirs
;Source: packages_2.4\*; DestDir: {app}\packages_2.4; Check: NoPythonFound; Flags: recursesubdirs

;
; we need to remove the .pyc files when we uninstall
;
[UninstallDelete]
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\*.pyc

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
Name: {group}\OgreNewt Demos\Collision Callbacks; Filename: {code:GetPythonExe}; Parameters: Demo03_ColisionCallbacks.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Ray Casting; Filename: {code:GetPythonExe}; Parameters: Demo04_RayCasting.py; WorkingDir: {app}\Demos\OgreNewt

Name: {group}\Uninstall Python-Ogre; Filename: {uninstallexe}
Name: {group}\Python-Ogre API Documenatation; Filename: {app}\docs\Python-Ogre.chm
Name: {group}\Python-Ogre API (Html); Filename: {app}\docs\html\index.html



[Code]
var
  Page: TInputOptionWizardPage;
  NumPythonVersions : Integer;
  PythonVersions : array of String;
  PythonVersion : String;
  DisplayedPythonWarning : Boolean;

function NoPythonFound(): Boolean;
begin
	if NumPythonVersions = 0 then begin
		Result := True;
	end else begin
		Result := False;
	end;
end;

function ReturnFalse ( ver : String ) : Boolean;
begin
	Result := False;
end;

function FoundPython (  ver: String ) : Boolean;
var
	I : Integer;
begin
	Result := False;
	if NumPythonVersions = 0 then begin
		Result := False;
	end else begin
		for I:=0 to GetArrayLength(PythonVersions)-1 do
			begin
			if PythonVersions[I] = ver then Result := True;
			end
	end
end;

function SelectedPython (  ver: String ) : Boolean;
var
	I : Integer;
begin
	if PythonVersion = ver then begin
		Result := True;
	end else begin
		Result := False;
	end
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
	if NumPythonVersions > 0 then begin
		Result := GetPythonInstallPath ( PythonVersion ) + '\python.exe';
	end else begin
		Result := 'python.exe';
	end
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := False;
  if PageID = Page.ID then
  begin
	if NumPythonVersions = 1 then begin
		Result := True;
	end;
	if NumPythonVersions = 0 then begin
		if DisplayedPythonWarning = False then begin
			MsgBox('Unable to Find Python - will install all versions - "python setup.py install" will need to be run', mbInformation, MB_OK);
		end
		DisplayedPythonWarning := True
		Result := True;
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
	DisplayedPythonWarning := False;
	PythonVersion := '';
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
	if NumPythonVersions > 0 then begin
		for I:=0 to GetArrayLength(PythonVersions)-1 do begin
			Page.Add ('Python Version ' + PythonVersions[I] );
			Page.Values[I] := False;
		end;
		Page.Values[I-1] := True;
	end


end;
