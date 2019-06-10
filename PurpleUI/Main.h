#ifndef MAIN_H
#define MAIN_H

#include "UIMain.h"
#include "MainSurface.h"

class Main : public UIMain
{
public:
	virtual void OnRegisterSurface()override;
};
#endif