using System.Security.Cryptography;
using System.Text;

namespace PortalLib
{
    public class Figure
    {
        static readonly byte[] HASH_CONST = {
                0x20, 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 0x28, 0x43, 0x29, 0x20, 0x32, // Copyright (C) 2
                0x30, 0x31, 0x30, 0x20, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x73, 0x69, 0x6F, 0x6E, 0x2E, 0x20, // 010 Activision.
                0x41, 0x6C, 0x6C, 0x20, 0x52, 0x69, 0x67, 0x68, 0x74, 0x73, 0x20, 0x52, 0x65, 0x73, 0x65, 0x72, // All Rights Reser
                0x76, 0x65, 0x64, 0x2E, 0x20};                                                                  // ved.
        byte[][] data = new byte[0x40][];
        byte[][] decryptedData = new byte[0x40][];

        #region manufacturer
        public uint NUID
        {
            get
            {
                return (uint)(GetByte(0x0, 0x0) << 24) + (uint)(GetByte(0x0, 0x1) << 16) + (uint)(GetByte(0x0, 0x2) << 8) + (uint)GetByte(0x0, 0x3);
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

        public byte SAK
        {
            get
            {
                return GetByte(0x0, 0x5);
            }
        }

        public short ATQA
        {
            get
            {
                return GetShort(0x0, 0x6);
            }
        }

        public string ProductionYear
        {
            get
            {
                return "20" + GetByte(0x0, 0xF).ToString("X");
            }
        }
        #endregion

        public short ID
        {
            get
            {
                return GetShort(0x01, 0x00);
            }
        }

        #region Toy code
        public uint ToyCodeNumber1
        {
            get
            {
                return GetUInt(0x01, 0x04);
            }
        }

        public uint ToyCodeNumber2
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
                return ((ulong)ToyCodeNumber2 << 32) | ToyCodeNumber1;
            }
        }

        public string ToyCode
        {
            get
            {
                string lookupTable = "23456789BCDFGHJKLMNPQRSTVWXYZ";

                ulong cur = FullToyCodeNumber;
                byte[] values = new byte[10];

                for (int i = 0; i < values.Length; i++)
                {
                    values[i] = (byte)(cur % 29);
                    cur /= 29;
                }

                StringBuilder sb = new(11);
                for (int i = 9; i >= 0; i--)
                {
                    if (i == 4)
                        sb.Append('-');
                    sb.Append(lookupTable[Convert.ToInt32(values[i])]);
                }

                return sb.ToString();

            }
        }
        #endregion

        public short VariantID
        {
            get
            {
                return GetShort(0x01, 0x0C);
            }
        }

        #region counters

        public byte CounterArea1
        {
            get
            {
                return GetByte(0x8, 0x9);
            }
            set
            {
                SetByte(0x8, 0x9, value);
            }
        }

        public byte CounterArea2
        {
            get
            {
                return GetByte(0x24, 0x9);
            }
            set
            {
                SetByte(0x24, 0x9, value);
            }
        }
        #endregion

        public string Nickname
        {
            get
            {
                byte firstSector = (byte)((CounterArea1 >= CounterArea2) ? 0xA : 0x26);
                string name = "";

                for (byte i = 0; i <= 15; i++)
                {

                    byte block = (byte)((i < 8) ? firstSector : firstSector + 2);

                    byte offset = (byte)((i < 8) ? i * 2 : (i - 8) * 2);

                    string character = Encoding.Unicode.GetString(BitConverter.GetBytes(GetShort(block, offset)));

                    name += character;

                    if(character == "\0")
                    {
                        break;
                    }
                }
                return (name != "\0")? name : "Not set";
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
                decryptedData[i] = new byte[0x10];
            }
        }

        #region data reading
        public void ReadData(byte index)
        {
            ClearData();

            for (byte i = 0; i < 64; i++)
            {
                byte[] output = Portal.Instance.Query(index, i);

                byte[] blockData = new byte[0x10];
                Array.Copy(output, 3, blockData, 0, 16);

                // block 1 is sometimes a duplicate of block 0
                if (i == 1)
                {
                    if (blockData.SequenceEqual(data[0]))
                    {
                        i -= 1;
                        continue;
                    }
                }

                Array.Copy(blockData, 0, data[i], 0, 16);

                if(((i + 1) % 4 == 0) || i < 8)
                {
                    Array.Copy(data[i], 0, decryptedData[i], 0, 16);
                }
                else
                {
                    // TODO: actually decrypt block
                    byte[] hashIn = new byte[0x56];
                    data[0].CopyTo(hashIn, 0);
                    data[1].CopyTo(hashIn, 0x10);
                    hashIn[0x20] = i;
                    HASH_CONST.CopyTo(hashIn, 0x21);

                    byte[] key = MD5.Create().ComputeHash(hashIn);

                    byte[] decrypted = new byte[0x10];

                    using (Aes aesAlg = Aes.Create())
                    {
                        aesAlg.Key = key;
                        aesAlg.Mode = CipherMode.ECB;
                        aesAlg.Padding = PaddingMode.Zeros;

                        ICryptoTransform decryptor = aesAlg.CreateDecryptor();

                        decrypted = decryptor.TransformFinalBlock(data[i], 0, data[i].Length);

                    }

                    Array.Copy(decrypted, 0, decryptedData[i], 0, 16);
                }
            }
        }

        public byte GetByte(int block, int offset)
        {
            return decryptedData[block][offset];
        }
        public void SetByte(int block, int offset, byte value)
        {
            decryptedData[block][offset] = value;
        }
        public short GetShort(int block, int offset) { return (short)(decryptedData[block][offset] + (decryptedData[block][offset + 1] << 8)); }

        public uint GetUInt(int block, int offset)
        {
            uint value = 0;

            for (sbyte i = 3; i >= 0; i--)
            {
                value += (uint)decryptedData[block][offset + i] << (8 * i);
            }

            return value;
        }
        #endregion

        public void Dump(bool decrypted, string filePath)
        {
            FileStream file = File.Create(filePath);
            if (!decrypted)
            {
                for (int i = 0; i < data.Length; i++)
                {
                    file.Write(data[i]);
                }
            }
            else
            {
                for (int i = 0; i < decryptedData.Length; i++)
                {
                    file.Write(decryptedData[i]);
                }
            }
            file.Close();
        }
    }
}
