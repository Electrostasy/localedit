#include "ExpandingTabBar.h"

QSize ExpandingTabBar::tabSizeHint(int index) const {
	return QSize(this->parentWidget()->width() / QTabBar::count(), QTabBar::tabSizeHint(index).height());
}
