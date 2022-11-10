using PortalLib;

namespace SkylandersEditor
{

    internal class Program
    {

        static void Main(string[] args)
        {
            Portal portal = Portal.Instance;

            portal.Activate();

            portal.SetColor(0x00, 0x00, 0xFF);

            Figure fig = new Figure();

            fig.ReadData(0);

            fig.LogData();

            Console.WriteLine(fig.ID);

            portal.Deactivate();

            portal.Close();
        }
    }
}