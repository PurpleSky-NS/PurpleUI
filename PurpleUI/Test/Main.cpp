#include "Main.h"
void Main::OnRegisterSurface()
{
	RegisterSurface("MainSurface", new MainSurface);
	LaunchSurface("MainSurface");
}
int main()
{
	UIMain::SetUIMain(new Main);
	UIMain::Run();
}