using PortalLib;

namespace SkylandersToolkit.Commands
{
    internal class SpeakersCommand : Command
    {
        public override string Name => "speakers";

        public override void Execute()
        {
            string option = Commands.RequestStringOption("Do you want to activate or deactivate the speakers? (activate - deactivate): ", new []{ "activate", "deactivate"});

            if(option == "activate")
            {
                Portal.Instance.Music(true);
            }
            else if(option == "deactivate")
            {
                Portal.Instance.Music(false);
            }
        }
    }
}
