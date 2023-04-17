using PortalLib;

namespace SkylandersToolkit.Commands
{
    internal class ActivateCommand : Command
    {
        public override string Name => "activate";

        public override void Execute()
        {
            Portal.Instance.Activate();

            Console.WriteLine("Activated the portal!");
        }
    }
}
