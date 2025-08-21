$target = "arduino_flat"
if (Test-Path $target) { Remove-Item $target -Recurse -Force }
New-Item -ItemType Directory -Path $target | Out-Null

Get-ChildItem -Path src -Include *.ino,*.cpp,*.c -Recurse | Copy-Item -Destination $target
Get-ChildItem -Path include -Include *.h -Recurse | Copy-Item -Destination $target

if (Test-Path "src/ui.h") { Copy-Item "src/ui.h" $target }
if (Test-Path "src/ui.c") { Copy-Item "src/ui.c" $target }

Write-Host "Tous les fichiers sont dans $target"