using PortalLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SkylandersToolkit.Commands
{
    internal class AnalyzeCommand : Command
    {
        public override string Name => "analyze";

        public override void Execute()
        {
            if (!Portal.Instance.active)
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

            Figure figure = new();
            figure.ReadData(index);

            Console.WriteLine($"NUID: 0x{figure.NUID:X}");
            Console.WriteLine($"BCC: 0x{figure.NUID:X}");
            Console.WriteLine($"SAK: 0x{figure.SAK:X}");
            Console.WriteLine($"ATQA: 0x{figure.ATQA:X}");
            Console.WriteLine($"Production year: {figure.ProductionYear}");
            Console.WriteLine($"Figure ID: 0x{figure.ID:X}");
            Console.WriteLine($"Variant ID: 0x{figure.VariantID}");
            Console.WriteLine($"Toy code number: 0x{figure.FullToyCodeNumber:X}");
            Console.WriteLine($"Toy code string: {figure.ToyCode}");

            Console.WriteLine($"Area 1 counter: 0x{figure.CounterArea1:X}");
            Console.WriteLine($"Area 2 counter: 0x{figure.CounterArea2:X}");

            Console.WriteLine($"Nickname: {figure.Nickname}");

        }
    }
}
