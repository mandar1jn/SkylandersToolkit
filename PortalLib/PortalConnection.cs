using PInvoke;
using System.Runtime.InteropServices;

namespace PortalLib
{
    public class PortalConnection
    {
        private readonly IntPtr devicePtr;
        private HidDevice device;

        public PortalConnection()
        {
            devicePtr = HidApi.hid_open(0x1430, 0x0150, null);
            if(devicePtr == IntPtr.Zero)
            {
                throw new Exception("No portal found");
            }

            device = Marshal.PtrToStructure<HidDevice>(devicePtr);
        }

        public void Write(byte[] input)
        {
            IntPtr inputPtr = Marshal.AllocHGlobal(input.Length);
            Marshal.Copy(input, 0, inputPtr, input.Length);
            Kernel32.DeviceIoControl(new Kernel32.SafeObjectHandle(device.device_handle, false), 721301, inputPtr, 0x21, IntPtr.Zero, 0, out _, new IntPtr());
            Marshal.FreeHGlobal(inputPtr);
        }

        public void WriteRaw(byte[] data)
        {
            HResult res = HidApi.hid_write(devicePtr, data, Convert.ToUInt32(data.Length));
            if(res.Failed)
            {
                Console.WriteLine(Marshal.PtrToStringAuto(HidApi.hid_error(devicePtr)));
            }
        }

        public byte[] Read()
        {
            byte[] output = new byte[0x20];
            HidApi.hid_read(devicePtr, output, 0x20);
            return output;
        }

        public void Close()
        {
            HidApi.hid_close(devicePtr);
        }
    }
}
