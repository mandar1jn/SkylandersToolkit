using PortalLib;

namespace SkylandersToolkit.Commands
{
    internal class StatusCommand : Command
    {
        public override string Name => "status";

        public override void Execute()
        {
            Console.WriteLine($"Portal ID: {Portal.Instance.id}");

            StatusResult status = Portal.Instance.GetStatus();

            Console.WriteLine($"Portal ready: {status.ready}");
            Console.WriteLine($"Portal active: {Portal.Instance.active}");
            Console.WriteLine($"Portal counter: {status.count}");

            if (Portal.Instance.active)
            {
                if (status.characterIndexes.Count == 0)
                {
                    Console.WriteLine("No characters on the portal");
                }
                else
                {
                    string indexes = "";
                    for (byte i = 0; i < status.characterIndexes.Count; i++)
                    {
                        if (i == 0)
                        {
                            indexes += $"{status.characterIndexes[i]}";
                        }
                        else
                        {
                            indexes += $", {status.characterIndexes[i]}";
                        }
                    }
                    Console.WriteLine($"Character indexes: {indexes}");
                }
            }
            else
            {
                Console.WriteLine("Please activate the portal to view figure indexes");
            }
        }
    }
}
