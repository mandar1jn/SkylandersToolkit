using PortalLib;

namespace SkylandersToolkit.Commands
{
    internal class DeactivateCommand : Command
    {
        public override string Name => "deactivate";

        public override void Execute()
        {
            Portal.Instance.Deactivate();

            Console.WriteLine("Deactivated the portal!");
        }
    }
}
