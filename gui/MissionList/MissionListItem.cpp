#include "MissionListItem.h"
#include <QtWidgets/QPlainTextDocumentLayout>

MissionListItem::Stage::Stage(QTextDocument *objectives, const bool &hasOpp) {
	this->objectives = objectives;
	// Need to change the doc layout because if we don't QPlainTextEdit::setDocument starts crying that
	// Document set does not support QPlainTextDocumentLayout
	auto *documentLayout = new QPlainTextDocumentLayout(objectives);
	this->objectives->setDocumentLayout(documentLayout);
	// Adding text to the QTextDocument adds it to the undo/redo stacks
	this->objectives->clearUndoRedoStacks();
	this->hasOpp = hasOpp;
}

MissionListItem::MissionListItem() {
	this->mission = Mission();
}

MissionListItem::MissionListItem(const QString &name, const QString &code) {
	this->mission = Mission(name, code);
}

void MissionListItem::setIdentifiers(const QString &name, const QString &code) {
	this->mission.name = name;
	this->mission.code = code;

	this->setText(name);
}

void MissionListItem::pushOwnerStage(const Stage &stage) {
	this->mission.owner.push_back(stage);
}

void MissionListItem::pushDispatchStage(const Stage &stage) {
	this->mission.dispatch.push_back(stage);
}

const QVector<MissionListItem::Stage> &MissionListItem::ownerStages() {
	return this->mission.owner;
}

const QVector<MissionListItem::Stage> &MissionListItem::dispatchStages() {
	return this->mission.dispatch;
}

QString MissionListItem::name() const {
	return this->mission.name;
}

QString MissionListItem::code() const {
	return this->mission.code;
}

QString MissionListItem::name() {
	return this->mission.name;
}

QString MissionListItem::code() {
	return this->mission.code;
}

MissionListItem::MissionListItem(const MissionListItem::Mission &mission) {
	this->mission = mission;
}

MissionListItem::Mission::Mission() {
	this->name = QString();
	this->code = QString();
	this->owner = QVector<Stage>();
	this->dispatch = QVector<Stage>();
}

MissionListItem::Mission::Mission(const QString &name, const QString &code) {
	this->name = name;
	this->code = code;
	this->owner = QVector<Stage>();
	this->dispatch = QVector<Stage>();
}
