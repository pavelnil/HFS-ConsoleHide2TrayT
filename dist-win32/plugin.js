exports.beforePlugin = "admin"
exports.apiRequired = 12
exports.description = "A plugin to work with HFS-ConsoleHide2TrayT, allowing for a window to be hidden in the system tray using a custom console window title"
exports.version = 1
exports.repo = "pavelnil/HFS-ConsoleHide2TrayT"
exports.preview = ["https://github.com/pavelnil/HFS-ConsoleHide2TrayT/blob/main/screenshots/screenshot.jpg?raw=true"]

exports.init = (api) => {
    const child_process = api.require('child_process');
    const os = api.require('os');
    
    let logEnabled = true;
    const log = (message) => {
        if (logEnabled) api.log(`[ConsoleHide2TrayT-agent] ${message}`);
    };

    const uniqueTitle = `HFS_CONSOLE_${process.pid}`;
    log(`Setting console title: ${uniqueTitle}`);
    
    try {
        if (process.platform === 'win32') {
            const script = `
$Signature = @'
[DllImport("kernel32.dll")] 
public static extern bool SetConsoleTitle(string lpConsoleTitle);
'@
$ConsoleTitle = Add-Type -MemberDefinition $Signature -Name "ConsoleTitle" -Namespace "Win32" -PassThru
$ConsoleTitle::SetConsoleTitle("${uniqueTitle}")
            `;
            
            const buffer = Buffer.from(script, 'utf16le');
            const base64Script = buffer.toString('base64');
            child_process.execSync(
                `powershell -ExecutionPolicy Bypass -EncodedCommand "${base64Script}"`,
                { stdio: 'ignore' }
            );
            log("Console title set via WinAPI");
        }
    } catch (e) {
        log(`Error setting console title: ${e.message}`);
        try {
            child_process.execSync(`title ${uniqueTitle}`, { stdio: 'ignore' });
            log("Console title set via title command");
        } catch (err) {
            log(`Fallback title command failed: ${err.message}`);
        }
    }

    log("Plugin initialized successfully");
    
    return {
        unload: () => {
            log("Plugin unloaded");
        }
    };
};