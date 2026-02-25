param(
    [string]$UpstreamRepo = "https://github.com/FastLED/FastLED.git",
    [string]$Ref = "master"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$tmp = Join-Path $PSScriptRoot "..\_tmp_fastled_sync"
if (Test-Path $tmp) {
    Remove-Item -Recurse -Force $tmp
}

git clone --depth 1 --branch $Ref $UpstreamRepo $tmp

Copy-Item -Recurse -Force (Join-Path $tmp "src") (Join-Path $PSScriptRoot "..\src")
Copy-Item -Force (Join-Path $tmp "PORTING.md") (Join-Path $PSScriptRoot "..\PORTING.md")

$commit = git -C $tmp rev-parse --short HEAD
Write-Host "Synced FastLED commit: $commit"

Remove-Item -Recurse -Force $tmp
