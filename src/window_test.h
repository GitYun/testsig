#ifndef WINDOW_TEST_H
#define WINDOW_TEST_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input.H>
#include "serialport.h"

class Win_Test : public Fl_Double_Window {
public:
    Win_Test(int x, int y, int w, int h, const char* label = 0);

    ~Win_Test();

private:
    static void polltask(void* arg);

    void add_text(const char* string) {
        m_text_buffer->append(string);
        m_text->scroll(m_text_buffer->count_lines(0, m_text_buffer->length()), 0);
    }

public:
    CSerialPort m_sp;
    mySlot* m_receive;
    
    Fl_Choice* m_com;
    Fl_Button* m_open;
    Fl_Button* m_close;
    Fl_Text_Display* m_text;
    Fl_Text_Buffer* m_text_buffer;
    Fl_Button* m_sender;
    Fl_Button* m_printer;
    Fl_Button* m_json_parser;
    Fl_Button* m_exit;
};

#endif
