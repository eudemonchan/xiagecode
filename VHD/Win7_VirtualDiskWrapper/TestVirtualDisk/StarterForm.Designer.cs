namespace TestVirtualDisk
{
    partial class StarterForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonVHD = new System.Windows.Forms.Button();
            this.buttonTools = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonVHD
            // 
            this.buttonVHD.Location = new System.Drawing.Point(12, 12);
            this.buttonVHD.Name = "buttonVHD";
            this.buttonVHD.Size = new System.Drawing.Size(75, 23);
            this.buttonVHD.TabIndex = 0;
            this.buttonVHD.Text = "Create VHD";
            this.buttonVHD.UseVisualStyleBackColor = true;
            this.buttonVHD.Click += new System.EventHandler(this.buttonVHD_Click);
            // 
            // buttonTools
            // 
            this.buttonTools.Location = new System.Drawing.Point(150, 12);
            this.buttonTools.Name = "buttonTools";
            this.buttonTools.Size = new System.Drawing.Size(75, 23);
            this.buttonTools.TabIndex = 1;
            this.buttonTools.Text = "VHD Tools";
            this.buttonTools.UseVisualStyleBackColor = true;
            this.buttonTools.Click += new System.EventHandler(this.buttonTools_Click);
            // 
            // StarterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(283, 54);
            this.Controls.Add(this.buttonTools);
            this.Controls.Add(this.buttonVHD);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "StarterForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "VHD Api Demo";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonVHD;
        private System.Windows.Forms.Button buttonTools;
    }
}