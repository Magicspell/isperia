#pragma once
#include "UI.h"
#include "Graph.h"

#define TB_WIDTH 0.07

class IsperiaApp : public UIApp {
public:
    IsperiaApp();
    void update();
protected:
    UIObject* mainScreen;
    UIToolbar* toolbar;
};