cd urho3d
call cmake_vs2015.bat ..\urho3d-build -DURHO3D_SAMPLES=0 -DURHO3D_D3D11=1 -DURHO3D_PACKAGING=1 -DURHO3D_STATIC_RUNTIME=0

cd ..\urho3d-build
call "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" Urho3d.sln /t:Build /p:Configuration=Release /verbosity:minimal /maxcpucount
call "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" Urho3d.sln /t:Build /p:Configuration=Debug /verbosity:minimal /maxcpucount

cd ..
