using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Diagnostics;

namespace calc
{
    public class App
    {
        private static App thisApp;

        private Uart uartCom;

        private Form1 mainForm;

        public static App Instance
        {
            get { return thisApp; }
        }
        
        public Form1 MainForm
        {
            get { return mainForm; }
        }


        //private void 

        public static void Initialize(Form1 form)
        {
            Trace.WriteLine("init");
            thisApp =  new App();
            thisApp.mainForm = form;
            thisApp.uartCom = new Uart();
            thisApp.mainForm.FormClosing += new FormClosingEventHandler(thisApp.uartCom.EndComm);
        }
    }
}
