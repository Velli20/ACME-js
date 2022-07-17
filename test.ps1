# Find CMake program.

$CMAKE = 'C:\Program Files\CMake\bin\cmake.exe'

if ( [string]::IsNullOrEmpty($CMAKE) )
{
    $CMAKE_URL = "https://cmake.org/download/"
    "Cannot find CMake.Get latest version from: '$CMAKE_URL'" | Write-Host -ForegroundColor Red
    exit 1
}

$CMAKE_ARGS = '-B', 'build',    `
              '-T', 'host=x86', `
              '-A', 'win32'

&$CMAKE $CMAKE_ARGS
&$CMAKE --build build -j 3