#pragma once

#include <QWidget>
#include <QtWidgets>

class ExpandingTabBar: public QTabBar {
	public:
	[[nodiscard]] QSize tabSizeHint(int index) const override;
};
