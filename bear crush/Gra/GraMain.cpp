/***************************************************************
 * Name:      GraMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2024-12-19
 * Copyright:  ()
 * License:
 **************************************************************/

#include "GraMain.h"
#include <wx/msgdlg.h>
#include <ctime>
#include <cstdlib>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <thread>
#include <wx/app.h>


using namespace std;
#undef _
#define _(s) wxString::FromUTF8(s)

//Funkcja zwracająca ścieżkę do pliku
wxString sciezka(const wxString& filename)
{
    wxString path = wxStandardPaths::Get().GetExecutablePath();
    path = wxFileName(path).GetPath();
    wxFileName fullPath(path, "kolorki");
    fullPath.AppendDir("kolorki");
    fullPath.SetFullName(filename);

    return fullPath.GetFullPath();
}

#include <wx/wx.h>
#include <wx/dcbuffer.h>

class RoundedButton : public wxBitmapButton {
public:
    RoundedButton(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxButtonNameStr)
        : wxBitmapButton(parent, id, bitmap, pos, size, style, validator, name) {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
    }

protected:
    void OnPaint(wxPaintEvent& event) {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        wxSize size = GetSize();

        // Ustawienie koloru tła na pastelowy niebieski
        wxColour bgColor(173, 216, 230);
        // Rysowanie prostokątnego tła
        dc.SetBrush(wxBrush(bgColor));
        dc.SetPen(wxPen(bgColor));
        dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight()); // Rysowanie prostokąta zamiast zaokrąglonych rogów

        // Rysowanie bitmapy
        wxBitmap bitmap = GetBitmapLabel();
        if (bitmap.IsOk()) {
            dc.DrawBitmap(bitmap, (size.GetWidth() - bitmap.GetWidth()) / 2, (size.GetHeight() - bitmap.GetHeight()) / 2, true);
        }
    }

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(RoundedButton, wxBitmapButton)
    EVT_PAINT(RoundedButton::OnPaint)
wxEND_EVENT_TABLE()


wxString ikona_przed_usunieciem(const wxString& kolor) {
    if (kolor.Contains("Zielony.png")) return sciezka("Zielonyusun.png");
    if (kolor.Contains("Czerwony.png")) return sciezka("Czerwonyusun.png");
    if (kolor.Contains("Niebieski.png")) return sciezka("Niebieskiusun.png");
    if (kolor.Contains("Fioletowy.png")) return sciezka("Fioletowyusun.png");
    if (kolor.Contains("Zolty.png")) return sciezka("Zoltyusun.png");
    if (kolor.Contains("Rozowy.png")) return sciezka("Rozowyusun.png");
    if (kolor.Contains("Bronzowy.png")) return sciezka("Bronzowyusun.png");
    return kolor; // Domyślnie zwracamy oryginalny kolor
}



// Funkcja, która losuje numer w zakresie
int random(int min, int max)
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

// Funkcja losowo wybierająca kolor
wxString los_wybor_koloru(int wielkosc) {
    int a = random(1, wielkosc);
    if (a == 1) return sciezka("Zielony.png");
    else if (a == 2) return sciezka("Czerwony.png");
    else if (a == 3) return sciezka("Niebieski.png");
    else if (a == 4) return sciezka("Fioletowy.png");
    else if (a == 5) return sciezka("Rozowy.png");
    else if (a == 6) return sciezka("Bronzowy.png");
    else return sciezka("Zolty.png");
}



//Funkcje która weźmie macierz reprezentującą kolory naszych kafelków
// i zwróci macierz 0 i 1 gdzie 1 będą do wymiany

