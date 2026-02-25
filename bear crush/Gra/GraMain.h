/***************************************************************
 * Name:      GraMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2024-12-19
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef GRAMAIN_H
#define GRAMAIN_H
#include <map>
#include <chrono>

//(*Headers(GraDialog)
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>
//*)


class GraDialog: public wxDialog
{
    public:

        int dodatkowy_czas;
        int dodatkowy_wynik;
        int wynik;
        int wielkosc;

        GraDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~GraDialog();

    private:
        int czas_poczatkowy;

        //(*Handlers(GraDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnBitmapButton1Click(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnRadioButton3Select(wxCommandEvent& event);
        void OnRadioButton2Select(wxCommandEvent& event);
        void OnRadioButton1Select(wxCommandEvent& event);
        //*)

        //(*Identifiers(GraDialog)
        static const long ID_BITMAPBUTTON1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_RADIOBUTTON1;
        static const long ID_RADIOBUTTON2;
        static const long ID_RADIOBUTTON3;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(GraDialog)
        wxBitmapButton* BitmapButton1;
        wxButton* Button1;
        wxButton* Button2;
        wxRadioButton* RadioButton1;
        wxRadioButton* RadioButton2;
        wxRadioButton* RadioButton3;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxTimer Timer1;
        //*)

        DECLARE_EVENT_TABLE()

        wxBitmapButton* pola[169];
        wxVector<wxVector<wxString>> kolorki2d;
        void AktualizujKolory();
        std::map<int,int> id2nr;

        //zmienne pomocnicze do zmiany kolorów kafelków przy klikaniu
        int r = 69;
        int c = 69;
        wxString x;

        //To jest do czasu
        std::chrono::time_point<std::chrono::high_resolution_clock>time_point;

};

#endif // GRAMAIN_H
