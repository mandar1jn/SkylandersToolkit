using PortalLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SkylandersToolkit.Commands
{
    internal class SidedColorCommand : Command
    {
        public override string Name => "color_sided";

        public override void Execute()
        {
            string sideString = Commands.RequestStringOption("Which side should the color be applied to? (left - right): ", new[] { "left", "right" });

            PortalSide side;

            switch (sideString)
            {
                default:
                case "left":
                    {
                        side = PortalSide.LEFT;
                        break;
                    }
                case "right":
                    {
                        side = PortalSide.RIGHT;
                        break;
                    }
            }

            byte red = Commands.RequestByte("Red (0-255): ", 0, 255);
            byte green = Commands.RequestByte("Green (0-255): ", 0, 255);
            byte blue = Commands.RequestByte("Blue (0-255): ", 0, 255);

            Portal.Instance.SetColorSided(side, red, green, blue, 0x00, 0x00);
        }
    }
}
