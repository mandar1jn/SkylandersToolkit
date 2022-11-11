using PortalLib;

namespace SkylandersEditor
{

    internal class Program
    {

        static void Main()
        {
            Portal portal = Portal.Instance;

            portal.Ready();

            portal.Activate();

            portal.SetColor(0x00, 0x00, 0xFF);

            StatusResult portalStatus = portal.GetStatus();

            if (portalStatus.characterIndexes.Count == 0)
                Console.WriteLine("Waiting for a character on the portal...");

            while (portalStatus.characterIndexes.Count == 0)
                portalStatus = portal.GetStatus();

            Console.WriteLine("Figures on portal: " + portalStatus.characterIndexes.Count);

            if (portalStatus.characterIndexes.Count > 0)
            {

                Figure fig = new Figure();

                fig.ReadData(portalStatus.characterIndexes[0]);

                fig.LogData();

                Console.WriteLine(fig.ID);

                Console.WriteLine(fig.VariantID);

                Console.WriteLine(fig.Type);
            }
            else
            {
                Console.WriteLine("No figure on the portal");
            }

            portalStatus = portal.GetStatus();

            portal.Deactivate();

            portal.Close();
        }
    }
}