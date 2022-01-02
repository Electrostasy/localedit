#include "StagesEditorWidget.h"

StagesEditorWidget::StagesEditorWidget(QWidget *parent): QWidget(parent) {
	auto *layout = new QVBoxLayout(this);

	tabWidget = new ExpandingTabWidget();

	pageOwner = new FramedPage();
	pageDispatch = new FramedPage();
	pageOwner->setLayout(new QVBoxLayout());
	pageDispatch->setLayout(new QVBoxLayout());

	pageOwner->setAutoFillBackground(true);
	pageDispatch->setAutoFillBackground(true);

	QPalette p = pageOwner->palette();
	p.setColor(QPalette::ColorRole::Window, QColor(255, 255, 255));
	pageOwner->setPalette(p);
	pageDispatch->setPalette(p);

	tabWidget->addTab(pageOwner, "Owner");
	tabWidget->addTab(pageDispatch, "Dispatch");

	layout->addWidget(tabWidget);
	layout->setMargin(0);
	this->setMinimumWidth(this->fontMetrics().averageCharWidth() * 80);
}

void StagesEditorWidget::buildStages(QWidget *page, const QVector<MissionListItem::Stage> &stages, MissionListItem *currentItem) {
	if(page->layout()->count() > 0) {
		LayoutUtils::clearLayout(page->layout());
	}

	// If there are more stages than the main window can show, we insure ourselves with a scroll area
	auto scrollableLayout = new QVBoxLayout();
	auto scrollArea = setupScrollArea(scrollableLayout);

	// Build stages UI
	int stageIndex = 1;
	for(const auto &stage: stages) {
		// Stages with Opp are duplicates so we don't need to show them
		if(stage.hasOpp) {
			continue;
		}

		auto *box = new QGroupBox();
		auto *boxLayout = new QVBoxLayout();
		box->setTitle("Stage " + QString::number(stageIndex));
		box->setFlat(true);
		box->setLayout(boxLayout);

		scrollableLayout->addWidget(box);

		auto *textField = new ExpandedTextEdit(stage.objectives);
		boxLayout->addWidget(textField, 0);

		// If there are undo steps remaining, that means the mission currently being edited needs to be marked as
		// having been modified, and if not - cleared of being marked modified
		connect(textField, &QPlainTextEdit::textChanged, this, [currentItem, textField]() {
			auto icon = textField->document()->availableUndoSteps() > 0 ? QIcon(":/resources/pending_changes") : QIcon();
			currentItem->setData(Qt::ItemDataRole::DecorationRole, icon);
		});

		// TODO: this doesn't correctly set a 3 line height
		QFontMetrics fm = QFontMetrics(textField->font());
		textField->setFixedHeight(fm.lineSpacing() * 2 + fm.height() * 3);

		stageIndex++;
	}

	scrollableLayout->addStretch(9);
	page->layout()->addWidget(scrollArea);
}

QScrollArea *StagesEditorWidget::setupScrollArea(QVBoxLayout *scrollableLayout) {
	auto *scrollableContent = new QWidget();
	scrollableContent->setLayout(scrollableLayout);

	auto *scrollArea = new QScrollArea();
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(scrollableContent);

	// Back background of scroll area transparent
	QPalette p = scrollArea->palette();
	p.setColor(QPalette::ColorRole::Window, Qt::GlobalColor::transparent);
	scrollArea->setPalette(p);
	scrollArea->setContentsMargins(0, 0, 0, 0);
	scrollArea->setFrameShape(QFrame::NoFrame);
	return scrollArea;
}