void kafelki_do_wymienienia(wxVector<wxVector<wxString>>& a, wxVector<wxVector<int>>& b, int& dodatkowy_czas, int& dodatkowy_wynik) {
    bool zmiana = false;

    // Licznik zniszczonych klocków w jednym ruchu
    int liczba_zniszczonych = 0;

    // Sprawdzanie poziomo
    for (size_t j = 0; j < a.size(); ++j) { // Wiersze
        for (size_t i = 0; i < a[j].size(); ++i) { // Kolumny
            size_t k = 0;
            while (i + k < a[j].size() && a[j][i] == a[j][i + k]) {
                ++k;
            }
            if (k >= 3) {
                liczba_zniszczonych += k; // Dodaj liczbę zniszczonych klocków
                for (size_t l = 0; l < k; ++l) {
                    a[j][i + l] = ikona_przed_usunieciem(a[j][i + l]);
                    b[j][i + l] = 1;
                }
                zmiana = true;
            }
        }
    }

    // Sprawdzanie pionowo
    for (size_t i = 0; i < a[0].size(); ++i) { // Kolumny
        for (size_t j = 0; j < a.size(); ++j) { // Wiersze
            size_t k = 0;
            while (j + k < a.size() && a[j][i] == a[j + k][i]) {
                ++k;
            }
            if (k >= 3) {
                liczba_zniszczonych += k; // Dodaj liczbę zniszczonych klocków
                for (size_t l = 0; l < k; ++l) {
                    a[j + l][i] = ikona_przed_usunieciem(a[j + l][i]);
                    b[j + l][i] = 1;
                }
                zmiana = true;
            }
        }
    }

    // Dodawanie czasu w zależności od liczby zniszczonych klocków
    if (liczba_zniszczonych == 3) {
        if (dodatkowy_czas == 0) {
            dodatkowy_czas = 0.5;
            dodatkowy_wynik = 10;
        }
    } else if (liczba_zniszczonych == 4) {
        if (dodatkowy_czas == 0) {
            dodatkowy_czas = 1;
            dodatkowy_wynik = 20;
        }
    } else if (liczba_zniszczonych >= 5) {
        if (dodatkowy_czas == 0) {
            dodatkowy_czas = 2;
            dodatkowy_wynik = 30;
        }
    }

    if (zmiana) {
        // Odświeżenie tylko raz po wykryciu zmian
        wxApp::GetInstance()->Yield();
    }
}


//Funkcja która ma wyświetlić komunikat o współrzędnych 1
void wspjedynek(const wxVector<wxVector<int>>& matrix) {
    wxString message;

    // Iteracja przez macierz
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 1) {
                message += wxString::Format("Jedynka na pozycji: (%zu, %zu)\n", i, j);
            }
        }
    }

    if (message.IsEmpty()) {
        message = "Brak jedynek w macierzy.";
    }

    // Wyświetlenie komunikatu
    wxMessageBox(message, "Współrzędne jedynek", wxOK | wxICON_INFORMATION);
}

void zmienkolory(wxVector<wxVector<wxString>>& a, wxVector<wxVector<int>>& b, int wielkosc) {
    // Zoptymalizowana zmiana kolorów
    for (size_t j = 0; j < a.size(); ++j) {
        for (size_t i = 0; i < a[j].size(); ++i) {
            if (b[j][i] == 1) {
                // Przesuwanie kafelków w górę
                int k = j;
                while (k > 0) {
                    a[k][i] = a[k - 1][i];
                    b[k][i] = 0;
                    --k;
                }
                a[k][i] = los_wybor_koloru(wielkosc);
                b[k][i] = 0;
            }
        }
    }
}


//Funkcja wyświetlająca macierz stringów jako messagebox
// Funkcja wyświetlająca macierz w oknie za pomocą wxMessageDialog
void pokamacierz(const wxVector<wxVector<wxString>>& matrix)
{
    wxString message;

    // Tworzymy ciąg znaków przedstawiający macierz
    for (size_t row = 0; row < matrix.size(); ++row)
    {
        for (size_t col = 0; col < matrix[row].size(); ++col)
        {
            message += matrix[row][col];
            if (col < matrix[row].size() - 1)
                message += wxT("\t");
        }
        message += wxT("\n");
    }

    // Wyświetlenie wiadomości w oknie dialogowym z przyciskiem "OK"
    wxMessageBox(message, wxT("Kolorki"), wxOK | wxCENTER);
}


bool czysamezera(const wxVector<wxVector<int>>& macierz) {
    for (const auto& wiersz : macierz) {
        for (int liczba : wiersz) {
            if (liczba != 0) {
                return false; // Jeśli znajdziemy liczbę różną od 0, zwracamy false
            }
        }
    }
    return true; // Jeśli przeszliśmy przez całą macierz, zwracamy true
}

