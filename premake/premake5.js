const { execFile } = require('child_process');

switch(process.platform)
{
    case "win32":
        execFile('premake5.exe vs2022', [], (error, stdout, stderr) => {
            if (error) {
              throw error;
            }
            console.log(stdout);
          });
          
        break;
}