#include "window_test.h"
#include "ntohl_test.h"
#include "xlnt/xlnt.hpp"
#include "cJSON/cJSON.h"
#include <FL/Fl_Native_File_Chooser.H>
#include <vector>

Win_Test::Win_Test(int x, int y, int w, int h, const char* label) : Fl_Double_Window(x, y, w, h, label) {
    m_com = new Fl_Choice(60, 10, 80, 25, "Com: ");
    m_com->align(FL_ALIGN_LEFT);
    m_com->when(FL_WHEN_CHANGED);

    m_open = new Fl_Button(m_com->x() + m_com->w() + 10, 
        m_com->y(), 80, 25, "Open");
    m_open->clear_active();

    m_close = new Fl_Button(m_open->x() + m_open->w() + 10,
        m_open->y(), 80, 25, "Close");
    m_close->clear_active();
    
    m_text = new Fl_Text_Display(5, m_com->y() + m_com->h() + 10,
        w - 10, h - m_com->y() - m_com->h() - 55);
    m_text_buffer = new Fl_Text_Buffer();
    m_text->buffer(m_text_buffer);
    Fl_Button* clear_text = new Fl_Button(m_text->x(), m_text->y(), m_text->w(), m_text->h());
    clear_text->box(FL_NO_BOX);

    m_sender = new Fl_Button(20, h - 35, 80, 25, "Send");
    m_sender->clear_active();

    m_printer = new Fl_Button((w - 80) / 2 - 45, m_sender->y(), 80, 25, "Print");

    m_json_parser = new Fl_Button(w / 2 + 5, m_sender->y(), 80, 25, "JsonParser");

    m_exit = new Fl_Button(w - 100, m_sender->y(), 80, 25, "Exit");

    end();

    // m_sp.setMinByteReadNotify(7);
    m_receive = new mySlot(&m_sp);
    m_receive->m_echo.connect([this](std::string str) {
        std::string output = "<<: " + str;
        Fl::lock();
        this->add_text(output.c_str());
        Fl::unlock();
        Fl::awake();
    });

    m_receive->m_echo.connect([this](std::string str) {
        std::string output = "<< Copy:" + str;
        Fl::awake([](void *arg) {
            Win_Test* wt = (Win_Test *)arg;
            wt->add_text("Copy!\n");
        }, this);
    });

    m_com->callback([](Fl_Widget* w, void *d) {
        Win_Test* wt = (Win_Test *)d;
        wt->m_sp.init(wt->m_com->text(wt->m_com->value()));
    }, this);

    m_open->callback([](Fl_Widget* w, void* d) {
        Win_Test* wt = (Win_Test *)d;
        wt->m_sp.open();
        if (wt->m_sp.isOpened()) {
            wt->add_text("Open Success\n");
            wt->m_sender->activate();
            wt->m_close->activate();
            wt->m_open->clear_active();
            wt->m_open->redraw();
        } else {
            wt->add_text("Open Failed\n");
        }

        wt->m_sp.readReady = std::bind(&mySlot::OnSendMessage, wt->m_receive);
    }, this);

    m_close->callback([](Fl_Widget* w, void* d) {
        Win_Test* wt = (Win_Test *)d;
        wt->m_sp.close();
        if (!wt->m_sp.isOpened()) {
            wt->add_text("Close Success\n");
            wt->m_sender->clear_active();
            wt->m_close->clear_active();
            wt->m_open->activate();
            wt->m_sender->redraw();
            wt->m_close->redraw();
        } else {
            wt->add_text("Close Failed\n");
        }
    }, this);

    m_sender->callback([](Fl_Widget* w, void *d) {
        Win_Test* wt = (Win_Test *)d;
        if (wt->m_sp.writeData("Hello\n", 7) == 7) {
            wt->add_text(">>: Hello\n");
        }
    }, this);

    m_exit->callback([](Fl_Widget* w, void *d) {
        Win_Test* wt = (Win_Test *)d;
        Fl::default_atclose(wt, nullptr);
    }, this);

    clear_text->callback([](Fl_Widget* w, void *d) {
        Win_Test* wt = (Win_Test *)d;

        if (Fl::event_button() == FL_LEFT_MOUSE && Fl::event_clicks() != 0) {
            wt->m_text_buffer->select(0, wt->m_text_buffer->length());
            wt->m_text_buffer->remove_selection();
        }
    }, this);

    m_printer->callback([](Fl_Widget* w, void *d) {
        Win_Test* wt = (Win_Test *)d;
        Fl_Native_File_Chooser fc;

        fc.title("Output Excel File");
        fc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
        fc.filter("Excel Files\t*.{xls, xlsx}");
        fc.directory(".");
        fc.preset_file("example.xls");
        
        auto output_report = [](const char * filename) {
            xlnt::workbook wb;
            xlnt::worksheet ws = wb.active_sheet();
            ws.title("example");

            ws.cell("A1").value(5);
            ws.cell("B2").value("string data");
            ws.cell("C3").formula("=RAND()");
            ws.merge_cells("C3:C4");
            ws.freeze_panes("B2");

            u_long a = 0x12345678;
            std::ostringstream oss;
            oss << std::hex << (int)a;
            ws.cell("D4").value(oss.str());
            oss.str("");
            oss << std::hex << (int)ntohl_test(a);
            ws.cell("D5").value(oss.str());

            wb.save(filename);
        };

        if (0 == fc.show() && fc.filename()) {
            output_report(fc.filename());
        }
    }, this);

    m_json_parser->callback([](Fl_Widget* w, void* d) {
        Win_Test* wt = (Win_Test *)d;
        std::string json = "{\"app\": \"TestApp\", \"version\": \"1.0.0\"}";

        cJSON* cjson = cJSON_Parse(json.c_str());
        wt->add_text(cJSON_Print(cjson));
        cJSON_Delete(cjson);
    }, this);

    polltask(this);
    Fl::add_timeout(0.5, polltask, this);
}


Win_Test::~Win_Test() {
    m_sp.close();
}

void Win_Test::polltask(void* arg) {
    Win_Test* wt = (Win_Test *)arg;
    vector<SerialPortInfo> m_availablePortList = CSerialPortInfo::availablePortInfos();

    bool is_has_port_opend = wt->m_sp.isOpened();
    bool is_opened_port_remain = false;
    std::string opened_port_name;
    if (is_has_port_opend)
        opened_port_name = wt->m_sp.getPortName();

    for (int i = 0; i < m_availablePortList.size(); ++i) {
        if (!wt->m_com->find_item(m_availablePortList.at(i).portName.c_str()))
            int idx = wt->m_com->add(m_availablePortList.at(i).portName.c_str());

        if (!opened_port_name.empty() && !is_opened_port_remain) {
            if (std::string::npos != opened_port_name.find(m_availablePortList.at(i).portName)) {
                is_opened_port_remain = true;
            }
        }
    }

    if (is_has_port_opend && !is_opened_port_remain) {
        wt->m_sp.close();
        wt->m_close->clear_active();
        wt->m_close->redraw();
        is_has_port_opend = false;
    }

    if (m_availablePortList.size() > 0) {
        if (!is_has_port_opend) {
            wt->m_com->value(0);
            wt->m_sp.init(wt->m_com->text(0));
            wt->m_open->activate();
        }
    } else {
        if (wt->m_com->mvalue() != nullptr) {
            wt->m_com->clear();
            wt->m_com->redraw();
        }

        if (wt->m_open->active() > 0) {
            wt->m_open->clear_active();
            wt->m_open->redraw();
        }
    }
    
    Fl::repeat_timeout(0.5, polltask, arg);
}