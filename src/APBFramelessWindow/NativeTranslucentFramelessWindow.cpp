#include "NativeTranslucentFramelessWindow.h"

NativeTranslucentFramelessWindow::NativeTranslucentFramelessWindow(QWidget *parent): QWidget(parent), borderWidth(8) {
	// Setup title bar
	// this->setWindowTitle(applicationName);
	this->titleBar = new APBFramelessWindowTitleBar();
	this->titleBar->setAutoFillBackground(true);
	this->titleBar->setFixedHeight(60);
	// TODO: Add native window minimizing, maximizing, restoring and closing
	connect(this->titleBar, &APBFramelessWindowTitleBar::minimizeButtonClicked, this, &QWidget::showMinimized);
	connect(this->titleBar, &APBFramelessWindowTitleBar::restoreButtonClicked, this, &QWidget::showNormal);
	connect(this->titleBar, &APBFramelessWindowTitleBar::maximizeButtonClicked, this, &QWidget::showMaximized);
	connect(this->titleBar, &APBFramelessWindowTitleBar::closeButtonClicked, this, &QWidget::close);
	// connect(this, &APBFramelessWindow::setWindowTitle, this->titleBar(), &APBFramelessWindowTitleBar::setTitleBarText);
	this->setLayout(new QVBoxLayout());
	this->layout()->addWidget(this->titleBar);

	// This flag is important for seeing behing the Qt painting of widgets and actually seeing the blur behind
	this->setAttribute(Qt::WA_TranslucentBackground);

	// Enable blur behind windows
	HRESULT (WINAPI * _pSetWindowCompositionAttribute)(HWND, WINCOMPATTR *) = nullptr;
	if (HINSTANCE user32 = LoadLibrary(R"(user32.dll)"); user32 != nullptr) {
		_pSetWindowCompositionAttribute = (HRESULT(WINAPI*)(HWND, WINCOMPATTR*))GetProcAddress(user32, "SetWindowCompositionAttribute");
	}

	const int dropShadowFlag = 0x20 | 0x40 | 0x80 | 0x100;
	DWMACCENTPOLICY policy = {ACCENT_ENABLE_BLURBEHIND, dropShadowFlag, 0, 0};

	const DWORD WCA_ACCENT_POLICY = 19;
	HWND hwnd = (HWND)this->window()->winId();
	WINCOMPATTR data = {WCA_ACCENT_POLICY, (WINCOMPATTR_DATA*)&policy, sizeof(WINCOMPATTR_DATA)};
	_pSetWindowCompositionAttribute(hwnd, &data);

	// Drop shadow
	QtWin::extendFrameIntoClientArea(this, 1, 1, 1, 1);

	// Frameless style with aero-snap friendly flags
	::SetWindowLongPtrW(hwnd, GWL_STYLE, WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
}

// https://stackoverflow.com/a/39735058
bool NativeTranslucentFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	MSG *nativeMessage = static_cast<MSG *>(message);
	LPARAM lparam = nativeMessage->lParam;
	// WPARAM wparam = nativeMessage->wParam;
	// HWND hwnd = nativeMessage->hwnd;
	UINT msg = nativeMessage->message;

	switch(msg) {
		case WM_NCCALCSIZE: {
			// Abort computing & drawing of the non client area.
			*result = 0;

			// Do not compute any further
			return true;
		}
		case WM_NCHITTEST: {
			// ensure *oResult = 0 as this will be the return result in case nothing is handled in this function,
			// that means we are in client area and the Non Client Event shouldn't be processed.
			*result = 0;

			// Collect rect and size data
			bool resizeHorizontal = minimumWidth() != maximumWidth();
			bool resizeVertical = minimumHeight() != maximumHeight();

			int x = ((int)(short)LOWORD(nativeMessage->lParam));
			int y = ((int)(short)HIWORD(nativeMessage->lParam));

			// Shape the data to be compatible with windows API stuff.
			const LONG border_width = borderWidth;
			RECT windowRect;
			GetWindowRect(reinterpret_cast<HWND>(this->winId()), &windowRect);
			QRect rect(
				windowRect.left,
				windowRect.top,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top
			);

			if(resizeHorizontal) {
				if(isLeftBorderHit(rect, border_width, x, y)) *result = HTLEFT;
				if(isRightBorderHit(rect, border_width, x, y)) *result = HTRIGHT;
			}

			if(resizeVertical) {
				if(isBottomBorderHit(rect, border_width, x, y)) *result = HTBOTTOM;
				if(isTopBorderHit(rect, border_width, x, y)) *result = HTTOP;
			}

			if(resizeHorizontal && resizeVertical) {
				if(isBottomLeftCornerHit(rect, border_width, x, y)) *result = HTBOTTOMLEFT;
				if(isBottomRightCornerHit(rect, border_width, x, y)) *result = HTBOTTOMRIGHT;
				if(isTopLeftCornerHit(rect, border_width, x, y)) *result = HTTOPLEFT;
				if(isTopRightCornerHit(rect, border_width, x, y)) *result = HTTOPRIGHT;
			}

			if(*result == 0) {
				if(isTitleBarHit(rect, border_width, x, y)) *result = HTCAPTION;
			}

			if(*result != 0) {
				// Message handled
				return true;
			}
		}
		default: {
			// Handle everything else
			return QWidget::nativeEvent(eventType, message, result);
		}
	}
}

bool NativeTranslucentFramelessWindow::isLeftBorderHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return iX >= iRect.left() && iX < iRect.left() + iBorderWidth;
}

bool NativeTranslucentFramelessWindow::isRightBorderHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	// Default implementation can be overriden in childs of this class.
	return iX < iRect.right() && iX >= iRect.right() - iBorderWidth;
}

bool NativeTranslucentFramelessWindow::isTopBorderHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return iY >= iRect.top() && iY < iRect.top() + iBorderWidth;
}

bool NativeTranslucentFramelessWindow::isBottomBorderHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return iY < iRect.bottom() && iY >= iRect.bottom() - iBorderWidth;
}

bool NativeTranslucentFramelessWindow::isTopLeftCornerHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return isTopBorderHit(iRect, iBorderWidth, iX, iY) && isLeftBorderHit(iRect, iBorderWidth, iX, iY);
}

bool NativeTranslucentFramelessWindow::isTopRightCornerHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return isTopBorderHit(iRect, iBorderWidth, iX, iY) && isRightBorderHit(iRect, iBorderWidth, iX, iY);
}

bool NativeTranslucentFramelessWindow::isBottomRightCornerHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return isBottomBorderHit(iRect, iBorderWidth, iX, iY) && isRightBorderHit(iRect, iBorderWidth, iX, iY);
}

bool NativeTranslucentFramelessWindow::isBottomLeftCornerHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	return isBottomBorderHit(iRect, iBorderWidth, iX, iY) && isLeftBorderHit(iRect, iBorderWidth, iX, iY);
}

bool NativeTranslucentFramelessWindow::isTitleBarHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) {
	bool isHeightSatisfied = iY >= iRect.top() && iY < iRect.top() + this->titleBar->height();
	bool isWidthSatisfied = iX >= iRect.left() && iX < iRect.right() - this->titleBar->buttonsWidth();

	return isHeightSatisfied && isWidthSatisfied;
}
