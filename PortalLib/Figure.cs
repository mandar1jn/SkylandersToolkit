﻿using Microsoft.VisualBasic;
using System.Linq;
using System.Text;

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

        public short ID
        {
            get
            {
                return GetShort(0x01, 0x00);
            }
        }

        public short VariantID
        {
            get
            {
                return GetShort(0x01, 0x0C);
            }
        }

        public byte BCC
        {
            get
            {
                return GetByte(0x0, 0x4);
            }
            set
            {
                SetByte(0x0, 0x4, value);
            }
        }

        public uint ToyCodeNumber1
        {
            get
            {
                return GetUInt(0x01, 0x04);
            }
        }

        public uint WebtoyNumber2
        {
            get
            {
                return GetUInt(0x01, 0x08);
            }
        }

        public ulong FullToyCodeNumber
        {
            get
            {
                return ((ulong)WebtoyNumber2 << 32) | ToyCodeNumber1;
            }
        }

        public string ToyCode
        {
            get
            {
                string lookupTable = "23456789BCDFGHJKLMNPQRSTVWXYZ";

                ulong cur = FullToyCodeNumber;
                byte[] values = new byte[10];

                for(int i = 0; i < values.Length;  i++)
                {
                    values[i] = (byte)(cur % 29);
                    cur /= 29;
                }

                StringBuilder sb = new(11);
                for(int i = 9; i >= 0; i--)
                {
                    if (i == 4)
                        sb.Append('-');
                    sb.Append(lookupTable[Convert.ToInt32(values[i])]);
                }

                return sb.ToString();

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

                byte[] blockData = new byte[0x10];
                Array.Copy(output, 3, blockData, 0, 16);

                // block 1 is sometimes a duplicate of block 0
                if (i == 1)
                {
                    if (blockData.SequenceEqual(data[0]))
                    {
                        i -= 2;
                        continue;
                    }
                }

                Array.Copy(blockData, 0, data[i], 0, 16);
            }
        }

        public byte GetByte(int block, int offset)
        {
            return data[block][offset];
        }
        public void SetByte(int block, int offset, byte value)
        {
            data[block][offset] = value;
        }
        public short GetShort(int block, int offset) { return (short)(data[block][offset] + (data[block][offset + 1] << 8)); }

        public uint GetUInt(int block, int offset)
        {
            uint value = 0;

            for (sbyte i = 3; i >= 0; i--)
            {
                value += (uint)data[block][offset + i] << (8 * i);
            }

            return value;
        }

        public void Dump(string filePath)
        {
            FileStream file = File.Create(filePath);
            for(int i = 0; i < data.Length; i++)
            {
                file.Write(data[i]);
            }
            file.Close();
        }
    }
}
