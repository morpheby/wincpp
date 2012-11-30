/*
 * EditboxWnd.h
 *
 *  Created on: 18.05.2012
 *      Author: Morphe
 */

#ifndef EDITBOXWND_H_
#define EDITBOXWND_H_

#include <Window.h>
#include <SharedPtr.h>

namespace EditboxInternal {
class EditboxWndInternal;
}

class EditboxWnd : public Window {
public:
	EditboxWnd();
	EditboxWnd(const std::wstring& initialText, int x, int y,
			int width, int height, HWND parent);
	EditboxWnd(HWND convertFrom);
	~EditboxWnd();

	void setOnTextChange(WndEventExtBase<const std::wstring&> *onTextChange) {
		onTextChange_ = onTextChange;
	}

	void setOnTextInput(WndEventExtBase<const std::wstring&> *onTextInput) {
		onTextInput_ = onTextInput;
	}

	void setText(const std::wstring& text);
	std::wstring getText() const {
		return text_;
	}
	void setEmptyText(const std::wstring& eText);
	void setSelection(int start, int end);

	void setReadonly(bool ro);

	EditboxInternal::EditboxWndInternal& getEBInternal() const;

protected:
	EditboxWnd(int style, const std::wstring& initialText, int x, int y,
				int width, int height, HWND parent);

	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void WMSize();

	bool WMSetFocus();
	bool WMKillFocus();

private:
	std::wstring text_, eText_;
	WndEventExtCaller<const std::wstring&> onTextChange_;
	WndEventExtCaller<const std::wstring&> onTextInput_;
	std::unique_ptr<EditboxInternal::EditboxWndInternal> eb;
	int OnSetFocusInternal(Window& sender);
	int OnKillFocusInternal(Window& sender);
	void InitEBInternal();
};
#endif /* EDITBOXWND_H_ */
