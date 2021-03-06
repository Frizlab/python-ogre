; Script generated by the ISS2NSI program.
; Generated from: C:\Development\pywork\PythonOgreInstallCreator.iss

!cd "C:\Development\PythonOgreRelease\"

; MUI 1.66 compatible
!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_SPECIALBITMAP "compiler:WizModernImage-IS.bmp"
!insertmacro MUI_PAGE_WELCOME
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "LICENSE.GPL"
!define MUI_PAGE_HEADER_TEXT "Information"
!define MUI_PAGE_HEADER_SUBTEXT "Please read the following important information before continuing."
!define MUI_LICENSEPAGE_TEXT_TOP "When you are ready to continue with Setup, click Next."
!define MUI_LICENSEPAGE_TEXT "' ' '&Next'"
!insertmacro MUI_PAGE_LICENSE "InstallWarning.rtf"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Python-Ogre"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\%sPython-Ogre"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "NSIS:StartMenuDir"
!insertmacro MUI_PAGE_STARTMENU
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_PAGE_HEADER_TEXT "Information"
!define MUI_PAGE_HEADER_SUBTEXT "Please read the following important information before continuing."
!define MUI_LICENSEPAGE_TEXT_TOP "When you are ready to continue with Setup, click Next."
!define MUI_LICENSEPAGE_TEXT "' ' '&Next'"
!insertmacro MUI_PAGE_LICENSE "postinstall.rtf"
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Name "Python-Ogre 1.0.2.3"
BrandingText "LPGL"
OutFile "C:\temp\PythonOgreInstaller.exe"
InstallDir "C:\PythonOgre"
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  MessageBox MB_YESNO|MB_ICONQUESTION "This will install $(^Name). Do you wish to continue?" IDYES +2
  Abort
FunctionEnd

Section -InstallDelete
  Delete "{code:GetPythonSiteDir}\Ogre\*.pyd"
  Delete "{code:GetPythonSiteDir}\OIS\*.pyd"
  Delete "{code:GetPythonSiteDir}\GUI\*.pyd"
  Delete "{code:GetPythonSiteDir}\OgreOde\*.pyd"
  Delete "{code:GetPythonSiteDir}\OgreNewt\*.pyd"
  Delete "{code:GetPythonSiteDir}\OgreAL\*.pyd"
  Delete "{code:GetPythonSiteDir}\OgreRefApp\*.pyd"
  Delete "{code:GetPythonSiteDir}\ODE\*.pyd"
  Delete "{code:GetPythonSiteDir}\FMOD\*.pyd"
  Delete "{code:GetPythonSiteDir}\Ogre\*.dll"
  Delete "{code:GetPythonSiteDir}\OIS\*.dll"
  Delete "{code:GetPythonSiteDir}\GUI\*.dll"
  Delete "{code:GetPythonSiteDir}\OgreOde\*.dll"
  Delete "{code:GetPythonSiteDir}\OgreNewt\*.dll"
  Delete "{code:GetPythonSiteDir}\OgreAL\*.dll"
  Delete "{code:GetPythonSiteDir}\OgreRefApp\*.dll"
  Delete "{code:GetPythonSiteDir}\ODE\*.dll"
  Delete "{code:GetPythonSiteDir}\FMOD\*.dll"
  Delete "{code:GetPythonSiteDir}\Ogre\_*.p*"
  Delete "{code:GetPythonSiteDir}\OIS\_*.p*"
  Delete "{code:GetPythonSiteDir}\GUI\_*.p*"
  Delete "{code:GetPythonSiteDir}\OgreOde\_*.p*"
  Delete "{code:GetPythonSiteDir}\OgreNewt\_*.p*"
  Delete "{code:GetPythonSiteDir}\OgreAL\_*.p*"
  Delete "{code:GetPythonSiteDir}\OgreRefApp\_*.p*"
  Delete "{code:GetPythonSiteDir}\ODE\_*.p*"
  Delete "{code:GetPythonSiteDir}\FMOD\_*.p*"
  Delete "{code:GetPythonSiteDir}\Ogre\sf_*.*"
  Delete "{code:GetPythonSiteDir}\Ogre"
  Delete "{code:GetPythonSiteDir}\OIS"
  Delete "{code:GetPythonSiteDir}\GUI"
  Delete "{code:GetPythonSiteDir}\OgreOde"
  Delete "{code:GetPythonSiteDir}\OgreNewt"
  Delete "{code:GetPythonSiteDir}\OgreAL"
  Delete "{code:GetPythonSiteDir}\OgreRefApp"
  Delete "{code:GetPythonSiteDir}\ODE"
  Delete "{code:GetPythonSiteDir}\FMOD"
  RMDir "{code:GetPythonSiteDir}\Ogre"
  RMDir "{code:GetPythonSiteDir}\OIS"
  RMDir "{code:GetPythonSiteDir}\GUI"
  RMDir "{code:GetPythonSiteDir}\OgreOde"
  RMDir "{code:GetPythonSiteDir}\OgreNewt"
  RMDir "{code:GetPythonSiteDir}\OgreAL"
  RMDir "{code:GetPythonSiteDir}\OgreRefApp"
  RMDir "{code:GetPythonSiteDir}\ODE"
