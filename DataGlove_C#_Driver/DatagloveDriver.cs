using Leviathan.Math;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.Runtime.InteropServices;

namespace Leviathan.Util
{
    public class DataGlove : IDataGloveListener
    {
        private Mutex quat_mutex;
        private DataGloveSerialWorker worker;
        private SerialPort serialPort;
        
        public HandOrient handorient;

        public DataGlove(string comport, int baud)
        {
            quat_mutex = new Mutex();
            serialPort = new SerialPort(comport, baud);
            serialPort.Open();
            worker = new DataGloveSerialWorker(serialPort, this);
            worker.Start();
        }

        public void OnHandOrientReceived(HandOrient orient)
        {
            handorient = orient;
        }
    }

    public class DataGloveSerialWorker
    {
        private Thread worker;
        private bool shutdownrequested;

        private SerialPort serial;
        private IDataGloveListener listener;

        public DataGloveSerialWorker(SerialPort _serialport, IDataGloveListener lst)
        {
            this.serial = _serialport;
            this.listener = lst;
        }

        public void Start()
        {
            if(worker == null)
            {
                worker = new Thread(new ThreadStart(ThreadFunc));
            } else
            {
                if(worker.IsAlive)
                {
                    throw new Exception("Could not start DataGloveSerialWorker");
                }
            }
            shutdownrequested = false;
            worker.Start();
        }

        public void RequestShutdown()
        {
            shutdownrequested = true;
            worker.Join();
        }

        private void ThreadFunc()
        {
            if(!serial.IsOpen)
            {
                throw new Exception("Serial Port was not open");
            }

            string line = string.Empty;
            while (serial.IsOpen && !shutdownrequested)
            {
                if(serial.BytesToRead > 0)
                {
                    byte[] datagram_size = new byte[4];
                    datagram_size[0] = (byte)serial.ReadChar();
                    datagram_size[1] = (byte)serial.ReadChar();
                    datagram_size[2] = (byte)serial.ReadChar();
                    datagram_size[3] = (byte)serial.ReadChar();
                    int dt_size = BitConverter.ToInt32(datagram_size, 0);

                    byte[] datagram = new byte[dt_size];
                    for(int i = 0; i < dt_size; i++)
                    {
                        datagram[i] = (byte)serial.ReadChar();
                    }
                    GCHandle handle = GCHandle.Alloc(datagram, GCHandleType.Pinned);
                    HandOrient structure = (HandOrient)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(HandOrient));
                    handle.Free();
                    listener.OnHandOrientReceived(structure);
                }
            }
        }

    }

    public interface IDataGloveListener
    {
        void OnHandOrientReceived(HandOrient orient);
    }

    public struct FingerOrient
    {
        Quaternion Distal;
        Quaternion Intermediate;

        public static readonly FingerOrient Identity = new FingerOrient()
        {
            Distal = Quaternion.Identity,
            Intermediate = Quaternion.Identity,
        };
    }

    public struct HandOrient
    {
        public FingerOrient Thumb_Finger;
        public FingerOrient Index_Finger;
        public FingerOrient Middle_Finger;
        public FingerOrient Ring_Finger;
        public FingerOrient Pinky_Finger;

        public static readonly HandOrient Identity = new HandOrient()
        {
            Thumb_Finger = FingerOrient.Identity,
            Index_Finger = FingerOrient.Identity,
            Middle_Finger = FingerOrient.Identity,
            Ring_Finger = FingerOrient.Identity,
            Pinky_Finger = FingerOrient.Identity
        };

    }

    
}

