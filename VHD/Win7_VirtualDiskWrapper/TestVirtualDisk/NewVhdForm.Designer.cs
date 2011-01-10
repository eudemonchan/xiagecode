namespace TestVirtualDisk
{
    partial class NewVhdForm
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
            this.backgroundWorkerCreate = new System.ComponentModel.BackgroundWorker();
            this.progressBarCreateVhd = new System.Windows.Forms.ProgressBar();
            this.groupBoxCreate = new System.Windows.Forms.GroupBox();
            this.radioButtonNewVhdFixed = new System.Windows.Forms.RadioButton();
            this.radioButtonNewVhdDynamic = new System.Windows.Forms.RadioButton();
            this.labelSizeNewVhd2 = new System.Windows.Forms.Label();
            this.labelSizeNewVhd = new System.Windows.Forms.Label();
            this.numericUpDownSizeNewVhd = new System.Windows.Forms.NumericUpDown();
            this.checkBoxAsyncCreate = new System.Windows.Forms.CheckBox();
            this.buttonNewVhd = new System.Windows.Forms.Button();
            this.buttonSelectPathCreate = new System.Windows.Forms.Button();
            this.textBoxVHD = new System.Windows.Forms.TextBox();
            this.groupBoxCreate.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSizeNewVhd)).BeginInit();
            this.SuspendLayout();
            // 
            // backgroundWorkerCreate
            // 
            this.backgroundWorkerCreate.WorkerReportsProgress = true;
            this.backgroundWorkerCreate.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            this.backgroundWorkerCreate.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorker1_RunWorkerCompleted);
            this.backgroundWorkerCreate.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker1_ProgressChanged);
            // 
            // progressBarCreateVhd
            // 
            this.progressBarCreateVhd.Location = new System.Drawing.Point(6, 124);
            this.progressBarCreateVhd.Name = "progressBarCreateVhd";
            this.progressBarCreateVhd.Size = new System.Drawing.Size(408, 23);
            this.progressBarCreateVhd.TabIndex = 1;
            // 
            // groupBoxCreate
            // 
            this.groupBoxCreate.Controls.Add(this.radioButtonNewVhdFixed);
            this.groupBoxCreate.Controls.Add(this.radioButtonNewVhdDynamic);
            this.groupBoxCreate.Controls.Add(this.progressBarCreateVhd);
            this.groupBoxCreate.Controls.Add(this.labelSizeNewVhd2);
            this.groupBoxCreate.Controls.Add(this.labelSizeNewVhd);
            this.groupBoxCreate.Controls.Add(this.numericUpDownSizeNewVhd);
            this.groupBoxCreate.Controls.Add(this.checkBoxAsyncCreate);
            this.groupBoxCreate.Controls.Add(this.buttonNewVhd);
            this.groupBoxCreate.Controls.Add(this.buttonSelectPathCreate);
            this.groupBoxCreate.Controls.Add(this.textBoxVHD);
            this.groupBoxCreate.Location = new System.Drawing.Point(13, 13);
            this.groupBoxCreate.Name = "groupBoxCreate";
            this.groupBoxCreate.Size = new System.Drawing.Size(422, 165);
            this.groupBoxCreate.TabIndex = 3;
            this.groupBoxCreate.TabStop = false;
            this.groupBoxCreate.Text = "Create VHD";
            // 
            // radioButtonNewVhdFixed
            // 
            this.radioButtonNewVhdFixed.AutoSize = true;
            this.radioButtonNewVhdFixed.Checked = true;
            this.radioButtonNewVhdFixed.Location = new System.Drawing.Point(9, 97);
            this.radioButtonNewVhdFixed.Name = "radioButtonNewVhdFixed";
            this.radioButtonNewVhdFixed.Size = new System.Drawing.Size(71, 17);
            this.radioButtonNewVhdFixed.TabIndex = 8;
            this.radioButtonNewVhdFixed.TabStop = true;
            this.radioButtonNewVhdFixed.Text = "Fixed size";
            this.radioButtonNewVhdFixed.UseVisualStyleBackColor = true;
            // 
            // radioButtonNewVhdDynamic
            // 
            this.radioButtonNewVhdDynamic.AutoSize = true;
            this.radioButtonNewVhdDynamic.Location = new System.Drawing.Point(9, 74);
            this.radioButtonNewVhdDynamic.Name = "radioButtonNewVhdDynamic";
            this.radioButtonNewVhdDynamic.Size = new System.Drawing.Size(133, 17);
            this.radioButtonNewVhdDynamic.TabIndex = 7;
            this.radioButtonNewVhdDynamic.Text = "Dynamically expending";
            this.radioButtonNewVhdDynamic.UseVisualStyleBackColor = true;
            // 
            // labelSizeNewVhd2
            // 
            this.labelSizeNewVhd2.AutoSize = true;
            this.labelSizeNewVhd2.Location = new System.Drawing.Point(106, 48);
            this.labelSizeNewVhd2.Name = "labelSizeNewVhd2";
            this.labelSizeNewVhd2.Size = new System.Drawing.Size(22, 13);
            this.labelSizeNewVhd2.TabIndex = 6;
            this.labelSizeNewVhd2.Text = "Mo";
            // 
            // labelSizeNewVhd
            // 
            this.labelSizeNewVhd.AutoSize = true;
            this.labelSizeNewVhd.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelSizeNewVhd.Location = new System.Drawing.Point(6, 48);
            this.labelSizeNewVhd.Name = "labelSizeNewVhd";
            this.labelSizeNewVhd.Size = new System.Drawing.Size(30, 13);
            this.labelSizeNewVhd.TabIndex = 5;
            this.labelSizeNewVhd.Text = "Size:";
            // 
            // numericUpDownSizeNewVhd
            // 
            this.numericUpDownSizeNewVhd.Location = new System.Drawing.Point(42, 45);
            this.numericUpDownSizeNewVhd.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.numericUpDownSizeNewVhd.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownSizeNewVhd.Name = "numericUpDownSizeNewVhd";
            this.numericUpDownSizeNewVhd.Size = new System.Drawing.Size(58, 20);
            this.numericUpDownSizeNewVhd.TabIndex = 4;
            this.numericUpDownSizeNewVhd.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // checkBoxAsyncCreate
            // 
            this.checkBoxAsyncCreate.AutoSize = true;
            this.checkBoxAsyncCreate.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.checkBoxAsyncCreate.Checked = true;
            this.checkBoxAsyncCreate.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAsyncCreate.Location = new System.Drawing.Point(259, 98);
            this.checkBoxAsyncCreate.Name = "checkBoxAsyncCreate";
            this.checkBoxAsyncCreate.Size = new System.Drawing.Size(155, 17);
            this.checkBoxAsyncCreate.TabIndex = 3;
            this.checkBoxAsyncCreate.Text = "Create in asynchrone mode";
            this.checkBoxAsyncCreate.UseVisualStyleBackColor = true;
            // 
            // buttonNewVhd
            // 
            this.buttonNewVhd.Location = new System.Drawing.Point(339, 74);
            this.buttonNewVhd.Name = "buttonNewVhd";
            this.buttonNewVhd.Size = new System.Drawing.Size(75, 23);
            this.buttonNewVhd.TabIndex = 2;
            this.buttonNewVhd.Text = "Create !!!";
            this.buttonNewVhd.UseVisualStyleBackColor = true;
            this.buttonNewVhd.Click += new System.EventHandler(this.buttonNewVhd_Click);
            // 
            // buttonSelectPathCreate
            // 
            this.buttonSelectPathCreate.Location = new System.Drawing.Point(339, 19);
            this.buttonSelectPathCreate.Name = "buttonSelectPathCreate";
            this.buttonSelectPathCreate.Size = new System.Drawing.Size(75, 23);
            this.buttonSelectPathCreate.TabIndex = 1;
            this.buttonSelectPathCreate.Text = "Select path";
            this.buttonSelectPathCreate.UseVisualStyleBackColor = true;
            this.buttonSelectPathCreate.Click += new System.EventHandler(this.buttonSelectPathCreate_Click);
            // 
            // textBoxVHD
            // 
            this.textBoxVHD.BackColor = System.Drawing.SystemColors.Window;
            this.textBoxVHD.Location = new System.Drawing.Point(6, 19);
            this.textBoxVHD.Name = "textBoxVHD";
            this.textBoxVHD.ReadOnly = true;
            this.textBoxVHD.Size = new System.Drawing.Size(327, 20);
            this.textBoxVHD.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(448, 188);
            this.Controls.Add(this.groupBoxCreate);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create new VHD";
            this.groupBoxCreate.ResumeLayout(false);
            this.groupBoxCreate.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSizeNewVhd)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.ComponentModel.BackgroundWorker backgroundWorkerCreate;
        private System.Windows.Forms.ProgressBar progressBarCreateVhd;
        private System.Windows.Forms.GroupBox groupBoxCreate;
        private System.Windows.Forms.TextBox textBoxVHD;
        private System.Windows.Forms.Button buttonSelectPathCreate;
        private System.Windows.Forms.CheckBox checkBoxAsyncCreate;
        private System.Windows.Forms.Button buttonNewVhd;
        private System.Windows.Forms.Label labelSizeNewVhd2;
        private System.Windows.Forms.Label labelSizeNewVhd;
        private System.Windows.Forms.NumericUpDown numericUpDownSizeNewVhd;
        private System.Windows.Forms.RadioButton radioButtonNewVhdFixed;
        private System.Windows.Forms.RadioButton radioButtonNewVhdDynamic;
    }
}

