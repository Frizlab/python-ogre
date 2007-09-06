del /Q cache\*
echo rmdir /s /q ..\generated
cd ogre
python generate_code.py >1
cd ..
cd ogreode
python generate_code.py >1
cd ..
cd ogrenewt
python generate_code.py >1
cd ..
cd ogreal
python generate_code.py >1
cd ..
cd ogrerefapp
python generate_code.py >1
cd ..
cd ode
python generate_code.py >1
cd ..
cd ois
python generate_code.py >1
cd ..
cd cegui
python generate_code.py >1
cd ..
cd quickgui
python generate_code.py >1
cd ..


