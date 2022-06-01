namespace calc
{
    public partial class Form1 : Form
    {


        public Form1()
        {
            InitializeComponent();
            //talan
            App.Initialize(this);

        }

        


        public TextBox LCDText
        {
            get { return this.lcdtext; }
        }

        delegate void SetLCDTextCallback(string s);

        public void SetLCDText(string s) //valamiert privatot irnak mindenhol
        {
            if(this.lcdtext.InvokeRequired)
            {
                SetLCDTextCallback d = new SetLCDTextCallback(SetLCDText);
                this.Invoke(d, new object[] { s });
            }
            else
            {
                this.lcdtext.Text = s;
            }
           
        }



        private void button1_Click(object sender, EventArgs e)
        {
            //
        }

        private void equals_Click(object sender, EventArgs e)
        {
            //lcdtext.Text = // lcdtext.Text + "idk";
        }
    }
}