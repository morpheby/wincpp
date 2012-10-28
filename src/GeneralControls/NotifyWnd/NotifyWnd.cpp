
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "NotifyWnd.h"
#include <ThemedDrawer.h>

using namespace std;

NotifyWnd::NotifyWnd(void) :
		Window(L"", 0, 0, 0, 0, 0, 0, 0, 0), txt(L""), txtRect({5, 5, 0, 0}) {
	setPaintCachingMode(false);
}

NotifyWnd::NotifyWnd(const wstring &text, int x, int y, HWND parentWnd) :
		Window(L"", WS_POPUP, x, y, 0, 0, parentWnd, 0, 0), txt(text),
		txtRect({5, 5, 0, 0}) {
	setPaintCachingMode(false); // using paint caching disables support for transparency
	setText(text);
}

void NotifyWnd::setText(const wstring &text) {
	txt = text;
	txtRect = {0, 0, 0, 0};
	std::shared_ptr<Drawing::Drawer> drawer = getDrawer();

	if(shared_ptr<Drawing::ThemedDrawer> tDrawer = std::dynamic_pointer_cast<Drawing::ThemedDrawer>(drawer))
		tDrawer->setFontThemed(TTP_BALLOON, TTBS_NORMAL);

	drawer->drawText(txt, DT_CALCRECT, txtRect);

	txtRect.left += 5; txtRect.right += 5;
	txtRect.top += 5; txtRect.bottom += 5;
	setSize(txtRect.right + 5, txtRect.bottom + 5);
	ImmediatelyUpdateWindow();
}

void NotifyWnd::PaintWindow(Drawing::Drawer &drawer) {
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer))
		tDrawer->setFontThemed(TTP_BALLOON, TTBS_NORMAL);

	drawer.drawText(txt, 0, txtRect);
}

NotifyWnd::~NotifyWnd(void) {
}

bool NotifyWnd::WMEraseBackground(Drawing::Drawer &drawer) {
	RECT rect = {0, 0, getSize().cx, getSize().cy};
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer))
		tDrawer->setBackgroundThemed(TTP_BALLOON, TTBS_NORMAL);
	return Window::WMEraseBackground(drawer);
}

wstring NotifyWnd::GetThemeApplicableClassList() {
	return wstring(TOOLTIPS_CLASSW) + L";TOOLTIP;" + Window::GetThemeApplicableClassList();
	// TOOLTIPS_CLASS does not always work
}

