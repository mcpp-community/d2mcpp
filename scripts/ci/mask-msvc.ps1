# Hosted runners only: hide all MSVC discovery paths before installing tools.
$ErrorActionPreference = 'Stop'
$Vswhere = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
if (Test-Path $Vswhere) { Rename-Item $Vswhere 'vswhere.exe.masked' }
Resolve-Path 'C:\Program Files*\Microsoft Visual Studio\*\*\VC' -ErrorAction SilentlyContinue | ForEach-Object {
    Rename-Item -LiteralPath $_.Path -NewName 'VC.masked'
}
foreach ($Name in @('VSINSTALLDIR', 'VCINSTALLDIR', 'VCToolsInstallDir', 'VS170COMNTOOLS', 'VS160COMNTOOLS', 'VS150COMNTOOLS')) {
    Remove-Item "Env:$Name" -ErrorAction SilentlyContinue
    if ($env:GITHUB_ENV) { "$Name=" | Out-File -Append $env:GITHUB_ENV -Encoding utf8 }
}
if (Get-ChildItem 'C:\Program Files*\Microsoft Visual Studio\*\*\VC\Tools\MSVC' -Directory -ErrorAction SilentlyContinue) {
    throw 'MSVC still present after masking'
}
Write-Host 'MSVC discovery masked'
