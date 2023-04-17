using PortalLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SkylandersToolkit.Commands
{
    internal class LightCommand : Command
    {
        public override string Name => "light";

        public override void Execute()
        {
            byte brightness = Commands.RequestByte("Brightness (0-255): ", 0, 255);

            Portal.Instance.SetLightColor(brightness);
        }
    }
}
