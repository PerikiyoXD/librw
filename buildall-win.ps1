[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# Every device x gfxlib combination xmake/matrix.lua allows on windows.
# Keep in sync with LIBRW_PLATFORM_BACKENDS.windows and LIBRW_GFXLIB_ORDER.
$configurations = @(
    [pscustomobject]@{ Backend = "gl3"; GfxLib = "sdl2" },
    [pscustomobject]@{ Backend = "gl3"; GfxLib = "sdl3" },
    [pscustomobject]@{ Backend = "gl3"; GfxLib = "glfw" },
    [pscustomobject]@{ Backend = "d3d9"; GfxLib = $null },
    [pscustomobject]@{ Backend = "null"; GfxLib = $null }
)

$runStarted = Get-Date
$runId = $runStarted.ToString("yyyyMMdd-HHmmss")
$logDirectory = Join-Path $PSScriptRoot "build\logs\buildall-win-$runId"
New-Item -ItemType Directory -Path $logDirectory -Force | Out-Null

function Run-XmakeStep {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Step,

        [Parameter(Mandatory = $true)]
        [string[]]$Arguments,

        [Parameter(Mandatory = $true)]
        [string]$LogPath
    )

    $command = "xmake " + ($Arguments -join " ")
    $started = Get-Date
    Write-Host ("[{0:HH:mm:ss}] {1}" -f $started, $Step) -ForegroundColor Cyan
    Write-Host ("> {0}" -f $command) -ForegroundColor DarkGray

    & xmake @Arguments 2>&1 |
        Tee-Object -FilePath $LogPath -Append |
        ForEach-Object { Write-Host $_ }
    $exitCode = $LASTEXITCODE

    $elapsed = (Get-Date) - $started
    $color = if ($exitCode -eq 0) { "Green" } else { "Red" }
    Write-Host ("[{0:HH:mm:ss}] Exit {1} after {2:n1}s" -f (Get-Date), $exitCode, $elapsed.TotalSeconds) -ForegroundColor $color
    return $exitCode
}

$results = foreach ($configuration in $configurations) {
    $backend = $configuration.Backend
    $gfxlib = $configuration.GfxLib
    $name = if ($gfxlib) { "$backend-$gfxlib" } else { $backend }
    $logPath = Join-Path $logDirectory "$name.log"
    $configurationStarted = Get-Date

    Write-Host ""
    Write-Host ("========== {0} ==========" -f $name) -ForegroundColor Yellow

    $configureArguments = @(
        "f",
        "-p", "windows",
        "-a", "x64",
        "--backend=$backend",
        "-y"
    )
    if ($gfxlib) {
        $configureArguments += "--gfxlib=$gfxlib"
    }

    $configureExitCode = Run-XmakeStep -Step "Configure $name" -Arguments $configureArguments -LogPath $logPath

    $buildExitCode = $null
    if ($configureExitCode -eq 0) {
        $buildExitCode = Run-XmakeStep -Step "Rebuild $name" -Arguments @("-y", "-r") -LogPath $logPath
    }

    $succeeded = $configureExitCode -eq 0 -and $buildExitCode -eq 0
    $failure = $null
    if (-not $succeeded) {
        $failureMatch = Select-String -Path $logPath -Pattern "error LNK", "error C\d+", "error:" -CaseSensitive:$false |
            Select-Object -First 1
        if ($failureMatch) {
            $failure = ($failureMatch.Line -replace "\s+", " ").Trim()
        } elseif ($configureExitCode -ne 0) {
            $failure = "configuration exited with code $configureExitCode"
        } else {
            $failure = "build exited with code $buildExitCode"
        }
    }

    [pscustomobject]@{
        Configuration = $name
        Status = if ($succeeded) { "OK" } else { "FAIL" }
        Seconds = [math]::Round(((Get-Date) - $configurationStarted).TotalSeconds, 1)
        Failure = $failure
        Log = $logPath
    }
}

Write-Host ""
Write-Host "========== Build summary ==========" -ForegroundColor Yellow
foreach ($result in $results) {
    $color = if ($result.Status -eq "OK") { "Green" } else { "Red" }
    Write-Host ("{0,-12} {1,-4} {2,7:n1}s" -f $result.Configuration, $result.Status, $result.Seconds) -ForegroundColor $color
    if ($result.Failure) {
        Write-Host ("  {0}" -f $result.Failure) -ForegroundColor Red
    }
    Write-Host ("  log: {0}" -f $result.Log) -ForegroundColor DarkGray
}

$totalElapsed = (Get-Date) - $runStarted
$failed = @($results | Where-Object { $_.Status -ne "OK" })
Write-Host ("Total: {0:n1}s. Logs: {1}" -f $totalElapsed.TotalSeconds, $logDirectory)

if ($failed.Count -gt 0) {
    exit 1
}

Write-Host "All Windows x64 configurations built successfully." -ForegroundColor Green
