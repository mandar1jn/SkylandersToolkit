using PortalLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SkylandersToolkit.Commands
{
    internal class DumpCommand : Command
    {
        public override string Name => "dump";

        public override void Execute()
        {
            if(!Portal.Instance.active)
            {
                Console.WriteLine("The portal needs to be activated to use this command");
                return;
            }

            byte index = Commands.RequestByte("Figure index (0 - 15): ", 0, 15);

            if (!Portal.Instance.GetStatus().characterIndexes.Contains((byte)index))
            {
                Console.WriteLine($"There is no figure with index {index} on the portal");
                return;
            }

            string fileName = Commands.RequestString("File name: ");

            Figure figure = new();
            figure.ReadData(index);

            {
                string filePath = fileName + ".dump";

                string path = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), filePath));

                
                figure.Dump(false, path);

                Console.WriteLine($"Successfully dumped the normal figure at {path}");
            }

            {
                string filePath = fileName + ".decrypted.dump";

                string path = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), filePath));

                figure.Dump(true, path);

                Console.WriteLine($"Successfully dumped the decrypted figure at {path}");
            }
        }
    }
}
