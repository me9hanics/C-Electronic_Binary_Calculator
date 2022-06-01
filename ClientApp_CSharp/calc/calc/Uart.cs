using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace calc
{
    internal class Uart
    {
        public static SerialPort com3;
        public Uart()
        {
            com3 = new SerialPort("COM3");
            com3.BaudRate = 115200;
            com3.Parity = Parity.None;
            com3.StopBits = StopBits.One;
            com3.DataBits = 8;
            com3.Handshake = Handshake.None;
            com3.Open();
            Trace.WriteLine("Uart Opened");
            com3.DataReceived += new SerialDataReceivedEventHandler(Com3_DataReceived);
        }

        private void Com3_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string yet = com3.ReadLine();
            //App.Instance.MainForm.SetLCDText(com3.ReadLine());
            App.Instance.MainForm.SetLCDText(yet);
            
            
        }

        public void EndComm (object sender, FormClosingEventArgs e)
        {
            //TODO
            com3.Close();
        }
    }
}
