using PortalLib;
using System.Text;

namespace SkylandersToolkit.Commands
{
    internal class PlayCommand : Command
    {
        public override string Name => "play";

        public override void Execute()
        {
            string path = Commands.RequestPath("Please enter the path to the wav file: ");

            Portal.Instance.Ready();

            Portal.Instance.Activate();

            Portal.Instance.Music(true);

            byte[] data = File.ReadAllBytes(path);

            string readerAsText = Encoding.ASCII.GetString(data);

            int dataIndex = readerAsText.IndexOf("data");

            byte[] byteArray;

            using (BinaryReader reader = new(File.OpenRead(path)))
            {
                reader.BaseStream.Position = dataIndex + 4;
                int dataLength = reader.ReadInt32();
                byteArray = reader.ReadBytes(dataLength);
                reader.Close();
            }

            for(int j = 0; j < byteArray.Length; j += 2)
            {
                (byteArray[j + 1], byteArray[j]) = (byteArray[j], byteArray[j + 1]);
            }

            int i = 0;

            while (i < byteArray.Length)
            {
                byte[] sound = new byte[32];

                for(int j = 0; j < sound.Length; j++)
                {
                    if (i >= byteArray.Length)
                        break;

                    sound[j] = byteArray[i++];
                }
                Portal.Instance.SendRawData(sound);
            }

            Portal.Instance.Music(false);
        }
    }
}
