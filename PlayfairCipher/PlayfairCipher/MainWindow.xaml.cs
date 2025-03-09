using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using System.IO;
using System.Net.Sockets;
using System;
using System.Diagnostics;
using System.ComponentModel;
using static System.Net.Mime.MediaTypeNames;


namespace PlayfairCipherUI
{
    public partial class MainWindow : Window
    {
        // Import C++ DLL
        public const string CppFunctionsDLL = @"..\..\..\..\x64\Debug\PlayfairCipherC++.dll";

        [DllImport(CppFunctionsDLL, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void encode(string text, string keyword, char separator, IntPtr buffer, int bufferSize);  // Load the C++ encryption function from DLL

        [DllImport(CppFunctionsDLL, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void decode(string text, string keyword, char separator, IntPtr buffer, int bufferSize);  // Load the C++ decryption function from DLL

        // Import Asm DLL
        public const string AsmProcsDLL = @"..\..\..\..\x64\Debug\PlayfairCipherAsm.dll";

        [DllImport(AsmProcsDLL, CallingConvention = CallingConvention.Cdecl)]
        private static extern void encodeAsm(string text, string keyword, char separator, byte[] buffer);  // Load the assembly encryption procedure from DLL

        [DllImport(AsmProcsDLL, CallingConvention = CallingConvention.Cdecl)]
        private static extern void decodeAsm(string text, string keyword, char separator, byte[] buffer);  // Load the assembly decryption procedure from DLL


        private int numberOfRuns = 0;    // Log the number of encryption adn decryption done  

        public MainWindow()
        {
            InitializeComponent();
            initializeSeparatorComboBox();
        }

        // Initialize seprator combobox
        private void initializeSeparatorComboBox()
        {
            for (char ch = 'A'; ch <= 'Z'; ch++)
            {
                comboBoxSeparator.Items.Add(ch);
            }
            comboBoxSeparator.SelectedItem = 'X';
        }


        // Validate input data by the user
        private bool ValidateInputs()
        {
            // Validate textKey
            string keyText = textKey.Text;
            if (string.IsNullOrWhiteSpace(keyText))
            {
                MessageBox.Show("The keyword cannot be empty. Please enter a valid keyword.", "Input Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return false;
            }
            if (!keyText.All(char.IsLetter))
            {
                MessageBox.Show("The keyword must contain only alphabetic characters (A-Z).", "Input Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return false;
            }

            // Validate textInput
            string inputText = textInput.Text;
            if (string.IsNullOrWhiteSpace(inputText))
            {
                MessageBox.Show("The input text or file path cannot be empty. Please provide valid input.", "Input Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return false;
            }

            // If textInput is expected to reference a file
            if (File.Exists(inputText))
            {
                if (System.IO.Path.GetExtension(inputText).ToLower() != ".txt")
                {
                    MessageBox.Show("Only text files (.txt) are supported for file input.", "Input Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
            }
            else if (!File.Exists(inputText))
            {
                // Ensure it's not a file but rather direct input text
                if (inputText.Any(ch => !char.IsLetter(ch) && !char.IsWhiteSpace(ch)))
                {
                    MessageBox.Show("The input text can only contain alphabetic characters and spaces.", "Input Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }

                // Check if cipher mode is Decryption and input length is odd
                string? cipherMode = (comboBoxCipherMode.SelectedItem as ComboBoxItem)?.Content.ToString();
                if (cipherMode == "Decryption" && textInput.Text.Length % 2 != 0)
                {
                    MessageBox.Show("For decryption, the input length must be an even number.", "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }

            }

            return true;
        }

        private void encryptwithHighLevelLanguage(string inputFile, out string outputFile, string keyText, char separatorChar, IntPtr buffer, int BufferSize, IProgress<int> progress)
        {
            outputFile = "encoded.txt";

            // Get the total number of lines for progress calculation
            int totalLines = File.ReadLines(inputFile).Count();
            int processedLines = 0;

            // Open the output file for writing
            using (StreamWriter writer = new StreamWriter(outputFile, false)) // false to overwrite file
            {
                using (StreamReader reader = new StreamReader(inputFile))
                {
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        encode(line, keyText, separatorChar, buffer, BufferSize);
                        string result = Marshal.PtrToStringAnsi(buffer);

                        // Write the encoded line to the output file
                        writer.WriteLine(result);

                        // Report progress
                        processedLines++;
                        progress.Report((int)((double)processedLines / totalLines * 100));
                    }
                }
            }
        }

        private void decryptwithHighLevelLanguage(string inputFile, out string outputFile, string keyText, char separatorChar, IntPtr buffer, int BufferSize, IProgress<int> progress)
        {
            outputFile = "decoded.txt";

            // Get the total number of lines for progress calculation
            int totalLines = File.ReadLines(inputFile).Count();
            int processedLines = 0;

            // Open the output file for writing
            using (StreamWriter writer = new StreamWriter(outputFile, false)) // false to overwrite file
            {
                using (StreamReader reader = new StreamReader(inputFile))
                {
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        decode(line, keyText, separatorChar, buffer, BufferSize);
                        string result = Marshal.PtrToStringAnsi(buffer);

                        // Write the decoded line to the output file
                        writer.WriteLine(result);

                        // Report progress
                        processedLines++;
                        progress.Report((int)((double)processedLines / totalLines * 100));
                    }
                }
            }
        }

        private void encryptwithAssemblyLanguage(string inputFile, out string outputFile, string keyText, char separatorChar, IProgress<int> progress)
        {
            outputFile = "encodedByAssembly.txt";

            // Get the total number of lines for progress calculation
            int totalLines = File.ReadLines(inputFile).Count();
            int processedLines = 0;

            // Open the output file for writing
            using (StreamWriter writer = new StreamWriter(outputFile, false)) // false to overwrite file
            {
                using (StreamReader reader = new StreamReader(inputFile))
                {
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {

                        // Allocate byte array to receive cipher text
                        byte[] cipherBuffer = new byte[26];  

                        // Call assembly code to encode the line
                        encodeAsm(line, keyText, separatorChar, cipherBuffer);

                        // Find the actual length of meaningful data in cipherBuffer
                        int actualLength = Array.IndexOf(cipherBuffer, (byte)0);
                        if (actualLength == -1) actualLength = cipherBuffer.Length; // No null byte, use full length

                        // Convert only the meaningful part of the buffer to a string
                        string result = Encoding.ASCII.GetString(cipherBuffer, 0, actualLength).Trim();

                        //Write the encoded line to the output file
                        writer.WriteLine(result);

                        // Report progress
                        processedLines++;
                        progress.Report((int)((double)processedLines / totalLines * 100));
                    }
                }
            }
        }

        private void decryptwithAssemblyLanguage(string inputFile, out string outputFile, string keyText, char separatorChar, IProgress<int> progress)
        {
            outputFile = "decodedByAssembly.txt";

            // Get the total number of lines for progress calculation
            int totalLines = File.ReadLines(inputFile).Count();
            int processedLines = 0;

            // Open the output file for writing
            using (StreamWriter writer = new StreamWriter(outputFile, false)) // false to overwrite file
            {
                using (StreamReader reader = new StreamReader(inputFile))
                {
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        // Allocate byte array to receive cipher text
                        byte[] cipherBuffer = new byte[26];

                        // Call assembly code to encode the line
                        decodeAsm(line, keyText, separatorChar, cipherBuffer);

                        // Find the actual length of meaningful data in cipherBuffer
                        int actualLength = Array.IndexOf(cipherBuffer, (byte)0);
                        if (actualLength == -1) actualLength = cipherBuffer.Length; // No null byte, use full length

                        // Convert only the meaningful part of the buffer to a string
                        string result = Encoding.ASCII.GetString(cipherBuffer, 0, actualLength).Trim();

                        //Write the encoded line to the output file
                        writer.WriteLine(result);

                        // Report progress
                        processedLines++;
                        progress.Report((int)((double)processedLines / totalLines * 100));
                    }
                }
            }
        }

        private async void btnRun_Click(object sender, RoutedEventArgs e)
        {
            // Button for Run
            if (!File.Exists(CppFunctionsDLL))
            {
                MessageBox.Show("DLL not found: " + CppFunctionsDLL);
                return;
            }

            // Input and keyword valiation
            if (!ValidateInputs())
            {
                return; // Stop execution if validation fails
            }

            // Initialize and reset the progress bar
            progressBar.Value = 0;

            // Initialize buffer
            const int BufferSize = 1024;
            IntPtr buffer = Marshal.AllocHGlobal(BufferSize);

            // Initialize variable obtained from GUI
            string inputText = textInput.Text;
            string keyText = textKey.Text;
            string separator = comboBoxSeparator.Text;
            char separatorChar = separator[0];
            string? cipherMode = (comboBoxCipherMode.SelectedItem as ComboBoxItem)?.Content.ToString();
            string? language = (comboBoxLanguage.SelectedItem as ComboBoxItem)?.Content.ToString();
            string inputFile = inputText;
            string outputFile = "";
            Stopwatch stopwatch = new Stopwatch();

            // Ensure operation is properly choosen
            if (cipherMode == null || language == null)
            {
                MessageBox.Show("Please select options from both ComboBoxes.");
                return;
            }

            try
            {
                // Progress bar update
                var progress = new Progress<int>(percentage =>
                {
                    // Update the progress bar on the UI thread
                    progressBar.Value = percentage;
                    // Update the progress text on the UI thread
                    textProgress.Text = $"Progress: {percentage}%";
                });


                // Import file handling
                if (File.Exists(inputText) && System.IO.Path.GetExtension(inputText).ToLower() == ".txt") {

                    await Task.Run(() =>
                    {
                        stopwatch.Start(); // Start the stopwatch

                        if (language == "C++" && cipherMode == "Encryption")
                        {
                            encryptwithHighLevelLanguage(inputText, out outputFile, keyText, separatorChar, buffer, BufferSize, progress);
                        }
                        else if (language == "C++" && cipherMode == "Decryption")
                        {
                            decryptwithHighLevelLanguage(inputText, out outputFile, keyText, separatorChar, buffer, BufferSize, progress);
                        }
                        else if (language == "Assembly" && cipherMode == "Encryption")
                        {
                            encryptwithAssemblyLanguage(inputFile, out outputFile, keyText, separatorChar, progress);
                        }
                        else if (language == "Assembly" && cipherMode == "Decryption")
                        {
                            decryptwithAssemblyLanguage(inputText, out outputFile, keyText, separatorChar, progress);
                        }
                        else
                        {
                            MessageBox.Show("Invalid combination of language and cipher mode.");
                            return;
                        }

                        // Stop the stopwatch after the work is completed
                        stopwatch.Stop();
                        numberOfRuns++;

                        // Update UI on the main thread
                        Dispatcher.Invoke(() =>
                        {
                            textTime.Text = $"Time taken: {stopwatch.ElapsedMilliseconds / 1000.0:F3} seconds";
                            textOutput.Text = outputFile;
                            textRun.Text = $"Number of consecutive runs: {numberOfRuns.ToString()}";
                        });

                    });
                }
                else
                {
                    // Single word handling
                    stopwatch.Start();
                    string result;

                    // Handle single-word operations
                    if (language == "C++" && cipherMode == "Encryption")
                    {
                        encode(inputText, keyText, separatorChar, buffer, BufferSize);
                        result = Marshal.PtrToStringAnsi(buffer);
                    }
                    else if (language == "C++" && cipherMode == "Decryption")
                    {
                        decode(inputText, keyText, separatorChar, buffer, BufferSize);
                        result = Marshal.PtrToStringAnsi(buffer);
                    }
                    else if (language == "Assembly" && cipherMode == "Encryption")
                    {
                        // Allocate byte array to receive cipher text
                        byte[] cipherBuffer = new byte[26];

                        // Call assembly code to encode the line
                        encodeAsm(inputText, keyText, separatorChar, cipherBuffer);

                        // Find the actual length of meaningful data in cipherBuffer
                        int actualLength = Array.IndexOf(cipherBuffer, (byte)0);
                        if (actualLength == -1) actualLength = cipherBuffer.Length; // No null byte, use full length

                        // Convert only the meaningful part of the buffer to a string
                        result = Encoding.ASCII.GetString(cipherBuffer, 0, actualLength).Trim();
                    }
                    else if (language == "Assembly" && cipherMode == "Decryption")
                    {
                        // Allocate byte array to receive cipher text
                        byte[] cipherBuffer = new byte[26];

                        // Call assembly code to encode the line
                        decodeAsm(inputText, keyText, separatorChar, cipherBuffer);

                        // Find the actual length of meaningful data in cipherBuffer
                        int actualLength = Array.IndexOf(cipherBuffer, (byte)0);
                        if (actualLength == -1) actualLength = cipherBuffer.Length; // No null byte, use full length

                        // Convert only the meaningful part of the buffer to a string
                        result = Encoding.ASCII.GetString(cipherBuffer, 0, actualLength).Trim();
                    }
                    else
                    {
                        MessageBox.Show("Invalid combination of language and cipher mode.");
                        return;
                    }

                    stopwatch.Stop();
                    numberOfRuns++;

                    textTime.Text = $"Time taken: {stopwatch.ElapsedMilliseconds / 1000.0:F3} seconds";
                    textOutput.Text = result ?? "Error: Function failed or buffer was too small.";
                    outputFile = result ?? "Error: Function failed or buffer was too small.";
                    progressBar.Value = 100;
                    textProgress.Text = $"Progress: 100%";
                    textRun.Text = $"Number of consecutive runs: {numberOfRuns.ToString()}";                 
                }
                
                // Show a message box after completing the operation
                MessageBox.Show($"Operation completed successfully!\n" +
                                $"Cipher Mode: {cipherMode}\n" +
                                $"Language: {language}\n" +
                                $"Output File/Text: {outputFile}\n",
                                "Operation Complete",
                                MessageBoxButton.OK,
                                MessageBoxImage.Information);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);                            
            }
        }

        private void btnImport_Click(object sender, RoutedEventArgs e)
        {
            // Button for Import file
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Title = "Please choose the file to import";

            bool? success = fileDialog.ShowDialog();

            if (success == true)
            {
                string path = fileDialog.FileName;
                string fileName = fileDialog.SafeFileName;

                textInput.Text = fileName;
                //textInput.Text = path;
            }
            else
            {
                // Nothing picked
                MessageBox.Show("File is not selected.");
            }
        }

    }
}