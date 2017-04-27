param([string]$mode = "ROOT")
$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSCommandPath
Push-Location $root

$bins = @()

Write-Host "Finding Alteryx Admin Install Location..."
$reg = Get-ItemProperty HKLM:\SOFTWARE\WOW6432Node\SRC\Alteryx -ErrorAction SilentlyContinue
if ($reg -ne $null) {
    $bins += $reg.InstallDir64
}

Write-Host "Finding Alteryx User Install Location..."
$reg = Get-ItemProperty HKCU:\SOFTWARE\SRC\Alteryx -ErrorAction SilentlyContinue
if ($reg -ne $null) {
    $bins += $reg.InstallDir64
}

if ($bins.Count -eq 0) {
    Write-Host "Failed to find Alteryx Install"
    Pop-Location
    exit -1
}

$files = Get-ChildItem "$root\*.xml" 
if ($mode.ToUpper() -eq "DEBUG") { $files += Get-ChildItem "$root\JDFormulaAddIn\x64\Debug\*.dll" }
if ($mode.ToUpper() -eq "RELEASE") { $files += Get-ChildItem "$root\JDFormulaAddIn\x64\Release\*.dll" }
if ($mode.ToUpper() -eq "ROOT") { $files += Get-ChildItem "$root\*.dll" }
$files | Unblock-File

foreach ($bin in $bins) {
    Write-Host "Installing current version to $bin ..."
    $files | Copy-Item -Destination "$bin\RuntimeData\FormulaAddIn"
}

Pop-Location
exit 0