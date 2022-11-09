using PortalLib;
using System.Text;

namespace SkylandersEditor
{

    internal class Program
    {

        static void Main(string[] args)
        {
            Portal portal = Portal.Instance;

            portal.Activate();

            Figure fig = new Figure();

            fig.ReadData(0);

            fig.LogData();

            portal.Deactivate();

            portal.Close();
        }
    }
}