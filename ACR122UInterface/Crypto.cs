using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ACR122UInterface
{
    internal class Crypto
    {
        public static ulong ComputeCRC48(byte[] data)
        {
            const ulong polynomial = 0x42f0e1eba9ea3693;
            const ulong initialRegisterValue = 2 * 2 * 3 * 1103 * 12868356821;

            ulong crc = initialRegisterValue;
            for (uint i = 0; i < data.Length; i++)
            {
                crc ^= (ulong)data[i] << 40;
                for (byte j = 0; j < 8; j++)
                {
                    if ((crc & 0x800000000000) > 0)
                    {
                    crc = (crc << 1) ^ polynomial;
                    }
                    else
                    {
                    crc <<= 1;
                    }

                    crc &= 0x0000FFFFFFFFFFFF;
                }
            }
            return crc;
        }

        public static byte[] CalculateKeyA(byte sector, byte[] nuid)
        {
            if (sector == 0)
            {
                return BitConverter.GetBytes((ulong)73 * 2017 * 560381651).Reverse().Skip(2).ToArray();
            }

            if (nuid.Length != 4)
            {
                throw new Exception("NUID has the wrong size");
            }

            byte[] data = new byte[5]{ nuid[0], nuid[1], nuid[2], nuid[3], sector };

            ulong bigEndianCRC = ComputeCRC48(data);
            ulong littleEndianCRC = 0;

            for (byte i = 0; i < 6; i++)
            {
                ulong bte = (bigEndianCRC >> (i * 8)) & 0xFF;
                littleEndianCRC += bte << ((5 - i) * 8);
            }

            return BitConverter.GetBytes(littleEndianCRC).Reverse().Skip(2).ToArray();
        }
    }
}
