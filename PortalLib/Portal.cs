using System.Collections;

namespace PortalLib
{

    public enum PortalSide: byte
    {
        RIGHT = 0x00,
        BOTH = 0x01,
        LEFT = 0x02,
    }

    public struct StatusResult
    {
        public bool ready = false;
        public byte count = 0;
        public List<byte> characterIndexes = new();

        public StatusResult()
        {
        }
    }

    public class Portal
    {
        readonly PortalConnection portalConnection;
        public ushort id;
        public bool active = false;

        private static Portal? _instance;
        public static Portal Instance
        {
            get
            {
                _instance ??= new Portal();
                return _instance;
            }
            set
            {
                _instance = value;
            }
        }

        public Portal()
        {
            portalConnection = new PortalConnection();

            Ready();

            Deactivate();

            Instance = this;
        }

        public void Ready()
        {
            byte[] readyCommand = new byte[0x21];
            readyCommand[1] = Convert.ToByte('R');

            byte[] output;

            do
            {
                portalConnection.Write(readyCommand);

                output = portalConnection.Read();
            } while (output[0] != Convert.ToByte('R'));

            id = Convert.ToUInt16((output[1] << 8) + output[2]);
        }

        public void Activate()
        {
            byte[] activationCommand = new byte[0x21];
            activationCommand[1] = Convert.ToByte('A');
            activationCommand[2] = 0x01;

            byte[] output;

            do
            {
                portalConnection.Write(activationCommand);

                output = portalConnection.Read();
            } while (output[0] != Convert.ToByte('A') && output[1] == 0x01);

            active = true;
        }

        public void Deactivate()
        {
            byte[] deactivationCommand = new byte[0x21];
            deactivationCommand[1] = Convert.ToByte('A');
            deactivationCommand[2] = 0x00;

            byte[] output;

            do
            {
                portalConnection.Write(deactivationCommand);

                output = portalConnection.Read();
            } while (output[0] != Convert.ToByte('A') && output[1] == 0x00);

            active = false;
        }

        public void SetColor(byte r, byte g, byte b)
        {
            byte[] colorCommand = new byte[0x21];
            colorCommand[1] = Convert.ToByte('C');
            colorCommand[2] = r;
            colorCommand[3] = g;
            colorCommand[4] = b;

            portalConnection.Write(colorCommand);
        }

        public void SetColorSided(PortalSide side, byte r, byte g, byte b, byte unknown1, byte unknown2)
        {
            if(side == PortalSide.BOTH)
            {
                SetColor(r, g, b);
                return;
            }

            byte[] sidedColorCommand = new byte[0x21];
            sidedColorCommand[1] = Convert.ToByte('J');
            sidedColorCommand[2] = (byte)side;
            sidedColorCommand[3] = r;
            sidedColorCommand[4] = g;
            sidedColorCommand[5] = b;
            sidedColorCommand[6] = unknown1;
            sidedColorCommand[7] = unknown2;

            portalConnection.Write(sidedColorCommand);
        }

        public void SetLightColor(byte brightness)
        {
            byte[] lightCommand = new byte[0x21];
            lightCommand[1] = Convert.ToByte('L');
            lightCommand[2] = (byte)PortalSide.BOTH;
            lightCommand[3] = brightness;

            portalConnection.Write(lightCommand);
        }

        public byte[] Query(byte characterIndex, byte block = 0x00)
        {
            byte[] queryCommand = new byte[0x21];

            queryCommand[1] = Convert.ToByte('Q');
            queryCommand[2] = characterIndex;
            queryCommand[3] = block;

            byte[] output;

            do
            {
                portalConnection.Write(queryCommand);
                output = portalConnection.Read();
            } while (output[0] != Convert.ToByte('Q') || (output[1] != characterIndex && output[1] != characterIndex + 0x10) || output[2] != block);

            return output;
        }

        public StatusResult GetStatus()
        {
            byte[] statusCommand = new byte[0x21];
            statusCommand[1] = Convert.ToByte('S');

            byte[] output;

            do
            {
                portalConnection.Write(statusCommand);
                output = portalConnection.Read();
            } while (output[0] != Convert.ToByte('S'));

            StatusResult result = new()
            {
                ready = output[6] == 0x01,
                count = output[5]
            };

            var bits = new BitArray(new int[] { output[1] });

            bool changeBitsSet = false;

            for (int i = 1; i < bits.Length; i += 2)
            {
                if (bits.Get(i))
                {
                    changeBitsSet = true;
                    break;
                }

            }

            if (!changeBitsSet)
            {
                for (int i = 0; i < bits.Length; i += 2)
                {
                    if (bits.Get(i))
                        result.characterIndexes.Add(Convert.ToByte(i / 2));

                }
            }
            else
            {
                result = GetStatus();
            }

            return result;
        }

        public void Close()
        {
            portalConnection.Close();
        }

        public void Music(bool activate)
        {
            byte[] musicCommand = new byte[0x21];
            musicCommand[1] = Convert.ToByte('M');
            musicCommand[2] = Convert.ToByte(activate ? 0x01 : 0x00);

            portalConnection.Write(musicCommand);
        }

        public void SendRawData(byte[] data)
        {
            portalConnection.WriteRaw(data);
        }
    }
}
