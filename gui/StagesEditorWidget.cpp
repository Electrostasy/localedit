#include "StagesEditorWidget.h"

// TODO: StagesEditorWidget should have an option for parent widget, issues with the QWidget c-tor
StagesEditorWidget::StagesEditorWidget(MissionListWidget *missionListWidget) {
	ownerPage = new QWidget();
	dispatchPage = new QWidget();

	ownerPage->setLayout(new QVBoxLayout());
	dispatchPage->setLayout(new QVBoxLayout());

	auto *ownerScrollable = new QScrollArea();
	auto *dispatchScrollable = new QScrollArea();

	ownerScrollable->setWidget(ownerPage);
	dispatchScrollable->setWidget(dispatchPage);

	// Widget needs a layout, QTabWidget can't just use `this` as parent widget
	auto *layout = new QVBoxLayout(this);

	tabWidget = new QTabWidget();
	tabWidget->addTab(ownerPage, "Owner");
	tabWidget->addTab(dispatchPage, "Dispatch");

	layout->addWidget(tabWidget);

	// Items selected in the mission list will update the stages
	connect(missionListWidget->missionList, &QListWidget::currentItemChanged, this,
		[=](const QListWidgetItem *currentItem) {
			clearLayout(ownerPage->layout());
			clearLayout(dispatchPage->layout());

			auto missionCode = currentItem->data(Qt::UserRole).toString();

			populatePage(ownerPage, missionListWidget->missionStagesMap[missionCode].first);
			populatePage(dispatchPage, missionListWidget->missionStagesMap[missionCode].second);
		});
}

void StagesEditorWidget::clearLayout(QLayout *layout) {
	if(layout) {
		while(layout->count() > 0) {
			QLayoutItem *item = layout->takeAt(0);
			QWidget *widget = item->widget();
			delete widget;
			delete item;
		}
	}
}

void StagesEditorWidget::populatePage(QWidget *page, const QVector<QString> &stages) {
	int stageIndex = 1;

	foreach(const QString &stage, stages) {
		auto *textField = new QTextEdit(page);
		textField->setPlainText(stage);

		new SyntaxHighlighter(textField->document());

		auto *layout = new QVBoxLayout();
		layout->addWidget(textField);

		auto *box = new QGroupBox("Stage " + QString::number(stageIndex));
		box->setLayout(layout);
		box->setFlat(true);

		page->layout()->addWidget(box);
		stageIndex++;
	}
}
