# Dot-source to retain PATH. Compatible with Windows PowerShell 5.1.
$ErrorActionPreference = 'Stop'
$InstallRoot = Split-Path $PSScriptRoot -Parent
$Versions = Get-Content (Join-Path $InstallRoot 'scripts/tool-versions.env') -Raw | ConvertFrom-StringData
$env:XLINGS_VERSION = $Versions.XLINGS_VERSION
$env:XLINGS_NON_INTERACTIVE = '1'
$env:PATH = "$env:USERPROFILE\.xlings\subos\current\bin;$env:USERPROFILE\.xlings\bin;$env:PATH"
$Existing = Get-Command xlings -ErrorAction SilentlyContinue
if (-not $Existing -or (& xlings --version) -ne "xlings $env:XLINGS_VERSION") {
    $Installer = Join-Path ([IO.Path]::GetTempPath()) ('xlings-install-' + [guid]::NewGuid() + '.ps1')
    try {
        Invoke-WebRequest "https://raw.githubusercontent.com/openxlings/xlings/v$env:XLINGS_VERSION/tools/other/quick_install.ps1" -UseBasicParsing -OutFile $Installer
        & $Installer -Version $env:XLINGS_VERSION
    } finally {
        Remove-Item $Installer -ErrorAction SilentlyContinue
    }
}
if ((& xlings --version) -ne "xlings $env:XLINGS_VERSION") { throw 'Unexpected xlings version' }
if ($env:GITHUB_PATH) {
    "$env:USERPROFILE\.xlings\subos\current\bin" | Out-File -Append $env:GITHUB_PATH -Encoding utf8
}
xlings --version
