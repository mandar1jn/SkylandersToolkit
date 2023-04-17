using PortalLib;

namespace SkylandersToolkit.Commands
{
    internal class ColorCommand : Command
    {
        public override string Name => "color";

        public override void Execute()
        {
            byte red = Commands.RequestByte("Red (0-255): ", 0, 255);
            byte green = Commands.RequestByte("Green (0-255): ", 0, 255);
            byte blue = Commands.RequestByte("Blue (0-255): ", 0, 255);

            Portal.Instance.SetColor(red, green, blue);
        }
    }
}
