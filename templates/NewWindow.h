
#ifndef MYENGINE_NEWWINDOW_H
#define MYENGINE_NEWWINDOW_H
#include <MyEngine>

class NewWindow : public MyEngine::Window {
public:
    explicit NewWindow(MyEngine::Engine* engine, const std::string& title, int width = 800, int height = 600);
    ~NewWindow();

    /// TODO: Add more functions to improve your custom window.

protected:
    void paintEvent() override;
    void resizeEvent() override;
    void moveEvent() override;
    void getFocusEvent() override;
    void lostFocusEvent() override;
    void unloadEvent() override;
    /// TIPS: you can add more functions below. :)


private:
    /// TODO: Add the private members or functions here.

};

#endif //MYENGINE_NEWWINDOW_H
