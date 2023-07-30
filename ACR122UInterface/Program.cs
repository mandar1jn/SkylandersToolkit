using PCSC;
using PCSC.Iso7816;
using PCSC.Monitoring;
using PCSC.Reactive.Events;
using System.Formats.Asn1;

namespace ACR122UInterface
{
    internal class Program
    {
#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider declaring as nullable.
        private static ISCardContext cardContext;
#pragma warning restore CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider declaring as nullable.

        static void Main(string[] args)
        {
            cardContext = ContextFactory.Instance.Establish(SCardScope.System);

            var cardReaderNames = cardContext.GetReaders();

            var monitor = MonitorFactory.Instance.Create(SCardScope.System);
            monitor.CardInserted += OnCardInserted;
            monitor.CardRemoved += OnCardRemoved;
            monitor.Start(new string[]{ "ACS ACR122 0" });

            Console.WriteLine("Please insert a card");

            while (true)
            {
                
            }

            cardContext.Dispose();
            return;

        }

        private static void OnCardRemoved(object sender, CardStatusEventArgs e)
        {
            //throw new NotImplementedException();
        }

        private static void OnCardInserted(object sender, CardStatusEventArgs e)
        {
            using ICardReader reader = cardContext.ConnectReader(e.ReaderName, SCardShareMode.Shared, SCardProtocol.Any);

            MifareCard card = new MifareCard(reader);

            byte[] nuid = card.GetNUID();

            Console.Write("NUID: ");

            for(int i = 0; i < nuid.Length; i++)
            {
                Console.Write(nuid[i].ToString("X2"));
            }
            Console.Write("\n");

            byte[] cardData = new byte[1024];

            for(byte i = 0; i < 16; i++)
            {
                byte[] key = Crypto.CalculateKeyA(i, nuid);

                card.LoadKey(key);

                card.Authenticate((byte)(i * 4));

                for(byte j = 0; j < 4; j++)
                {
                    byte block = (byte)((i * 4) + j);

                    byte[] data = card.ReadBlock(block);

                    data.CopyTo(cardData, block * 16);
                }
            }

            Console.WriteLine("Finished reading card");

            Console.Write("Please enter a file name: ");

            string fileName = Console.ReadLine()!;

            string filePath = fileName + ".dump";

            string path = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), filePath));

            FileStream file = File.Create(filePath);

            file.Write(cardData);

            file.Close();

            Console.WriteLine("Successfully dumped file");
        }
    }
}