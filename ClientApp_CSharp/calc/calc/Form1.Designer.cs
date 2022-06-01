namespace calc
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.one = new System.Windows.Forms.Button();
            this.four = new System.Windows.Forms.Button();
            this.plus = new System.Windows.Forms.Button();
            this.minus = new System.Windows.Forms.Button();
            this.x = new System.Windows.Forms.Button();
            this.dot = new System.Windows.Forms.Button();
            this.equals = new System.Windows.Forms.Button();
            this.divide = new System.Windows.Forms.Button();
            this.lcdtext = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // one
            // 
            this.one.Location = new System.Drawing.Point(90, 135);
            this.one.Name = "one";
            this.one.Size = new System.Drawing.Size(64, 68);
            this.one.TabIndex = 0;
            this.one.Text = "1";
            this.one.UseVisualStyleBackColor = true;
            this.one.Click += new System.EventHandler(this.button1_Click);
            // 
            // four
            // 
            this.four.Location = new System.Drawing.Point(90, 231);
            this.four.Name = "four";
            this.four.Size = new System.Drawing.Size(64, 68);
            this.four.TabIndex = 3;
            this.four.Text = "0";
            this.four.UseVisualStyleBackColor = true;
            // 
            // plus
            // 
            this.plus.Location = new System.Drawing.Point(385, 135);
            this.plus.Name = "plus";
            this.plus.Size = new System.Drawing.Size(64, 68);
            this.plus.TabIndex = 10;
            this.plus.Text = "+";
            this.plus.UseVisualStyleBackColor = true;
            // 
            // minus
            // 
            this.minus.Location = new System.Drawing.Point(385, 231);
            this.minus.Name = "minus";
            this.minus.Size = new System.Drawing.Size(64, 68);
            this.minus.TabIndex = 11;
            this.minus.Text = "-";
            this.minus.UseVisualStyleBackColor = true;
            // 
            // x
            // 
            this.x.Location = new System.Drawing.Point(385, 329);
            this.x.Name = "x";
            this.x.Size = new System.Drawing.Size(64, 68);
            this.x.TabIndex = 12;
            this.x.Text = "x";
            this.x.UseVisualStyleBackColor = true;
            // 
            // dot
            // 
            this.dot.Location = new System.Drawing.Point(90, 329);
            this.dot.Name = "dot";
            this.dot.Size = new System.Drawing.Size(64, 68);
            this.dot.TabIndex = 13;
            this.dot.Text = ".";
            this.dot.UseVisualStyleBackColor = true;
            // 
            // equals
            // 
            this.equals.Location = new System.Drawing.Point(90, 423);
            this.equals.Name = "equals";
            this.equals.Size = new System.Drawing.Size(64, 68);
            this.equals.TabIndex = 14;
            this.equals.Text = "=";
            this.equals.UseVisualStyleBackColor = true;
            this.equals.Click += new System.EventHandler(this.equals_Click);
            // 
            // divide
            // 
            this.divide.Location = new System.Drawing.Point(385, 423);
            this.divide.Name = "divide";
            this.divide.Size = new System.Drawing.Size(64, 68);
            this.divide.TabIndex = 15;
            this.divide.Text = "/";
            this.divide.UseVisualStyleBackColor = true;
            // 
            // lcdtext
            // 
            this.lcdtext.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.lcdtext.Location = new System.Drawing.Point(90, 79);
            this.lcdtext.Name = "lcdtext";
            this.lcdtext.ReadOnly = true;
            this.lcdtext.Size = new System.Drawing.Size(357, 27);
            this.lcdtext.TabIndex = 16;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(612, 539);
            this.Controls.Add(this.lcdtext);
            this.Controls.Add(this.divide);
            this.Controls.Add(this.equals);
            this.Controls.Add(this.dot);
            this.Controls.Add(this.x);
            this.Controls.Add(this.minus);
            this.Controls.Add(this.plus);
            this.Controls.Add(this.four);
            this.Controls.Add(this.one);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Button one;
        private Button four;
        private Button plus;
        private Button minus;
        private Button x;
        private Button dot;
        private Button equals;
        private Button divide;
        private TextBox lcdtext;
    }
}