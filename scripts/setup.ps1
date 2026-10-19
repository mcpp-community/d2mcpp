param([switch]$InstallOnly)
$ErrorActionPreference = 'Stop'
Set-Location (Split-Path $PSScriptRoot -Parent)
. "$PSScriptRoot/install-xlings.ps1"
function Invoke-Checked {
    param([string]$Tool, [string[]]$Arguments)
    & $Tool @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Tool $Arguments failed ($LASTEXITCODE). See docs/environment.md." }
}
Write-Host '[1/3] Installing the course versions from .xlings.json'
Invoke-Checked xlings @('update')
Invoke-Checked xlings @('install', '-y')
$Workspace = (Get-Content .xlings.json -Raw | ConvertFrom-Json).workspace
foreach ($Tool in @('mcpp', 'd2x')) {
    $Actual = & $Tool --version
    if ($LASTEXITCODE -ne 0 -or ($Actual -split ' ')[-1] -ne $Workspace.$Tool) { throw "Unexpected $Tool version: $Actual" }
    Write-Host (Get-Command $Tool).Source
    Write-Host $Actual
}
if ($InstallOnly) { return }
Write-Host '[2/3] Building the Provider (the first build downloads its C++ toolchain)'
Invoke-Checked mcpp @('build', '-p', 'd2x/buildtools')
Write-Host '[3/3] Checking the Provider'
$Description = & mcpp run -q -p d2x/buildtools -- describe
if ($LASTEXITCODE -ne 0) { throw 'Provider describe failed' }
$Events = @($Description | Where-Object { $_ -match '^\s*\{' } | ForEach-Object { $_ | ConvertFrom-Json })
if (-not ($Events | Where-Object { $_.event -eq 'describe' -and $_.protocol -eq 1 -and $_.name -eq 'mcpp' })) { throw 'Invalid Provider description' }
$Exercises = & mcpp run -q -p d2x/buildtools -- exercises
if ($LASTEXITCODE -ne 0 -or -not ($Exercises | Select-String '"event":"exercise"')) { throw 'Provider returned no exercises' }
Write-Host 'Provider ready. Verify with: mcpp test -p solutions'
Write-Host 'Then start practicing: d2x checker'
