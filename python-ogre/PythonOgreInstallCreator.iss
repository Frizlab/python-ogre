;
; Python-Ogre setup script
;
[Setup]
AppName=Python-Ogre
AppVerName=Python-Ogre 1.6RC1
DefaultDirName=C:\PythonOgre
DefaultGroupName=Python-Ogre
OutputBaseFilename=PythonOgreInstaller
OutputDir=C:\temp
SourceDir=C:\Development\PythonOgreRelease
VersionInfoDescription=Release 1.6RC1 of Python-Ogre
AllowNoIcons=true
AppPublisher=OpenSource (Andy and Team)
AppPublisherURL=http://www.python-ogre.org
AppSupportURL=http://www.python-ogre.org
AppUpdatesURL=http://www.python-ogre.org
AppVersion=1.6.0
LicenseFile=LICENSE.GPL
Compression=lzma
InfoBeforeFile=InstallWarning.rtf
InfoAfterFile=postinstall.rtf
SolidCompression=yes
AppCopyright=LPGL
VersionInfoCompany=OpenSource (Andy and Team)
VersionInfoTextVersion=1.6.0
VersionInfoCopyright=PythonOgre Development Team
RestartIfNeededByRun=false
UninstallDisplayName=PythonOgre
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
VersionInfoVersion=1.6.0
[Files]
; base files, demos and tools
Source: *; DestDir: {app}
Source: plugins\*; DestDir: {app}\plugins
Source: tools\*; DestDir: {app}\tools
Source: demos\*; DestDir: {app}\demos; Flags: recursesubdirs
Source: docs\*; DestDir: {app}\docs; Flags: recursesubdirs
; the python modules - in the event we find python
Source: packages_2.5\*; DestDir: {code:GetPythonSiteDir}; Flags: recursesubdirs; Check: SelectedPython('2.5')
; the python modules again - if we don't find python we install both versions in the app directory
;Source: packages_2.5\*; DestDir: {app}\packages_2.5; Flags: recursesubdirs; Check: NoPythonFound

; I'm going to install here anyway to make updates easier...
Source: packages_2.5\*; DestDir: {app}\packages_2.5; Flags: recursesubdirs

Source: ThirdParty\dxwebsetup.exe; DestDir: {app}\ThirdParty
Source: ThirdParty\vcredist_x86.exe; DestDir: {app}\ThirdParty
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
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\gui\QuickGUI\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\gui\BetaGUI\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\sound\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\sound\OgreAL\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OgreRefApp\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\ODE\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OgreOde\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OgreNewt\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\NxOgre\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\Physx\*.pyc
Type: filesandordirs; Name: {code:GetPythonSiteDir}\ogre\physics\OpCode\*.pyc

