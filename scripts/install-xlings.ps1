# Dot-source to retain PATH. Compatible with Windows PowerShell 5.1.
$ErrorActionPreference = 'Stop'
$VersionFile = if ($PSScriptRoot) { Join-Path (Split-Path $PSScriptRoot -Parent) 'scripts/tool-versions.env' } else { $null }
if ($VersionFile -and (Test-Path $VersionFile)) {
    $VersionText = Get-Content $VersionFile -Raw
} else {
    # Also usable as the bootstrap command before the course has been downloaded.
    $VersionText = (Invoke-WebRequest 'https://raw.githubusercontent.com/mcpp-community/d2mcpp/main/scripts/tool-versions.env' -UseBasicParsing).Content
}
$Versions = $VersionText | ConvertFrom-StringData
$env:XLINGS_VERSION = $Versions.XLINGS_VERSION
$env:XLINGS_NON_INTERACTIVE = '1'
$env:PATH = "$env:USERPROFILE\.xlings\subos\current\bin;$env:USERPROFILE\.xlings\bin;$env:PATH"
$Existing = Get-Command xlings -ErrorAction SilentlyContinue
if (-not $Existing -or (& xlings --version) -ne "xlings $env:XLINGS_VERSION") {
    # The pinned upstream quick installer probes with a restricted Range header,
    # which Windows PowerShell 5.1 rejects. Download its immutable release assets
    # directly, verify the sidecar, and use the same `self install` entry point.
    $Temp = Join-Path ([IO.Path]::GetTempPath()) ('xlings-bootstrap-' + [guid]::NewGuid())
    New-Item -ItemType Directory -Path $Temp | Out-Null
    try {
        if (-not [Environment]::Is64BitOperatingSystem -or $env:PROCESSOR_ARCHITECTURE -eq 'ARM64') { throw 'This release requires Windows x86_64' }
        $Asset = "xlings-$env:XLINGS_VERSION-windows-x86_64.zip"
        $Sources = @(
            "https://github.com/openxlings/xlings/releases/download/v$env:XLINGS_VERSION/$Asset",
            "https://gitcode.com/xlings-res/xlings/releases/download/$env:XLINGS_VERSION/$Asset"
        )
        $Zip = Join-Path $Temp $Asset
        $Downloaded = $false
        foreach ($Url in $Sources) {
            try {
                Write-Host "Downloading $Url"
                Invoke-WebRequest $Url -UseBasicParsing -OutFile $Zip
                Invoke-WebRequest "$Url.sha256" -UseBasicParsing -OutFile "$Zip.sha256"
                $Sidecar = (Get-Content "$Zip.sha256" -Raw).Trim()
                $Hash = [regex]::Match($Sidecar, '^([a-fA-F0-9]{64})(?:\s|$)')
                if (-not $Hash.Success -or (Get-FileHash $Zip -Algorithm SHA256).Hash -ne $Hash.Groups[1].Value) { throw 'SHA256 mismatch' }
                $Downloaded = $true
                break
            } catch { Write-Warning $_.Exception.Message }
        }
        if (-not $Downloaded) { throw 'All xlings release sources failed' }
        Expand-Archive $Zip -DestinationPath $Temp
        $Package = Get-ChildItem $Temp -Directory -Filter 'xlings-*' | Select-Object -First 1
        if (-not $Package) { throw 'Invalid xlings archive' }
        & (Join-Path $Package.FullName 'bin/xlings.exe') self install
        if ($LASTEXITCODE -ne 0) { throw 'xlings self install failed' }
    } finally {
        Remove-Item $Temp -Recurse -Force -ErrorAction SilentlyContinue
    }
}
if ((& xlings --version) -ne "xlings $env:XLINGS_VERSION") { throw 'Unexpected xlings version' }
if ($env:GITHUB_PATH) {
    "$env:USERPROFILE\.xlings\subos\current\bin" | Out-File -Append $env:GITHUB_PATH -Encoding utf8
}
xlings --version
