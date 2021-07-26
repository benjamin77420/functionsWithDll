using System;
using System.Windows;
using System.Windows.Media;
using Microsoft.Win32;
using System.Runtime.InteropServices;


namespace WPFInterviewTest
{

    public partial class MainWindow : Window
    {
        //importing the function that is locaited in the cpp Dll file.
        [DllImport("ScanDll.dll", EntryPoint = "scanFile", CallingConvention = CallingConvention.Cdecl)]
        public static extern int scanFile(string filePath, string firstChar, string secondChar);//without the use of a WinAPI

        //importing the function that is locaited in the cpp Dll file.
        [DllImport("ScanDll.dll", EntryPoint = "scanFile2", CallingConvention = CallingConvention.Cdecl)]
        public static extern int scanFile2(string filePath, string firstChar, string secondChar);//with the use of WinAPI

        //will store the file path to the wanted text file.
        static string filePath = null;

        public MainWindow()
        {
            InitializeComponent();
            loading.Source = new Uri(Environment.CurrentDirectory + @"\\loading.gif");//loading the source of the gif 
        }

        //open the file explorer that will 
        private void fileExplorerBtn_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "TextFile|*.txt|AllFiles |*.*";
            if(openFileDialog.ShowDialog() == true)//if a atext file was picked
            {
                filePath = openFileDialog.FileName;//assigned to the static variable
            }
        }

        //starts the animation, that will triger the computing function at its end 
        private void startBtn_Click(object sender, RoutedEventArgs e)
        {
            if (loading.Visibility == System.Windows.Visibility.Collapsed) {//the gif is collapsed,
               
                if(checkInput() == false)//one of the inputs is missing
                {
                    return;
                }

                loading.Visibility = System.Windows.Visibility.Visible;//make the gig visable for the user.
                startBtn.Background = Brushes.Gray;//set a intermediate color of gray for the button until the results
                loading.Position = new TimeSpan(0,0,0,0,2000);//setting the time position of the gif, to match the loading animation
                loading.Play();//play the gif
            }
            
        }


        public void computeResult(object sender, RoutedEventArgs e)
        {
            if(checkInput() == false)//in the time of loading one of the imputs was deleted
            {
                loading.Visibility = System.Windows.Visibility.Collapsed;//collapsed the currntly visable gif
                return;
            }

            //scanFile(filePath, firstCharText.Text, secondCharText.Text);
            int result = scanFile2(filePath, firstCharText.Text, secondCharText.Text);

            if (result > 0)//change the color of the button to green  
            {
                startBtn.Background = Brushes.Green;
            }
            else if (result <= 0)//change the color of the button to read
            {
                startBtn.Background = Brushes.Red;
            }

            loading.Visibility = System.Windows.Visibility.Collapsed;//enter the gif to a visability state of collapsed
        }

        public bool checkInput()
        {
            if (firstCharText.Text == string.Empty || secondCharText.Text == string.Empty || filePath == null)//one of the 3 inputs are missing.
            {
                MessageBox.Show("Please make sure that all the needed data was entered, please contact support otherwise");//message the user for the incomplete input that was given.
                return false;
            }

            return true;
        }
    }
}
