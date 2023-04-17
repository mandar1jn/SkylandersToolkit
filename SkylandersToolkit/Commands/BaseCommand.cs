namespace SkylandersToolkit.Commands
{
    internal abstract class Command
    {
        public abstract string Name { get; }

        public abstract void Execute();
    }
}
