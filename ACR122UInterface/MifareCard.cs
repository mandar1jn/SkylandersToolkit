using PCSC;
using PCSC.Iso7816;

namespace ACR122UInterface
{
    internal class MifareCard
    {
        readonly ICardReader reader;

        public MifareCard(ICardReader reader) {

            this.reader = reader;
        }

        public byte[] GetNUID()
        {

            var getNuidCommand = new CommandApdu(IsoCase.Case2Short, SCardProtocol.Any)
            {
                CLA = 0xFF,
                Instruction = InstructionCode.GetData,
                P1 = 0x00,
                P2 = 0x00,
            };

            byte[] nuid = new byte[6];

            reader.Transmit(getNuidCommand.ToArray(), nuid);

            Array.Resize(ref nuid, 4);

            return nuid;
        }

        public void LoadKey(byte[] key)
        {
            var loadKeyCommand = new CommandApdu(IsoCase.Case3Short, SCardProtocol.Any)
            {
                CLA = 0xFF,
                Instruction = InstructionCode.ExternalAuthenticate,
                P1 = 0x00,
                P2 = 0x00,
                Data = key
            };

            byte[] ret = new byte[2];
            reader.Transmit(loadKeyCommand.ToArray(), ret);

            if (ret[0] != 0x90)
            {
                Console.WriteLine("Failed to load key");
                Console.ReadLine();
                throw new Exception();
            }
        }

        public bool Authenticate(byte block)
        {
            var authenticateCommand = new CommandApdu(IsoCase.Case3Short, SCardProtocol.Any)
            {
                CLA = 0xFF,
                Instruction = InstructionCode.InternalAuthenticate,
                P1 = 0x00,
                P2 = 0x00,
                Data = new byte[] { 0x01, 0x00, block, 0x60, 0x00 }
            };

            byte[] ret = new byte[2];
            reader.Transmit(authenticateCommand.ToArray(), ret);

            return (ret[0] == 0x90);
        }

        public byte[] ReadBlock(byte block)
        {
            var readCommand = new CommandApdu(IsoCase.Case2Short, SCardProtocol.Any)
            {
                CLA = 0xFF,
                Instruction = InstructionCode.ReadBinary,
                P1 = 0x00,
                P2 = block,
                Le = 16
            };

            var ret = new byte[18];

            reader.Transmit(readCommand.ToArray(), ret);

            if (ret[16] != 0x90)
            {
                Console.WriteLine($"Failed to read block {block}");
                throw new Exception();
            }

            Array.Resize(ref ret, 16);

            return ret;
        }
    }
}
