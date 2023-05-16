using PortalLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SkylandersToolkit.Commands
{
    internal class ReadyCommand : Command
    {
        public override string Name => "ready";

        public override void Execute()
        {
            ushort id = Portal.Instance.Ready();

            Console.WriteLine($"Portal ID: 0x{id:X}");
        }
    }
}