Type: files; Name: {app}\demos\CEGUI\*.pyc
Type: files; Name: {app}\demos\CEGUI\*.log
Type: files; Name: {app}\demos\CEGUI\*.cfg
Type: files; Name: {app}\demos\GUI\*.pyc
Type: files; Name: {app}\demos\GUI\*.log
Type: files; Name: {app}\demos\GUI\*.cfg
Type: files; Name: {app}\demos\ogre\*.pyc
Type: files; Name: {app}\demos\ogre\*.log
Type: files; Name: {app}\demos\ogre\*.cfg
Type: files; Name: {app}\demos\ogreode\*.pyc
Type: files; Name: {app}\demos\ogreode\*.log
Type: files; Name: {app}\demos\ogreode\*.cfg
Type: files; Name: {app}\demos\ogrenewt\*.pyc
Type: files; Name: {app}\demos\ogrenewt\*.log
Type: files; Name: {app}\demos\ogrenewt\*.cfg
Type: files; Name: {app}\demos\ogreal\*.pyc
Type: files; Name: {app}\demos\ogreal\*.log
Type: files; Name: {app}\demos\ogreal\*.cfg
Type: files; Name: {app}\demos\ode\*.pyc
Type: files; Name: {app}\demos\ode\*.log
Type: files; Name: {app}\demos\ode\*.cfg
Type: files; Name: {app}\demos\ffmpeg\*.pyc
Type: files; Name: {app}\demos\ffmpeg\*.log
Type: files; Name: {app}\demos\ffmpeg\*.cfg
Type: files; Name: {app}\demos\nxogre\*.pyc
Type: files; Name: {app}\demos\nxogre\*.log
Type: files; Name: {app}\demos\nxogre\*.cfg
Type: files; Name: {app}\demos\dshow\*.pyc
Type: files; Name: {app}\demos\dshow\*.log
Type: files; Name: {app}\demos\dshow\*.cfg
Type: dirifempty; Name: {app}\demos\CEGUI
Type: dirifempty; Name: {app}\demos\GUI
Type: dirifempty; Name: {app}\demos\ogre
Type: dirifempty; Name: {app}\demos\ogreode
Type: dirifempty; Name: {app}\demos\ogrenewt
Type: dirifempty; Name: {app}\demos\ogreal
Type: dirifempty; Name: {app}\demos\ode
Type: dirifempty; Name: {app}\demos\nxogre
Type: dirifempty; Name: {app}\demos\ffmepg
Type: dirifempty; Name: {app}\demos\dshow
Type: dirifempty; Name: {app}\demos
Type: dirifempty; Name: {app}

;
; I REALLY DON'T like doing this but if people have used a previous version of Python-Ogre
; I need to remove it to force any existing source code to fail and hence have them
; update to the new directory structure..
; being very conservative in this!!!!!!!!!!!!!
;
[InstallDelete]
Type: files; Name: {code:GetPythonSiteDir}\Ogre\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OIS\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\GUI\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreOde\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreAL\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\ODE\*.pyd
Type: files; Name: {code:GetPythonSiteDir}\FMOD\*.pyd

Type: files; Name: {code:GetPythonSiteDir}\Ogre\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OIS\*.dll
Type: files; Name: {code:GetPythonSiteDir}\GUI\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreOde\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreAL\*.dll
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp\*.dll
Type: files; Name: {code:GetPythonSiteDir}\ODE\*.dll
Type: files; Name: {code:GetPythonSiteDir}\FMOD\*.dll

Type: files; Name: {code:GetPythonSiteDir}\Ogre\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OIS\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\GUI\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreOde\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreAL\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\ODE\_*.p*
Type: files; Name: {code:GetPythonSiteDir}\FMOD\_*.p*

Type: files; Name: {code:GetPythonSiteDir}\Ogre\sf_*.*

Type: files; Name: {code:GetPythonSiteDir}\Ogre
Type: files; Name: {code:GetPythonSiteDir}\OIS
Type: files; Name: {code:GetPythonSiteDir}\GUI
Type: files; Name: {code:GetPythonSiteDir}\OgreOde
Type: files; Name: {code:GetPythonSiteDir}\OgreNewt
Type: files; Name: {code:GetPythonSiteDir}\OgreAL
Type: files; Name: {code:GetPythonSiteDir}\OgreRefApp
Type: files; Name: {code:GetPythonSiteDir}\ODE
Type: files; Name: {code:GetPythonSiteDir}\FMOD

Type: dirifempty; Name: {code:GetPythonSiteDir}\Ogre
Type: dirifempty; Name: {code:GetPythonSiteDir}\OIS
Type: dirifempty; Name: {code:GetPythonSiteDir}\GUI
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
Name: {group}\Ogre Demos\Compositor; Filename: {code:GetPythonExe}; Parameters: Demo_Compositor.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\ENV Map; Filename: {code:GetPythonExe}; Parameters: Demo_EnvMap.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Facial; Filename: {code:GetPythonExe}; Parameters: Demo_Facial.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Fish; Filename: {code:GetPythonExe}; Parameters: Demo_OneFish.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Fresnel; Filename: {code:GetPythonExe}; Parameters: Demo_Fresnel.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Grass; Filename: {code:GetPythonExe}; Parameters: Demo_Grass.py; WorkingDir: {app}\Demos\Ogre
Name: {group}\Ogre Demos\Lighting; Filename: {code:GetPythonExe}; Parameters: Demo_Lighting.py; WorkingDir: {app}\Demos\Ogre
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

