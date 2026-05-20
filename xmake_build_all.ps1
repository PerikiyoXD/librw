$combos = @(
    @{ backend = "null";  gfxlib = $null },
    @{ backend = "d3d9";  gfxlib = $null },
    @{ backend = "gl3";   gfxlib = "glfw" },
    @{ backend = "gl3";   gfxlib = "sdl2" },
    @{ backend = "gl3";   gfxlib = "sdl3" }
)

$modes = @("release", "debug")

$failed = @()

foreach ($mode in $modes) {
    foreach ($c in $combos) {
        $backend = $c.backend
        $gfxlib  = $c.gfxlib

        $desc = "$mode $backend"
        $args = "f -m $mode --backend=$backend"
        if ($gfxlib) {
            $args += " --gfxlib=$gfxlib"
            $desc += "-$gfxlib"
        }

        Write-Host ""
        Write-Host "==> $desc" -ForegroundColor Cyan

        Invoke-Expression "xmake $args"
        if ($LASTEXITCODE -ne 0) { $failed += "$desc (config)"; continue }

        xmake -yDr
        if ($LASTEXITCODE -ne 0) { $failed += "$desc (build)" }
    }
}

Write-Host ""
if ($failed.Count -eq 0) {
    Write-Host "All combos built successfully." -ForegroundColor Green
} else {
    Write-Host "Failed combos:" -ForegroundColor Red
    $failed | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    exit 1
}