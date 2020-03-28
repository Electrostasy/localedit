#include "StagesEditorWidget.h"

StagesEditorWidget::StagesEditorWidget(MissionListWidget *missionListWidget) {
	this->associatedListWidget = missionListWidget;

	ownerPage = new QWidget();
	dispatchPage = new QWidget();

	ownerPage->setLayout(new QVBoxLayout());
	dispatchPage->setLayout(new QVBoxLayout());

	tabWidget = new QTabWidget();
	tabWidget->addTab(ownerPage, "Owner");
	tabWidget->addTab(dispatchPage, "Dispatch");

	auto *layout = new QVBoxLayout(this);
	layout->addWidget(tabWidget);

	// Items selected in the mission list will update the stages
	connect(associatedListWidget->missionList, &QTreeWidget::currentItemChanged, this, &StagesEditorWidget::showMissionStages);
}

void StagesEditorWidget::showMissionStages(QTreeWidgetItem *currentItem) {
	shownStages.clear();
	clearLayout(ownerPage->layout());
	clearLayout(dispatchPage->layout());

	auto missionCode = currentItem->data(0, MissionListWidget::Metadata::MissionCode).toString();

	buildMutableStages(associatedListWidget->missionMap[missionCode], true);
	buildMutableStages(associatedListWidget->missionMap[missionCode], false);
}

void StagesEditorWidget::buildMutableStages(MissionListWidget::Mission &mutableMission, const bool &forOwner) {
	// Pointer to a pointer of a QWidget containing the page
	QWidget **page = nullptr;
	// Pointer to an already existing QVector holding QStrings
	QVector<QString> *stages;
	if(forOwner) {
		page = &ownerPage;
		stages = &mutableMission.stagesOwner;
	} else {
		page = &dispatchPage;
		stages = &mutableMission.stagesDispatch;
	}

	// If there are more stages than the main window can show, we insure ourselves with a scroll area
	auto *scrollArea = new QScrollArea();
	auto *scrollableContent = new QWidget();
	scrollArea->setWidgetResizable(true);

	// Back background of scroll area transparent
	QPalette p = scrollArea->palette();
	p.setColor(QPalette::ColorRole::Window, Qt::GlobalColor::transparent);
	scrollArea->setPalette(p);
	scrollArea->setContentsMargins(0, 0, 0, 0);
	scrollArea->setFrameShape(QFrame::NoFrame);

	scrollArea->setWidget(scrollableContent);
	scrollableContent->setLayout(new QVBoxLayout());

	(*page)->layout()->addWidget(scrollArea);

	int stageIndex = 1;
	foreach(const QString &stage, *stages) {
		auto *box = new QGroupBox("Stage " + QString::number(stageIndex));
		box->setFlat(true);
		scrollableContent->layout()->addWidget(box);

		auto *textField = new QTextEdit(*page);
		new SyntaxHighlighter(textField->document());
		textField->setPlainText(stage);
		QFontMetrics fm = QFontMetrics(textField->font());
		int fieldHeight = fm.lineSpacing() * 2 + fm.height() * 3;
		textField->setFixedHeight(fieldHeight);

		auto *layout = new QVBoxLayout();
		layout->addWidget(textField);
		box->setLayout(layout);

		shownStages.push_back(textField);
		stageIndex++;
	}
}

// https://stackoverflow.com/a/56966160
void StagesEditorWidget::clearLayout(QLayout *layout) {
	QLayoutItem *layoutItem;
	while((layoutItem = layout->takeAt(0)) != nullptr) {
		if(layoutItem->layout()) {
			clearLayout(layoutItem->layout());
			layoutItem->layout()->deleteLater();
		}
		if(layoutItem->widget()) {
			layoutItem->widget()->deleteLater();
		}
		delete layoutItem;
	}
}