//(*InternalHeaders(GraDialog)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

// helper functions
enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f)
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(GraDialog)
const long GraDialog::ID_BITMAPBUTTON1 = wxNewId();
const long GraDialog::ID_STATICTEXT1 = wxNewId();
const long GraDialog::ID_STATICTEXT2 = wxNewId();
const long GraDialog::ID_STATICTEXT3 = wxNewId();
const long GraDialog::ID_RADIOBUTTON1 = wxNewId();
const long GraDialog::ID_RADIOBUTTON2 = wxNewId();
const long GraDialog::ID_RADIOBUTTON3 = wxNewId();
const long GraDialog::ID_BUTTON1 = wxNewId();
const long GraDialog::ID_BUTTON2 = wxNewId();
const long GraDialog::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(GraDialog, wxDialog)
    //(*EventTable(GraDialog)
    //*)
END_EVENT_TABLE()

GraDialog::GraDialog(wxWindow* parent, wxWindowID id)
{
    // Ustawienie koloru tła całego okna na pastelowy niebieski
    SetBackgroundColour(wxColour(173, 216, 230)); // Pastelowy niebieski

    dodatkowy_czas = 0;
    czas_poczatkowy = 90;
    wynik = 0;
    wielkosc = 5;
//(*Initialize(GraDialog)
wxBoxSizer* BoxSizer1;
wxFlexGridSizer* FlexGridSizer1;
wxFlexGridSizer* FlexGridSizer2;
wxGridSizer* GridSizer1;

Create(parent, wxID_ANY, _("Bear Crush"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
FlexGridSizer1 = new wxFlexGridSizer(1, 2, 0, 0);
FlexGridSizer1->AddGrowableCol(0);
GridSizer1 = new wxGridSizer(13, 13, 0, 0);

BitmapButton1 = new RoundedButton(this, ID_BITMAPBUTTON1, wxBitmap(wxImage((sciezka("Czerwony.png")))), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));

GridSizer1->Add(BitmapButton1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
FlexGridSizer1->Add(GridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
FlexGridSizer2 = new wxFlexGridSizer(6, 1, 0, 0);
FlexGridSizer2->AddGrowableRow(4);

// Zmiana koloru tekstu dla StaticText1
StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Czas"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
wxFont StaticText1Font(20, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Times New Roman"), wxFONTENCODING_DEFAULT);
StaticText1->SetFont(StaticText1Font);
StaticText1->SetForegroundColour(wxColour(106, 125, 156)); // Ustaw kolor tekstu
FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

// Zmiana koloru tekstu dla StaticText2
StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Wynik"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
wxFont StaticText2Font(20, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Times New Roman"), wxFONTENCODING_DEFAULT);
StaticText2->SetFont(StaticText2Font);
StaticText2->SetForegroundColour(wxColour(106, 125, 156)); // Ustaw kolor tekstu
FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Poziom trudności"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
wxFont StaticText3Font(10, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Times New Roman"), wxFONTENCODING_DEFAULT);
StaticText3->SetFont(StaticText3Font);
StaticText3->SetForegroundColour(wxColour(106, 125, 156)); // Ustaw kolor tekstu


FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);

// RadioButton1
RadioButton1 = new wxRadioButton(this, ID_RADIOBUTTON1, _("Łatwy"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
RadioButton1->SetForegroundColour(wxColour(106, 125, 156)); // Ustaw kolor tekstu
BoxSizer1->Add(RadioButton1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

// RadioButton2
RadioButton2 = new wxRadioButton(this, ID_RADIOBUTTON2, _("Średni"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
RadioButton2->SetForegroundColour(wxColour(106, 125, 156)); // Ustaw kolor tekstu
RadioButton2->SetValue(true);
BoxSizer1->Add(RadioButton2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

// RadioButton3
RadioButton3 = new wxRadioButton(this, ID_RADIOBUTTON3, _("Trudny"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
RadioButton3->SetForegroundColour(wxColour(106, 125, 156)); // Ustaw kolor tekstu
BoxSizer1->Add(RadioButton3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

FlexGridSizer2->Add(BoxSizer1, 1, wxALL|wxEXPAND, 4);

// Wczytaj i przeskaluj grafikę dla Button1
wxImage imageButton1(sciezka("nowagra.png")); // Wczytaj obraz
imageButton1.Rescale(200, 100, wxIMAGE_QUALITY_HIGH); // Przeskaluj obraz

// Utwórz przycisk z przeskalowaną grafiką
Button1 = new wxBitmapButton(this, ID_BUTTON1, wxBitmap(imageButton1), wxDefaultPosition, wxSize(200, 100), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BUTTON1"));
wxFont Button1Font(20, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Blackadder ITC"), wxFONTENCODING_DEFAULT);
Button1->SetFont(Button1Font);
FlexGridSizer2->Add(Button1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

// Wczytaj i przeskaluj grafikę dla Button2
wxImage imageButton2(sciezka("tworcy.png")); // Wczytaj obraz
imageButton2.Rescale(200, 100, wxIMAGE_QUALITY_HIGH); // Przeskaluj obraz

// Utwórz przycisk z przeskalowaną grafiką
Button2 = new wxBitmapButton(this, ID_BUTTON2, wxBitmap(imageButton2), wxDefaultPosition, wxSize(200, 100), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BUTTON2"));
wxFont Button2Font(20, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Blackadder ITC"), wxFONTENCODING_DEFAULT);
Button2->SetFont(Button2Font);
FlexGridSizer2->Add(Button2, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);


FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
SetSizer(FlexGridSizer1);
Timer1.SetOwner(this, ID_TIMER1);
Timer1.Start(17, false);
FlexGridSizer1->Fit(this);
FlexGridSizer1->SetSizeHints(this);


Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GraDialog::OnBitmapButton1Click);
Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&GraDialog::OnRadioButton1Select);
Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&GraDialog::OnRadioButton2Select);
Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&GraDialog::OnRadioButton3Select);
Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GraDialog::OnButton1Click);
Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GraDialog::OnButton2Click);
Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&GraDialog::OnTimer1Trigger);
//*)



    pola[0] = BitmapButton1;
    wxString kolorki1d[169];
    kolorki1d[0]= sciezka("Czerwony.png");


    for (int i = 1; i < 169; i++) {
        kolorki1d[i]=los_wybor_koloru(wielkosc);
        pola[i] = new RoundedButton(this, wxID_ANY, wxBitmap(wxImage(kolorki1d[i])), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
        GridSizer1->Add(pola[i], 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
        pola[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &GraDialog::OnBitmapButton1Click, this);

        }

    for(int i = 0; i < 169; i++) {
    id2nr[pola[i]->GetId()] = i;
    }

    kolorki2d.resize(13, wxVector<wxString>(13));

    int i=0;
    for(int k=0;k<13;k++){
        for(int l=0;l<13;l++){
            kolorki2d[k][l]=kolorki1d[i++];
            }
    }

    //Inizjalizacja macierzy zer i jedynek
    wxVector<wxVector<int>> zeraijedynki(13, wxVector<int>(13, 0));

    //Tu upewniam się że po stworzeniu macierzy nie ma żadnych 3+ kolorów w lini
    kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
    while(czysamezera(zeraijedynki)==false){
       kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
       zmienkolory(kolorki2d,zeraijedynki, wielkosc);
       kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
    }
    AktualizujKolory();
    time_point = std::chrono::high_resolution_clock::now();
    wxMessageBox(_("Gra polega na układaniu pionowo lub poziomo misiów o tych samych kolorach. Klikając najpierw na jednego misia, a później na drugiego zamieniamy je miejscami. Celem jest zdobycie jak największej liczby punktów za zbijanie misiów. W zależności od wybranego poziomu trudności masz określony czas w którym musisz się zmieścić (łatwy - 120 sekund, średni - 90 sekund, trudny - 60 sekund). Domyślnie zaczynasz od poziomu średniego. Ale nie martw się, każde zbicie doda trochę czasu. Powdzenia! "), "Tutorial", wxOK | wxICON_INFORMATION);
    time_point = std::chrono::high_resolution_clock::now();
    StaticText1->SetLabel(wxString::Format("%3d", czas_poczatkowy)); // Reset wyświetlania czasu
    Timer1.Start(17, false); // Ponowne uruchomienie timera
    StaticText2->SetLabel(wxString::Format("%3d", wynik));

    SetIcon(wxICON(aaaa));
}




GraDialog::~GraDialog()
{
    //(*Destroy(GraDialog)
    //*)
}

void GraDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void GraDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


//To jest dodana przeze mnie metoda do klasy gra, która ma aktualizować cały interfejs o kolorki2d
void GraDialog::AktualizujKolory() {
    int index = 0;
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            wxImage image(kolorki2d[i][j]);
            if (!image.HasAlpha()) {
                image.InitAlpha();
            }
            pola[index]->SetBitmap(wxBitmap(image));
            ++index;
        }
    }

    // Tylko raz odświeżamy widok po zakończeniu wszystkich operacji
    Layout();
    Refresh();
}
void GraDialog::OnBitmapButton1Click(wxCommandEvent& event) {
    wxVector<wxVector<int>> zeraijedynki(13, wxVector<int>(13, 0));
    int dodatkowy_czas = 0; // Zmienna na dodatkowy czas
    int dodatkowy_wynik = 0;
    int id = id2nr[event.GetId()];
    int row = id / 13;
    int col = id % 13;

    if (r == 69 && c == 69 && row!=r && col !=c) {
        r = row;
        c = col;
    } else if (abs(row - r) < 2 && abs(col - c) < 2 && (row == r || col == c)) {
        x = kolorki2d[r][c];
        kolorki2d[r][c] = kolorki2d[row][col];
        kolorki2d[row][col] = x;
        kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik); // Przekazanie zmiennej dodatkowy_czas
        if (czysamezera(zeraijedynki)) {
            wxString x = kolorki2d[row][col];
            kolorki2d[row][col] = kolorki2d[r][c];
            kolorki2d[r][c] = x;
            r = 69;
            c = 69;
        }else{
        AktualizujKolory();
        r = 69;
        c = 69;

        kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
        AktualizujKolory(); // Odświeżenie widoku po zmianie na "przed usunięciem"

        wxApp::GetInstance()->Yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        while (czysamezera(zeraijedynki) == false) {
            kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
            zmienkolory(kolorki2d, zeraijedynki,wielkosc);
            kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
        }
        AktualizujKolory();
        wynik += dodatkowy_wynik;
    StaticText2->SetLabel(wxString::Format("%3d", wynik));
    }
    } else {}

    // Dodanie czasu do czasu gry
    czas_poczatkowy += dodatkowy_czas;

    /*
    wxString message = wxString::Format("Kliknięto kafelek: wiersz = %d, kolumna = %d; id = %d", row, col, id);
    wxMessageBox(message, "Informacja", wxOK | wxICON_INFORMATION);
    */
}


void GraDialog::OnTimer1Trigger(wxTimerEvent& event) {
    auto tp = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(tp - time_point).count();
    int remaining_time = czas_poczatkowy - elapsed; // Użyj aktualnej wartości czas_poczatkowy

    if (remaining_time <= 0) {
        StaticText1->SetLabel("0.0");
        Timer1.Stop(); // Zatrzymaj licznik, gdy czas dobiegnie końca
        wxMessageBox(_("Koniec czasu! Twój wynik to : ") + wxString::Format("%3d", wynik), "Informacja", wxOK | wxICON_INFORMATION);
        for (int i = 0; i < 169; ++i) {
            if (pola[i]) {
                pola[i]->Disable(); // Wyłączanie każdego bitmap button
            }
        }
    } else {
        wxString w = wxString::Format("%3d", remaining_time);
        StaticText1->SetLabel(w);
    }
}


void GraDialog::OnButton1Click(wxCommandEvent& event)
{
    for (int i = 0; i < 169; ++i) {
        if (pola[i]) {
            pola[i]->Enable();
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int k = 0; k < 13; k++) {
            for (int l = 0; l < 13; l++) {
                kolorki2d[k][l] = los_wybor_koloru(wielkosc);
            }
        }
        wxVector<wxVector<int>> zeraijedynki(13, wxVector<int>(13, 0));
        kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
        while (czysamezera(zeraijedynki) == false) {
            kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
            zmienkolory(kolorki2d, zeraijedynki, wielkosc);
            kafelki_do_wymienienia(kolorki2d, zeraijedynki, dodatkowy_czas, dodatkowy_wynik);
        }
        AktualizujKolory();
    }
    dodatkowy_czas = 0;
    wynik = 0;
    dodatkowy_wynik = 0;
    StaticText2->SetLabel(wxString::Format("%3d", wynik));

    // Ustaw czas początkowy na aktualną wartość
    time_point = std::chrono::high_resolution_clock::now();
    StaticText1->SetLabel(wxString::Format("%3d", czas_poczatkowy)); // Reset wyświetlania czasu
    Timer1.Stop(); // Zatrzymaj timer
    Timer1.Start(17, false); // Uruchom timer ponownie
}


void GraDialog::OnButton2Click(wxCommandEvent& event)
{
    Timer1.Stop();
    auto pause_time = std::chrono::high_resolution_clock::now();

    // Wyświetl okno dialogowe z informacją o twórcach
    wxMessageBox(_("Gra została stworzona przez: Łukasz Walczak, Ksawery Trębacz, Łukasz Skrzypczak"), _("Twórcy"), wxOK | wxICON_INFORMATION);

    // Oblicz, ile czasu upłynęło podczas pauzy
    auto resume_time = std::chrono::high_resolution_clock::now();
    auto pause_duration = std::chrono::duration_cast<std::chrono::seconds>(resume_time - pause_time).count();

    // Przesuń punkt początkowy czasu o czas pauzy
    time_point += std::chrono::seconds(pause_duration);

    Timer1.Start(17, false);
}

void GraDialog::OnRadioButton3Select(wxCommandEvent& event)
{
    wielkosc = 7; // Ustawienie liczby kolorów na 7 (trudny poziom)
    czas_poczatkowy = 60; // Ustawienie czasu gry na 60 sekund

    // Resetowanie gry po zmianie poziomu trudności
    wxCommandEvent dummyEvent;
    OnButton1Click(dummyEvent);

    // Zresetuj timer
    time_point = std::chrono::high_resolution_clock::now(); // Zresetuj czas początkowy
    Timer1.Stop(); // Zatrzymaj timer
    Timer1.Start(17, false); // Uruchom timer ponownie
    StaticText1->SetLabel(wxString::Format("%3d", czas_poczatkowy)); // Zaktualizuj wyświetlany czas
}

void GraDialog::OnRadioButton2Select(wxCommandEvent& event)
{
    wielkosc = 5; // Ustawienie liczby kolorów na 5 (średni poziom)
    czas_poczatkowy = 90; // Ustawienie czasu gry na 90 sekund

    // Resetowanie gry po zmianie poziomu trudności
    wxCommandEvent dummyEvent;  // Pusty obiekt wxCommandEvent
    OnButton1Click(dummyEvent);

    // Zresetuj timer
    time_point = std::chrono::high_resolution_clock::now(); // Zresetuj czas początkowy
    Timer1.Stop(); // Zatrzymaj timer
    Timer1.Start(17, false); // Uruchom timer ponownie
    StaticText1->SetLabel(wxString::Format("%3d", czas_poczatkowy)); // Zaktualizuj wyświetlany czas
}

void GraDialog::OnRadioButton1Select(wxCommandEvent& event)
{
    wielkosc = 3;
    czas_poczatkowy = 120;

    // Resetowanie gry po zmianie poziomu trudności
    wxCommandEvent dummyEvent;
    OnButton1Click(dummyEvent);

    time_point = std::chrono::high_resolution_clock::now(); // Zresetuj czas początkowy
    Timer1.Stop(); // Zatrzymaj timer
    Timer1.Start(17, false); // Uruchom timer ponownie
    StaticText1->SetLabel(wxString::Format("%3d", czas_poczatkowy)); // Zaktualizuj wyświetlany czas
}
