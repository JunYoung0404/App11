// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::App11::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        flag = false;
        px = 100;
        py = 100;
        mySize = 16;
        IsChecked = true;
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

   
}

struct  winrt::Windows::UI::Color myCol = winrt::Microsoft::UI::Colors::Green();

void winrt::App11::implementation::MainWindow::Slider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e)
{
    mySize = e.NewValue();
}


void winrt::App11::implementation::MainWindow::ColorPicker_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    if (IsChecked)
    {
        IsChecked = FALSE;
        ColorPicker().Label(L"Disable");

        slider().Visibility(Visibility::Collapsed);
        colorPanel().Visibility(Visibility::Collapsed);
    }
    else
    {
        IsChecked = TRUE;
        ColorPicker().Label(L"Enable");

        slider().Visibility(Visibility::Visible);
        colorPanel().Visibility(Visibility::Visible);
    }
}


void winrt::App11::implementation::MainWindow::ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
{
    myCol = args.NewColor();
}


void winrt::App11::implementation::MainWindow::CanvasControl_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = true;
}


void winrt::App11::implementation::MainWindow::CanvasControl_PointerReleased(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = false;
    px = py = 0.0;
    vx.push_back(px);
    vy.push_back(py);
    col.push_back(myCol);
    size.push_back(mySize);
}


void winrt::App11::implementation::MainWindow::CanvasControl_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    px = e.GetCurrentPoint(canvas).Position().X;
    py = e.GetCurrentPoint(canvas).Position().Y;
    if (flag) {
        vx.push_back(px);
        vy.push_back(py);
        col.push_back(myCol);
        size.push_back(mySize);
        canvas.Invalidate();
    }
}


void winrt::App11::implementation::MainWindow::CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    int n = vx.size();
    if (n <= 2) return;

    for (int i = 1; i < n; i++) {
        if (vx[i] == 0.0 && vy[i] == 0.0) {
            i++; continue;
        }
        args.DrawingSession().DrawLine(vx[i - 1], vy[i - 1], vx[i], vy[i], col[i], size[i]);
        args.DrawingSession().FillCircle(vx[i - 1], vy[i - 1], size[i] / 2, col[i]);
        args.DrawingSession().FillCircle(vx[i], vy[i], size[i] / 2, col[i]);
    }
    canvas.Invalidate();
}


void winrt::App11::implementation::MainWindow::myWrite_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fw;
    errno_t err;
    err = fopen_s(&fw, "C:/Users/PC/source/repos/my.txt", "w");// +, ccs = UTF - 8");
    if (err == 0)
    {
        MessageBox(NULL, L"The file was opened\n", L"hi1", NULL);
        int num = vx.size();
        fprintf_s(fw, "%d\n", num);
        for (int i = 0; i < num; i++) {
            fprintf_s(fw, "%f %f %d %d %d %d %f\n", vx[i], vy[i],
                col[i].A, col[i].B, col[i].G, col[i].R, size[i]);
        }
        fclose(fw);
    }
    else  MessageBox(NULL, L"The file was not opened\n", L"hi2", NULL);
}


void winrt::App11::implementation::MainWindow::myRead_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fr;
    errno_t err;
    err = fopen_s(&fr, "C:/Users/PC/source/repos/my.txt", "r");// +, ccs = UTF - 8");
    if (err == 0)
    {
        //  MessageBox(NULL, L"The file was opened\n", L"hi1", NULL);
        vx.clear();
        vy.clear();
        col.clear();
        size.clear();

        int num;
        float vx1, vy1, size1;
        int colA, colB, colG, colR;

        fscanf_s(fr, "%d", &num);
        for (int i = 0; i < num; i++) {
            fscanf_s(fr, "%f %f %hhu %hhu %hhu %hhu %f ", &vx1, &vy1, &colA, &colB, &colG, &colR, &size1);
            vx.push_back(vx1);
            vy.push_back(vy1);
            myCol.A = colA;
            myCol.B = colB;
            myCol.G = colG;
            myCol.R = colR;
            col.push_back(myCol);
            size.push_back(size1);
        }
        fclose(fr);
        //MessageBox(NULL, L"The file closed\n", L"hi1", NULL);
        CanvasControl_PointerReleased(NULL, NULL);
    }
    else  MessageBox(NULL, L"The file was not opened\n", L"hi2", NULL);
}

void winrt::App11::implementation::MainWindow::myClear_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    vx.clear();
    vy.clear();
    col.clear();
    size.clear();
    flag = false;
    px = 100;
    py = 100;
    mySize = 16;
}


void winrt::App11::implementation::MainWindow::MenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    vx.clear();
    vy.clear();
    size.clear();
    col.clear();
}


void winrt::App11::implementation::MainWindow::MenuFlyoutItem_Click_1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/PC/source/repos/my.txt", "w");

    if (fp)
    {
        int n = vx.size();
        for (int i = 0; i < n; i++)
        {
            fprintf(fp, "%f %f %d %d %d %d %f ", vx[i], vy[i], col[i].R, col[i].G, col[i].B, col[i].A, size[i]);
        }
        fclose(fp);
    }
}


void winrt::App11::implementation::MainWindow::MenuFlyoutItem_Click_2(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/PC/source/repos/my.txt", "r");

    if (fp)
    {
        vx.clear(); vy.clear(); size.clear(); col.clear();

        float x, y, s;
        int r, g, b, a;
        while (EOF != fscanf_s(fp, "%f %f %d %d %d %d %f", &x, &y, &r, &g, &b, &a, &s))
        {
            vx.push_back(x);
            vy.push_back(y);
            Windows::UI::Color tCol;
            tCol.R = r; tCol.G = g; tCol.B = b; tCol.A = a;
            col.push_back(tCol);
            size.push_back(s);
        }
        fclose(fp);
    }
}


void winrt::App11::implementation::MainWindow::MenuFlyoutItem_Click_3(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    this->Close();
}



void winrt::App11::implementation::MainWindow::myText_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    // 표시할 텍스트
    winrt::hstring buttonText = L"안녕하세요, 버튼이 클릭되었습니다!";

    // TextBlock 객체
   auto textBlock = myTextBlock();

    // TextBlock 내용 업데이트
    textBlock.Text(buttonText);
}



