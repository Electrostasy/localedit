#include "ExpandingTabWidget.h"

ExpandingTabWidget::ExpandingTabWidget() {
	this->setTabBar(new ExpandingTabBar());
	this->tabBar()->setDocumentMode(true);
}