SectionEnd

Section -Files
  SetOutPath "$INSTDIR"
  File "*"
  SetOutPath "$INSTDIR\plugins"
  File "plugins\*"
  SetOutPath "$INSTDIR\tools"
  File "tools\*"
  SetOutPath "$INSTDIR\demos"
  File "demos\*"
  SetOutPath "$INSTDIR\docs"
  File "docs\*"
  SetOutPath "{code:GetPythonSiteDir}"
  File "packages_2.5\*"
  File "packages_2.4\*"
  SetOutPath "$INSTDIR\packages_2.5"
  File "packages_2.5\*"
  SetOutPath "$INSTDIR\packages_2.4"
  File "packages_2.4\*"
  SetOutPath "$INSTDIR\ThirdParty"
  File "ThirdParty\dxwebsetup.exe"
  File "ThirdParty\vcredist_x86.exe"
SectionEnd

Section -Icons
!insertmacro MUI_STARTMENU_WRITE_BEGIN
  CreateDirectory "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos"
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Bezier.lnk" "{code:GetPythonExe}" "Demo_Bezier.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\BSP.lnk" "{code:GetPythonExe}" "Demo_BSP.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\BSP_Collision.lnk" "{code:GetPythonExe}" "Demo_BspCollision.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Camera Tracking.lnk" "{code:GetPythonExe}" "Demo_CameraTracking.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\CEL Shading.lnk" "{code:GetPythonExe}" "Demo_CelShading.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Compositor.lnk" "{code:GetPythonExe}" "Demo_Compositor.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\ENV Map.lnk" "{code:GetPythonExe}" "Demo_EnvMap.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Facial.lnk" "{code:GetPythonExe}" "Demo_Facial.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Fish.lnk" "{code:GetPythonExe}" "Demo_OneFish.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Fresnel.lnk" "{code:GetPythonExe}" "Demo_Fresnel.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Grass.lnk" "{code:GetPythonExe}" "Demo_Grass.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Lighting.lnk" "{code:GetPythonExe}" "Demo_Lighting.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Particles.lnk" "{code:GetPythonExe}" "Demo_ParticleFX.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Render to Texture.lnk" "{code:GetPythonExe}" "Demo_RenderToTexture.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Shadows.lnk" "{code:GetPythonExe}" "Demo_Shadows_Org.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Animation 1.lnk" "{code:GetPythonExe}" "Demo_SkeletalAnimation.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Animation 2.lnk" "{code:GetPythonExe}" "Demo_SkeletonAnimation.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Sky Box.lnk" "{code:GetPythonExe}" "Demo_SkyBox.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Sky Dome.lnk" "{code:GetPythonExe}" "Demo_SkyDome.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Sky Plane.lnk" "{code:GetPythonExe}" "Demo_SkyPlane.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Smoke.lnk" "{code:GetPythonExe}" "Demo_Smoke.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Terrain.lnk" "{code:GetPythonExe}" "Demo_Terrain.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Textures.lnk" "{code:GetPythonExe}" "Demo_TextureFX.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Transparency.lnk" "{code:GetPythonExe}" "Demo_Transparency.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\WX Gui.lnk" "{code:GetPythonExe}" "Demo_WX.py" "" "" "" "" ""
  CreateDirectory "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI"
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\CEGUI - Basic.lnk" "{code:GetPythonExe}" "Demo_CEGUI_Gui.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\CEGUI Drag and Drop.lnk" "{code:GetPythonExe}" "Demo_CEGUI_DragnDrop.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\CEGUI - Nice.lnk" "{code:GetPythonExe}" "Demo_CEGUI_NewGui.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\QuickGUI-01.lnk" "{code:GetPythonExe}" "Demo_QuickGUI01.py" "" "" "" "" ""
  CreateDirectory "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\ODE Demos"
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\ODE Demos\Multiple Scenes.lnk" "{code:GetPythonExe}" "Demo_Scenes.py" "" "" "" "" ""
  CreateDirectory "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreAL Demos"
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreAL Demos\Basic Sound.lnk" "{code:GetPythonExe}" "Demo_RenderToTexture.py" "" "" "" "" ""
  CreateDirectory "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos"
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Basics.lnk" "{code:GetPythonExe}" "Demo01_TheBasics.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Joints.lnk" "{code:GetPythonExe}" "Demo02_Joints.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Collision Callbacks.lnk" "{code:GetPythonExe}" "Demo03_CollisionCallbacks.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Ray Casting.lnk" "{code:GetPythonExe}" "Demo04_RayCasting.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Simple Vehicle.lnk" "{code:GetPythonExe}" "Demo05_SimpleVehicle.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Simple Buoyancy.lnk" "{code:GetPythonExe}" "Demo06_SimpleBuoyancy.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Custom Joints.lnk" "{code:GetPythonExe}" "Demo07_CustomJoints.py" "" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Ragdoll.lnk" "{code:GetPythonExe}" "Demo08_ragdoll.py" "" "" "" "" ""
  CreateDirectory "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}"
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Uninstall Python-Ogre.lnk" "$INSTDIR\uninstall.exe" 
  CreateShortCut "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Python-Ogre API Documenatation.lnk" "$INSTDIR\docs\Python-Ogre.chm" 
!insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -PostInstall
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "DisplayName" "PythonOgre"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "Publisher" "OpenSource"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "URLInfoAbout" "http://www.python-ogre.org"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "HelpLink" "http://www.python-ogre.org"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "URLUpdateInfo" "http://www.python-ogre.org"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre" "DisplayVersion" "1.0.2.3"
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section -Run
  ExecWait "$INSTDIR\ThirdParty\vcredist_x86.exe"
  DetailPrint "You probably NEED to update your Directx Drivers"
  ExecWait "$INSTDIR\ThirdParty\dxwebsetup.exe"
SectionEnd


#### Uninstaller code ####

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Section Uninstall
  ReadRegStr ${MUI_STARTMENUPAGE_VARIABLE} ${MUI_STARTMENUPAGE_REGISTRY_ROOT} "${MUI_STARTMENUPAGE_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_REGISTRY_VALUENAME}"

  RMDir /r "{code:GetPythonSiteDir}\ogre\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\renderer\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\renderer\OGRE\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\io\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\io\OIS\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\gui\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\gui\CEGUI\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\gui\QuickGUI\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\sound\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\sound\OgreAL\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\physics\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\physics\OgreRefApp\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\physics\ODE\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\physics\OgreOde\*.pyc"
  RMDir /r "{code:GetPythonSiteDir}\ogre\physics\OgreNewt\*.pyc"
  Delete "$INSTDIR\demos\CEGUI\*.pyc"
  Delete "$INSTDIR\demos\CEGUI\*.log"
  Delete "$INSTDIR\demos\GUI\*.pyc"
  Delete "$INSTDIR\demos\GUI\*.log"
  Delete "$INSTDIR\demos\ogre\*.pyc"
  Delete "$INSTDIR\demos\ogre\*.log"
  Delete "$INSTDIR\demos\ogreode\*.pyc"
  Delete "$INSTDIR\demos\ogreode\*.log"
  Delete "$INSTDIR\demos\ogrenewt\*.pyc"
  Delete "$INSTDIR\demos\ogrenewt\*.log"
  Delete "$INSTDIR\demos\ogreal\*.pyc"
  Delete "$INSTDIR\demos\ogreal\*.log"
  Delete "$INSTDIR\demos\ode\*.pyc"
  Delete "$INSTDIR\demos\ode\*.log"
  Delete "$INSTDIR\demos\CEGUI\*.cfg"
  Delete "$INSTDIR\demos\GUI\*.cfg"
  Delete "$INSTDIR\demos\ogre\*.cfg"
  Delete "$INSTDIR\demos\ogreode\*.cfg"
  Delete "$INSTDIR\demos\ogrenewt\*.cfg"
  Delete "$INSTDIR\demos\ogreal\*.cfg"
  Delete "$INSTDIR\demos\ode\*.cfg"
  RMDir "$INSTDIR\demos\CEGUI"
  RMDir "$INSTDIR\demos\GUI"
  RMDir "$INSTDIR\demos\ogre"
  RMDir "$INSTDIR\demos\ogreode"
  RMDir "$INSTDIR\demos\ogrenewt"
  RMDir "$INSTDIR\demos\ogreal"
  RMDir "$INSTDIR\demos\ode"
  RMDir "$INSTDIR\demos"
  RMDir "$INSTDIR"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Python-Ogre"
  DeleteRegValue ${MUI_STARTMENUPAGE_REGISTRY_ROOT} "${MUI_STARTMENUPAGE_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_REGISTRY_VALUENAME}"

  Delete "$INSTDIR\uninstall.exe"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Python-Ogre API Documenatation.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Uninstall Python-Ogre.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Ragdoll.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Custom Joints.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Simple Buoyancy.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Simple Vehicle.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Ray Casting.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Collision Callbacks.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Joints.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos\Basics.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreAL Demos\Basic Sound.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\ODE Demos\Multiple Scenes.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\QuickGUI-01.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\CEGUI - Nice.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\CEGUI Drag and Drop.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI\CEGUI - Basic.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\WX Gui.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Transparency.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Textures.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Terrain.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Smoke.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Sky Plane.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Sky Dome.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Sky Box.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Animation 2.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Animation 1.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Shadows.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Render to Texture.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Particles.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Lighting.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Grass.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Fresnel.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Fish.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Facial.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\ENV Map.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Compositor.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\CEL Shading.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Camera Tracking.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\BSP_Collision.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\BSP.lnk"
  Delete "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos\Bezier.lnk"
  Delete "$INSTDIR\ThirdParty\vcredist_x86.exe"
  Delete "$INSTDIR\ThirdParty\dxwebsetup.exe"
  Delete "$INSTDIR\packages_2.4\*"
  Delete "$INSTDIR\packages_2.5\*"
  Delete "{code:GetPythonSiteDir}\*"
  Delete "$INSTDIR\docs\*"
  Delete "$INSTDIR\demos\*"
  Delete "$INSTDIR\tools\*"
  Delete "$INSTDIR\plugins\*"
  Delete "$INSTDIR\*"

  RMDir "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}"
  RMDir "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreNewt Demos"
  RMDir "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\OgreAL Demos"
  RMDir "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\ODE Demos"
  RMDir "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\GUI"
  RMDir "$SMPROGRAMS\${MUI_STARTMENUPAGE_VARIABLE}\Ogre Demos"
  RMDir "$INSTDIR\ThirdParty"
  RMDir "$INSTDIR\packages_2.4"
  RMDir "$INSTDIR\packages_2.5"
  RMDir "{code:GetPythonSiteDir}"
  RMDir "$INSTDIR\docs"
  RMDir "$INSTDIR\demos"
  RMDir "$INSTDIR\tools"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR"

  SetAutoClose true
SectionEnd

