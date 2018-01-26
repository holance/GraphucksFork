using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace SeaShurpTest
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
            xamlButton.Click += xamlButton_Click;
            setSizeButton.Click += setSizeButton_Click;
        }

        void setSizeButton_Click(object sender, RoutedEventArgs e)
        {
            meTest.SetBackbufferSize(new Size(100, 100));
        }

        async void xamlButton_Click(object sender, RoutedEventArgs e)
        {
            FileOpenPicker getVidFile =  new FileOpenPicker();

            getVidFile.SuggestedStartLocation = PickerLocationId.VideosLibrary;
            getVidFile.ViewMode = PickerViewMode.Thumbnail;
                     
            getVidFile.FileTypeFilter.Clear();
            getVidFile.FileTypeFilter.Add(".mp4");
            getVidFile.FileTypeFilter.Add(".m4v");
            getVidFile.FileTypeFilter.Add(".mts");
            getVidFile.FileTypeFilter.Add(".mov");
            getVidFile.FileTypeFilter.Add(".wmv");
            getVidFile.FileTypeFilter.Add(".avi");
            getVidFile.FileTypeFilter.Add(".asf");

            var mediaFile = await getVidFile.PickSingleFileAsync();

            var stream = await mediaFile.OpenAsync(FileAccessMode.Read);
           
            meTest.SetSource(stream);
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.  The Parameter
        /// property is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }
    }
}