Name: {group}\GUI\CEGUI - Basic; Filename: {code:GetPythonExe}; Parameters: Demo_CEGUI_Gui.py; WorkingDir: {app}\Demos\cegui
Name: {group}\GUI\CEGUI Drag and Drop; Filename: {code:GetPythonExe}; Parameters: Demo_CEGUI_DragnDrop.py; WorkingDir: {app}\Demos\cegui
Name: {group}\GUI\CEGUI - Nice; Filename: {code:GetPythonExe}; Parameters: Demo_CEGUI_NewGui.py; WorkingDir: {app}\Demos\cegui
Name: {group}\GUI\QuickGUI-01; Filename: {code:GetPythonExe}; Parameters: Demo_QuickGUI01.py; WorkingDir: {app}\Demos\qgui
Name: {group}\GUI\Navi; Filename: {code:GetPythonExe}; Parameters: Demo_Navi.py; WorkingDir: {app}\Demos\navi

Name: {group}\ODE Demos\Multiple Scenes; Filename: {code:GetPythonExe}; Parameters: Demo_Scenes.py; WorkingDir: {app}\Demos\OgreOde
Name: {group}\ODE Demos\Gran Turism; Filename: {code:GetPythonExe}; Parameters: GranTurismOgre.py; WorkingDir: {app}\Demos\OgreOde

Name: {group}\OgreAL Demos\Basic Sound; Filename: {code:GetPythonExe}; Parameters: Demo_RenderToTexture.py; WorkingDir: {app}\Demos\OgreAL

Name: {group}\OgreNewt Demos\Basics; Filename: {code:GetPythonExe}; Parameters: Demo01_TheBasics.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Joints; Filename: {code:GetPythonExe}; Parameters: Demo02_Joints.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Collision Callbacks; Filename: {code:GetPythonExe}; Parameters: Demo03_CollisionCallbacks.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Ray Casting; Filename: {code:GetPythonExe}; Parameters: Demo04_RayCasting.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Simple Vehicle; Filename: {code:GetPythonExe}; Parameters: Demo05_SimpleVehicle.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Simple Buoyancy; Filename: {code:GetPythonExe}; Parameters: Demo06_SimpleBuoyancy.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Custom Joints; Filename: {code:GetPythonExe}; Parameters: Demo07_CustomJoints.py; WorkingDir: {app}\Demos\OgreNewt
Name: {group}\OgreNewt Demos\Ragdoll; Filename: {code:GetPythonExe}; Parameters: Demo08_ragdoll.py; WorkingDir: {app}\Demos\OgreNewt

;Name: {group}\Video Demos\ffmpeg Video; Filename: {code:GetPythonExe}; Parameters: Demo_Video.py; WorkingDir: {app}\Demos\ffmpeg
;Name: {group}\Video Demos\DirectShow Video; Filename: {code:GetPythonExe}; Parameters: Demo_Video.py; WorkingDir: {app}\Demos\dshow
Name: {group}\Video Demos\Theora Video; Filename: {code:GetPythonExe}; Parameters: Demo_Video.py; WorkingDir: {app}\Demos\theora

;Name: {group}\NxOgre\Download AGEIA drivers first; Filename: http://www.ageia.com/drivers/drivers.html
;Name: {group}\NxOgre\Cakewalk; Filename: {code:GetPythonExe}; Parameters: Cakeframework.py; WorkingDir: {app}\Demos\nxogre
;Name: {group}\NxOgre\Demo101; Filename: {code:GetPythonExe}; Parameters: Demo_101.py; WorkingDir: {app}\Demos\nxogre
;Name: {group}\NxOgre\Demo102; Filename: {code:GetPythonExe}; Parameters: Demo_102.py; WorkingDir: {app}\Demos\nxogre
;Name: {group}\NxOgre\Demo103; Filename: {code:GetPythonExe}; Parameters: Demo_103.py; WorkingDir: {app}\Demos\nxogre

