namespace TestVirtualDisk
{
    partial class ToolsVhdForm
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
            this.labelFile = new System.Windows.Forms.Label();
            this.textBoxFile = new System.Windows.Forms.TextBox();
            this.buttonCompact = new System.Windows.Forms.Button();
            this.buttonExpand = new System.Windows.Forms.Button();
            this.checkBoxAsyncCreate = new System.Windows.Forms.CheckBox();
            this.progressBarChangeVhd = new System.Windows.Forms.ProgressBar();
            this.buttonDetach = new System.Windows.Forms.Button();
            this.labelSizeNewVhd2 = new System.Windows.Forms.Label();
            this.labelSizeNewVhd = new System.Windows.Forms.Label();
            this.numericUpDownExpand = new System.Windows.Forms.NumericUpDown();
            this.buttonAttach = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.checkBoxNoLocalHost = new System.Windows.Forms.CheckBox();
            this.checkBoxPermLifetime = new System.Windows.Forms.CheckBox();
            this.checkBoxNoDriveLetter = new System.Windows.Forms.CheckBox();
            this.checkBoxReadOnly = new System.Windows.Forms.CheckBox();
            this.groupBoxInfo = new System.Windows.Forms.GroupBox();
            this.buttonGetInfo = new System.Windows.Forms.Button();
            this.comboBoxInfos = new System.Windows.Forms.ComboBox();
            this.backgroundWorkerCompact = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorkerExpand = new System.ComponentModel.BackgroundWorker();
            this.buttonOpen = new System.Windows.Forms.Button();
            this.labelInfos = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownExpand)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBoxInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelFile
            // 
            this.labelFile.AutoSize = true;
            this.labelFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelFile.Location = new System.Drawing.Point(13, 13);
            this.labelFile.Name = "labelFile";
            this.labelFile.Size = new System.Drawing.Size(26, 13);
            this.labelFile.TabIndex = 0;
            this.labelFile.Text = "File:";
            // 
            // textBoxFile
            // 
            this.textBoxFile.BackColor = System.Drawing.SystemColors.Window;
            this.textBoxFile.Location = new System.Drawing.Point(16, 30);
            this.textBoxFile.Name = "textBoxFile";
            this.textBoxFile.ReadOnly = true;
            this.textBoxFile.Size = new System.Drawing.Size(351, 20);
            this.textBoxFile.TabIndex = 1;
            // 
            // buttonCompact
            // 
            this.buttonCompact.Location = new System.Drawing.Point(16, 19);
            this.buttonCompact.Name = "buttonCompact";
            this.buttonCompact.Size = new System.Drawing.Size(102, 23);
            this.buttonCompact.TabIndex = 3;
            this.buttonCompact.Text = "Compact";
            this.buttonCompact.UseVisualStyleBackColor = true;
            this.buttonCompact.Click += new System.EventHandler(this.buttonCompact_Click);
            // 
            // buttonExpand
            // 
            this.buttonExpand.Location = new System.Drawing.Point(202, 19);
            this.buttonExpand.Name = "buttonExpand";
            this.buttonExpand.Size = new System.Drawing.Size(102, 23);
            this.buttonExpand.TabIndex = 4;
            this.buttonExpand.Text = "Expand";
            this.buttonExpand.UseVisualStyleBackColor = true;
            this.buttonExpand.Click += new System.EventHandler(this.buttonExpand_Click);
            // 
            // checkBoxAsyncCreate
            // 
            this.checkBoxAsyncCreate.AutoSize = true;
            this.checkBoxAsyncCreate.Checked = true;
            this.checkBoxAsyncCreate.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAsyncCreate.Location = new System.Drawing.Point(16, 76);
            this.checkBoxAsyncCreate.Name = "checkBoxAsyncCreate";
            this.checkBoxAsyncCreate.Size = new System.Drawing.Size(132, 17);
            this.checkBoxAsyncCreate.TabIndex = 5;
            this.checkBoxAsyncCreate.Text = "Use asynchrone mode";
            this.checkBoxAsyncCreate.UseVisualStyleBackColor = true;
            // 
            // progressBarChangeVhd
            // 
            this.progressBarChangeVhd.Location = new System.Drawing.Point(16, 99);
            this.progressBarChangeVhd.Name = "progressBarChangeVhd";
            this.progressBarChangeVhd.Size = new System.Drawing.Size(351, 23);
            this.progressBarChangeVhd.TabIndex = 6;
            // 
            // buttonDetach
            // 
            this.buttonDetach.Location = new System.Drawing.Point(16, 71);
            this.buttonDetach.Name = "buttonDetach";
            this.buttonDetach.Size = new System.Drawing.Size(102, 23);
            this.buttonDetach.TabIndex = 7;
            this.buttonDetach.Text = "Detach";
            this.buttonDetach.UseVisualStyleBackColor = true;
            this.buttonDetach.Click += new System.EventHandler(this.buttonDetach_Click);
            // 
            // labelSizeNewVhd2
            // 
            this.labelSizeNewVhd2.AutoSize = true;
            this.labelSizeNewVhd2.Location = new System.Drawing.Point(329, 48);
            this.labelSizeNewVhd2.Name = "labelSizeNewVhd2";
            this.labelSizeNewVhd2.Size = new System.Drawing.Size(22, 13);
            this.labelSizeNewVhd2.TabIndex = 11;
            this.labelSizeNewVhd2.Text = "Mo";
            // 
            // labelSizeNewVhd
            // 
            this.labelSizeNewVhd.AutoSize = true;
            this.labelSizeNewVhd.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelSizeNewVhd.Location = new System.Drawing.Point(209, 48);
            this.labelSizeNewVhd.Name = "labelSizeNewVhd";
            this.labelSizeNewVhd.Size = new System.Drawing.Size(55, 13);
            this.labelSizeNewVhd.TabIndex = 10;
            this.labelSizeNewVhd.Text = "New Size:";
            // 
            // numericUpDownExpand
            // 
            this.numericUpDownExpand.Location = new System.Drawing.Point(265, 45);
            this.numericUpDownExpand.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.numericUpDownExpand.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownExpand.Name = "numericUpDownExpand";
            this.numericUpDownExpand.Size = new System.Drawing.Size(58, 20);
            this.numericUpDownExpand.TabIndex = 9;
            this.numericUpDownExpand.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // buttonAttach
            // 
            this.buttonAttach.Location = new System.Drawing.Point(16, 19);
            this.buttonAttach.Name = "buttonAttach";
            this.buttonAttach.Size = new System.Drawing.Size(102, 23);
            this.buttonAttach.TabIndex = 12;
            this.buttonAttach.Text = "Attach";
            this.buttonAttach.UseVisualStyleBackColor = true;
            this.buttonAttach.Click += new System.EventHandler(this.buttonAttach_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonCompact);
            this.groupBox1.Controls.Add(this.buttonExpand);
            this.groupBox1.Controls.Add(this.labelSizeNewVhd2);
            this.groupBox1.Controls.Add(this.progressBarChangeVhd);
            this.groupBox1.Controls.Add(this.numericUpDownExpand);
            this.groupBox1.Controls.Add(this.checkBoxAsyncCreate);
            this.groupBox1.Controls.Add(this.labelSizeNewVhd);
            this.groupBox1.Location = new System.Drawing.Point(16, 63);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(380, 132);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.checkBoxNoLocalHost);
            this.groupBox2.Controls.Add(this.checkBoxPermLifetime);
            this.groupBox2.Controls.Add(this.checkBoxNoDriveLetter);
            this.groupBox2.Controls.Add(this.checkBoxReadOnly);
            this.groupBox2.Controls.Add(this.buttonAttach);
            this.groupBox2.Controls.Add(this.buttonDetach);
            this.groupBox2.Location = new System.Drawing.Point(16, 202);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(380, 110);
            this.groupBox2.TabIndex = 14;
            this.groupBox2.TabStop = false;
            // 
            // checkBoxNoLocalHost
            // 
            this.checkBoxNoLocalHost.AutoSize = true;
            this.checkBoxNoLocalHost.Location = new System.Drawing.Point(259, 42);
            this.checkBoxNoLocalHost.Name = "checkBoxNoLocalHost";
            this.checkBoxNoLocalHost.Size = new System.Drawing.Size(88, 17);
            this.checkBoxNoLocalHost.TabIndex = 16;
            this.checkBoxNoLocalHost.Text = "No local host";
            this.checkBoxNoLocalHost.UseVisualStyleBackColor = true;
            // 
            // checkBoxPermLifetime
            // 
            this.checkBoxPermLifetime.AutoSize = true;
            this.checkBoxPermLifetime.Location = new System.Drawing.Point(142, 42);
            this.checkBoxPermLifetime.Name = "checkBoxPermLifetime";
            this.checkBoxPermLifetime.Size = new System.Drawing.Size(112, 17);
            this.checkBoxPermLifetime.TabIndex = 15;
            this.checkBoxPermLifetime.Text = "Permanent lifetime";
            this.checkBoxPermLifetime.UseVisualStyleBackColor = true;
            // 
            // checkBoxNoDriveLetter
            // 
            this.checkBoxNoDriveLetter.AutoSize = true;
            this.checkBoxNoDriveLetter.Location = new System.Drawing.Point(224, 19);
            this.checkBoxNoDriveLetter.Name = "checkBoxNoDriveLetter";
            this.checkBoxNoDriveLetter.Size = new System.Drawing.Size(92, 17);
            this.checkBoxNoDriveLetter.TabIndex = 14;
            this.checkBoxNoDriveLetter.Text = "No drive letter";
            this.checkBoxNoDriveLetter.UseVisualStyleBackColor = true;
            // 
            // checkBoxReadOnly
            // 
            this.checkBoxReadOnly.AutoSize = true;
            this.checkBoxReadOnly.Location = new System.Drawing.Point(142, 19);
            this.checkBoxReadOnly.Name = "checkBoxReadOnly";
            this.checkBoxReadOnly.Size = new System.Drawing.Size(76, 17);
            this.checkBoxReadOnly.TabIndex = 13;
            this.checkBoxReadOnly.Text = "Read-Only";
            this.checkBoxReadOnly.UseVisualStyleBackColor = true;
            // 
            // groupBoxInfo
            // 
            this.groupBoxInfo.Controls.Add(this.labelInfos);
            this.groupBoxInfo.Controls.Add(this.buttonGetInfo);
            this.groupBoxInfo.Controls.Add(this.comboBoxInfos);
            this.groupBoxInfo.Location = new System.Drawing.Point(16, 319);
            this.groupBoxInfo.Name = "groupBoxInfo";
            this.groupBoxInfo.Size = new System.Drawing.Size(380, 174);
            this.groupBoxInfo.TabIndex = 15;
            this.groupBoxInfo.TabStop = false;
            this.groupBoxInfo.Text = "Infos";
            // 
            // buttonGetInfo
            // 
            this.buttonGetInfo.Location = new System.Drawing.Point(245, 20);
            this.buttonGetInfo.Name = "buttonGetInfo";
            this.buttonGetInfo.Size = new System.Drawing.Size(102, 23);
            this.buttonGetInfo.TabIndex = 5;
            this.buttonGetInfo.Text = "Get";
            this.buttonGetInfo.UseVisualStyleBackColor = true;
            this.buttonGetInfo.Click += new System.EventHandler(this.buttonGetInfo_Click);
            // 
            // comboBoxInfos
            // 
            this.comboBoxInfos.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxInfos.FormattingEnabled = true;
            this.comboBoxInfos.Location = new System.Drawing.Point(7, 20);
            this.comboBoxInfos.Name = "comboBoxInfos";
            this.comboBoxInfos.Size = new System.Drawing.Size(211, 21);
            this.comboBoxInfos.TabIndex = 0;
            // 
            // backgroundWorkerCompact
            // 
            this.backgroundWorkerCompact.WorkerReportsProgress = true;
            this.backgroundWorkerCompact.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorkerCompact_DoWork);
            this.backgroundWorkerCompact.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorkerCompact_RunWorkerCompleted);
            this.backgroundWorkerCompact.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorkerCompact_ProgressChanged);
            // 
            // backgroundWorkerExpand
            // 
            this.backgroundWorkerExpand.WorkerReportsProgress = true;
            this.backgroundWorkerExpand.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorkerExpand_DoWork);
            this.backgroundWorkerExpand.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorkerExpand_RunWorkerCompleted);
            this.backgroundWorkerExpand.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorkerExpand_ProgressChanged);
            // 
            // buttonOpen
            // 
            this.buttonOpen.Location = new System.Drawing.Point(374, 30);
            this.buttonOpen.Name = "buttonOpen";
            this.buttonOpen.Size = new System.Drawing.Size(22, 23);
            this.buttonOpen.TabIndex = 16;
            this.buttonOpen.Text = "...";
            this.buttonOpen.UseVisualStyleBackColor = true;
            this.buttonOpen.Click += new System.EventHandler(this.buttonOpen_Click);
            // 
            // labelInfos
            // 
            this.labelInfos.Location = new System.Drawing.Point(7, 48);
            this.labelInfos.Name = "labelInfos";
            this.labelInfos.Size = new System.Drawing.Size(360, 123);
            this.labelInfos.TabIndex = 6;
            // 
            // ToolsVhdForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(408, 505);
            this.Controls.Add(this.buttonOpen);
            this.Controls.Add(this.groupBoxInfo);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBoxFile);
            this.Controls.Add(this.labelFile);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ToolsVhdForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "VHD Tools";
            this.Load += new System.EventHandler(this.ToolsCreatedVhdForm_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ToolsCreatedVhdForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownExpand)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBoxInfo.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelFile;
        private System.Windows.Forms.TextBox textBoxFile;
        private System.Windows.Forms.Button buttonCompact;
        private System.Windows.Forms.Button buttonExpand;
        private System.Windows.Forms.CheckBox checkBoxAsyncCreate;
        private System.Windows.Forms.ProgressBar progressBarChangeVhd;
        private System.Windows.Forms.Button buttonDetach;
        private System.Windows.Forms.Label labelSizeNewVhd2;
        private System.Windows.Forms.Label labelSizeNewVhd;
        private System.Windows.Forms.NumericUpDown numericUpDownExpand;
        private System.Windows.Forms.Button buttonAttach;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox checkBoxNoDriveLetter;
        private System.Windows.Forms.CheckBox checkBoxReadOnly;
        private System.Windows.Forms.CheckBox checkBoxNoLocalHost;
        private System.Windows.Forms.CheckBox checkBoxPermLifetime;
        private System.Windows.Forms.GroupBox groupBoxInfo;
        private System.Windows.Forms.ComboBox comboBoxInfos;
        private System.Windows.Forms.Button buttonGetInfo;
        private System.ComponentModel.BackgroundWorker backgroundWorkerCompact;
        private System.ComponentModel.BackgroundWorker backgroundWorkerExpand;
        internal System.Windows.Forms.Button buttonOpen;
        private System.Windows.Forms.Label labelInfos;
    }
}