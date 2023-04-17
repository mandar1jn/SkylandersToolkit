namespace SkylandersToolkit.Commands
{
    internal class HelpCommand : Command
    {
        public override string Name => "help";

        public override void Execute()
        {
            List<Command> commands = Commands.GetCommandList();

            foreach (Command command in commands)
            {
                Console.WriteLine(command.Name);
            }
        }
    }
}
