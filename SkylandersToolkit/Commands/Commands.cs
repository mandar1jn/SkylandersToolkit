using System.Reflection;

namespace SkylandersToolkit.Commands
{
    internal class Commands
    {
        static List<Command> commands = new();

        public static void RegisterCommands()
        {
            foreach (Type type in Assembly.GetAssembly(typeof(Command))!.GetTypes()
            .Where(myType => myType.IsClass && !myType.IsAbstract && myType.IsSubclassOf(typeof(Command))))
            {
                commands.Add((Command)Activator.CreateInstance(type)!);
            }
        }

        public static Command? GetCommand(string commandName)
        {
            foreach(Command command in commands)
            {
                if(command.Name ==  commandName)
                    return command;
            }

            return null;
        }

        public static ref List<Command> GetCommandList()
        {
            return ref commands;
        }

        public static byte RequestByte(string message, byte min, byte max)
        {
            while (true)
            {
                Console.Write(message);

                if (byte.TryParse(Console.ReadLine(), out byte number))
                {
                    if (number < min || number > max)
                    {
                        Console.WriteLine("Please enter a valid number");
                        continue;
                    }
                    return number;
                }
                else
                {
                    Console.WriteLine("Please enter a valid number");
                    continue;
                }

            }
        }

        public static string RequestStringOption(string message, string[] options)
        {
            while (true)
            {
                Console.Write(message);

                string option = Console.ReadLine()!;

                if(!options.Contains(option))
                {
                    Console.WriteLine("Please choose a valid option");
                    continue;
                }

                return option;

            }
        }

        public static string RequestPath(string message)
        {
            while (true)
            {
                Console.Write(message);

                string path = Path.GetFullPath(Console.ReadLine()!);

                if (!File.Exists(path))
                {
                    Console.WriteLine("Please enter a valid path");
                    continue;
                }

                return path;
            }
        }

        public static string RequestString(string message)
        {
            Console.Write(message);

            return Console.ReadLine()!;
        }
    }
}
