# BCM Auto-Build Script - runs make in MSYS2 and reports errors
$Msys2Bash = "D:\MSYS2\usr\bin\bash.exe"
$ProjectRoot = "D:\AutoSAR_Project\classic-platform-master"
$env:CHERE_INVOKING = "1"

$buildCmd = @"
cd /d/AutoSAR_Project/classic-platform-master && make BOARDDIR=stm32f103_bcm CROSS_COMPILE=arm-none-eabi- BDIR=bcm_app clean all 2>&1
"@

Write-Host "=== BCM Auto-Build ===" -ForegroundColor Cyan
Write-Host "MSYS2: $Msys2Bash"
Write-Host "Project: $ProjectRoot"

try {
    $result = & $Msys2Bash -l -c "`"$buildCmd`""
    Write-Host $result
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`n=== BUILD SUCCESS ===" -ForegroundColor Green
        # Check for elf file
        if (Test-Path "$ProjectRoot\binaries\stm32f103_bcm\bcm_app.elf") {
            Write-Host "ELF output: binaries/stm32f103_bcm/bcm_app.elf" -ForegroundColor Green
        }
    } else {
        Write-Host "`n=== BUILD FAILED (Exit code: $LASTEXITCODE) ===" -ForegroundColor Red
        
        # Extract error patterns
        $errors = $result | Select-String -Pattern "error:|fatal error:|undefined reference|No rule to make target|No such file" 
        Write-Host "`n--- Error Summary ---" -ForegroundColor Yellow
        foreach ($e in $errors) {
            Write-Host $e.Line -ForegroundColor Red
        }
    }
} catch {
    Write-Host "`n=== SCRIPT ERROR ===" -ForegroundColor Red
    Write-Host $_.Exception.Message
}
