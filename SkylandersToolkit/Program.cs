using SkylandersToolkit.Commands;

namespace SkylandersToolkit
{
    internal class Program
    {
        static void Main()
        {
            Commands.Commands.RegisterCommands();

            while (true)
            {
                Console.Write("> ");

                string commandName = Console.ReadLine()!;

                Command? command = Commands.Commands.GetCommand(commandName);

                if (command == null)
                {
                    Console.WriteLine($"The command \"{commandName}\" was not found. Use the help command to get a list of all commands.");
                    continue;
                }

                command.Execute();
            }
        }
    }
}