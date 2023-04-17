namespace PortalLib
{
    public enum FigureType : byte
    {
        SKYLANDER = 0x18,
        VEHICLE = 0x40,
        TRAP = 0x30,
        ULTIMATE_KAOS_TRAP = 0x35
    }

    public class Figure
    {
        byte[][] data = new byte[0x40][];

        public byte Area
        {
            get
            {
                return (byte)((GetArea1Sequence() < GetArea2Sequence()) ? 0x0 : 0x1);
            }
        }

        public short ID
        {
            get
            {
                return GetShort(0x01, 0x00);
            }
        }

        public byte VariantID
        {
            get
            {
                return GetByte(0x01, 0x0C);
            }
        }

        public FigureType Type
        {
            get
            {
                return (FigureType)GetByte(0x01, 0x0D);
            }
        }

        public Figure()
        {
            ClearData();
        }

        public void ClearData()
        {
            data = new byte[0x40][];

            for (int i = 0; i < data.Length; i++)
            {
                data[i] = new byte[0x10];
            }
        }

        public void ReadData(byte index)
        {
            ClearData();

            for(byte i = 0; i < 64; i++)
            {
                byte[] output =  Portal.Instance.Query(index, i);

                Array.Copy(output, 3, data[i], 0, 16);
            }
        }

        public byte GetByte(int block, int offset)
        {
            return data[block][offset];
        }
        public short GetShort(int block, int offset) { return (short)(data[block][offset] + (data[block][offset + 1] << 8)); }

        public byte GetArea1Sequence()
        {
            return GetByte(0x08, 0x09);
        }

        public byte GetArea2Sequence()
        {
            return GetByte(0x24, 0x09);
        }

        public byte GetAreaBlock()
        {
            return (byte)((Area == 0) ? 0x08 : 0x24);
        }

        public void LogData()
        {
            for (byte i = 0; i < data.Length; i++)
            {
                string dataString = "";
                for (int j = 0; j < data[i].Length; j++)
                {
                    dataString += $" 0x{data[i][j]:X2}";
                }
                Console.WriteLine($"Block 0x{i:X2}: {dataString}");
            }
        }
    }
}