Name: {group}\Plib Demos\Networking - Server; Filename: {code:GetPythonExe}; Parameters: Demo_Server.py; WorkingDir: {app}\Demos\plib
Name: {group}\Plib Demos\Networking - Client; Filename: {code:GetPythonExe}; Parameters: Demo_Client.py; WorkingDir: {app}\Demos\plib
;Name: {group}\Plib Demos\Sound; Filename: {code:GetPythonExe}; Parameters: Demo_Sound.py; WorkingDir: {app}\Demos\plib

Name: {group}\Special Effects\Forests; Filename: {code:GetPythonExe}; Parameters: Demo_Forest.py; WorkingDir: {app}\Demos\ogreforests
Name: {group}\Special Effects\Editable Terrain; Filename: {code:GetPythonExe}; Parameters: Demo_CEGUI_ET.py; WorkingDir: {app}\Demos\et
Name: {group}\Special Effects\Caelum Sky; Filename: {code:GetPythonExe}; Parameters: Demo_Caelum01.py; WorkingDir: {app}\Demos\caelum
Name: {group}\Special Effects\Noise Generation; Filename: {code:GetPythonExe}; Parameters: Demo_Noise_01.py; WorkingDir: {app}\Demos\noise
Name: {group}\Special Effects\Particle Universe; Filename: {code:GetPythonExe}; Parameters: Demo_Particle.py; WorkingDir: {app}\Demos\particleuniverse

Name: {group}\Uninstall Python-Ogre; Filename: {uninstallexe}
Name: {group}\Python-Ogre API Documenatation; Filename: {app}\docs\Python-Ogre.chm
;Name: {group}\Python-Ogre API (Html); Filename: {app}\docs\html\index.html

[Run]
Filename: {app}\ThirdParty\vcredist_x86.exe
Filename: {app}\ThirdParty\dxwebsetup.exe; Description: DirectX Installer; StatusMsg: You probably NEED to update your Directx Drivers; Flags: postinstall

[Code]
var
  Page: TInputOptionWizardPage;
  NumPythonVersions : Integer;
  PythonVersions : array of String;
  PythonLocations : array of String;
  PythonVersion : String;
  DisplayedPythonWarning : Boolean;
  SupportedVersions : array of String;
  Debug : Boolean;

function NoPythonFound(): Boolean;
begin
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


function FoundPythonIn (  ver: String; listin: array of String ) : Boolean;
var
	I : Integer;
begin
	Result := False;
	if GetArrayLength(listin) = 0 then
		Result := False
	else
		for I:=0 to GetArrayLength(listin)-1 do
			if listin[I] = ver then Result := True;
end;

function SelectedPython (  ver: String ) : Boolean;
var
	I : Integer;
begin
	if PythonVersion = ver then
		Result := True
	else
		Result := False;
end;

function IsSupportedVersion ( Param: String ) : Boolean;
var
	I: Integer;
begin
	Result := False;
	;msgbox ('Checking Supported Versions ' + Param + ' ' + IntToStr(GetArrayLength(SupportedVersions) ), mbInformation, MB_OK);

	for I:=0 to GetArrayLength(SupportedVersions)-1 do begin
		if Param = SupportedVersions[i] then begin
		    Result := True;
    		;msgbox ( 'Is a supported version ' + Param + ' (' + SupportedVersions[i] + ')  ' + Param , mbInformation, MB_OK);
	  end else begin
	      ;msgbox ( 'Not a supported version ' + Param +  ' (' +SupportedVersions[i] + ')  ', mbInformation, MB_OK);
        end;	        
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
	Result1, Result2 : array of String;
	tempv, templ :  array of String;
