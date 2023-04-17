using PortalLib;

namespace SkylandersToolkit.Commands
{
    internal class QueryCommand : Command
    {
        public override string Name => "query";

        public override void Execute()
        {
            if(!Portal.Instance.active)
            {
                Console.WriteLine("The portal needs to be activated to use this command");
                return;
            }

            byte index = Commands.RequestByte("Figure index (0 - 15): ", 0, 15);

            if(!Portal.Instance.GetStatus().characterIndexes.Contains((byte)index))
            {
                Console.WriteLine($"There is no figure with index {index} on the portal");
                return;
            }

            byte block = Commands.RequestByte("Storage block (0 - 63): ", 0, 63);

            byte[] data = Portal.Instance.Query(index, block);

            string queryResult = $"0x{data[3]:X2}";

            for(byte i = 1; i < 16; i++)
            {
                queryResult += $", 0x{data[3 + i]:X2}";
            }

            Console.WriteLine(queryResult);
        }
    }
}
