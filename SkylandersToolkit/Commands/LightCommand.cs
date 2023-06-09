using PortalLib;
using System;
using System.Collections.Generic;
using System.Drawing;
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
            string sideString = Commands.RequestStringOption("Which side LED do you want to set? (left - right - trap): ", new[] { "left", "right", "trap" });

            PortalSide side;

            switch (sideString)
            {
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
                default:
                case "trap":
                    {
                        if(!Portal.Instance.active)
                        {
                            Console.WriteLine("The portal needs to be activated to set the trap light.");
                            return;
                        }

                        byte brightness = Commands.RequestByte("Brightness (0-255): ", 0, 255);

                        Portal.Instance.SetLightBrightness(brightness);
                        return;
                    }
            }

            byte red = Commands.RequestByte("Red (0-255): ", 0, 255);
            byte green = Commands.RequestByte("Green (0-255): ", 0, 255);
            byte blue = Commands.RequestByte("Blue (0-255): ", 0, 255);

            Portal.Instance.SetLightColorSided(side, red, green, blue);
        }
    }
}