begin
	tempv := ['',''];
	templ := ['',''];
	count := 0;
	RegGetSubkeyNames(HKEY_LOCAL_MACHINE, 'SOFTWARE\Python\PythonCore', Result1);
	;msgbox ( 'LOCAL MACHINE  ' + InttoStr(GetArrayLength(Result1)) , mbInformation, MB_OK);
	// if we have versions here process them
	for i:=0 to GetArrayLength(Result1)-1 do
		if IsSupportedVersion ( Result1[i] ) then begin // make sure we support it..
			if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Python\PythonCore\' + Result1[i] + '\InstallPath', '', p) then begin
				if FileExists ( p + '\python.exe' ) then begin
					tempv[count] := Result1[i];
					templ[count] := p;	// save the location
					count := count + 1;
				end;
			end;
		end;


	if count < 2 then begin // we've only found zero, or one version so far..
		// OK so we should now have all the valid versions listed in LOCAL_MACHINE
		RegGetSubkeyNames(HKEY_CURRENT_USER, 'SOFTWARE\Python\PythonCore', Result2);
		;msgbox ( 'CURRENT_USER  ' + InttoStr(GetArrayLength(Result2)) , mbInformation, MB_OK);
		for i:=0 to GetArrayLength(Result2)-1 do
			if not FoundPythonIn ( Result2[i], tempv ) then  // check we don't already know about this version
				if IsSupportedVersion (Result2[i]) then begin
					if RegQueryStringValue(HKEY_CURRENT_USER, 'SOFTWARE\Python\PythonCore\' + Result2[i] + '\InstallPath', '', p) then begin
						if FileExists ( p + '\python.exe' ) then begin
							tempv[count] := Result2[i];
							templ[count] := p;
							count := count + 1;
						end;
					end;
				end;

	end;
	for i:=0 to 1 do
		if ( Length(tempv[i]) > 0 ) then begin
			setarraylength ( PythonVersions, getarraylength (PythonVersions) + 1);
			PythonVersions[getarraylength(PythonVersions)-1] := tempv[i];
			setarraylength ( PythonLocations, getarraylength (Pythonlocations) + 1);
			PythonLocations[getarraylength(PythonLocations)-1] := templ[i];
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
	if PageID = Page.ID then begin
		if NumPythonVersions = 1 then
			Result := True;
		if NumPythonVersions = 0 then begin
			if DisplayedPythonWarning = False then
				MsgBox('Unable to Find Python - will install all versions - "python setup.py install" will need to be run', mbInformation, MB_OK);
			DisplayedPythonWarning := True;
			Result := True;
		end;
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
	if NoPythonFound then begin
		Result := Result + 'Python was not found so the python modules will' + NewLine +
		Space + 'be installed under ' + ExpandConstant('{app}') +'.' + NewLine +
		Space + 'You will need to run "python setup.py install" to install the modules' +
		NewLine + NewLine +
		Space + 'The program links will probably be broken as Python was not found';
	end else begin
		Result := Result + 'Python Modules will be installed to:' + NewLine +
		Space + GetPythonSiteDir ( PythonVersion ) + NewLine;
	end;
end;
{ we have an issue that sometimes the older python ogre is not uninstalled or completely
deleted by out predelete file lists -- probably because a file has been added etc
This is OK except it leave the module directory with the wrong name so we need to fix this
}
procedure CurStepChanged(CurStep: TSetupStep);
begin
	if CurStep = ssInstall then begin
		if DirExists ( GetPythonSiteDir('') + '\Ogre' ) then begin
			RenameFile ( GetPythonSiteDir('') + '\Ogre', GetPythonSiteDir('') + '\ogre' );
		end;
	end;
end;

procedure InitializeWizard();
var
	I: Integer;
begin

	DisplayedPythonWarning := False;
	PythonVersion := '';
	SupportedVersions := [ '2.5'];

	Debug := False;

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
